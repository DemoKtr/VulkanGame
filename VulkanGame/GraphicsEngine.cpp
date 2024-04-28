#include "GraphicsEngine.h"
#include "Instance.h"
#include "Logging.h"
#include "Device.h"
#include "Pipeline.h"
#include "Swapchain.h"
#include "FrameBuffer.h"
#include "Commands.h"
#include "Synchronizer.h"


void GraphicsEngine::make_instance()
{
	this->instance = vkInit::make_instance(this->debugMode, this->appName);
	this->dldi = vk::DispatchLoaderDynamic(instance, vkGetInstanceProcAddr);
	if (debugMode) {
		debugMessenger = vkInit::make_debug_messenger(instance, dldi);
	}
	VkSurfaceKHR c_style_surface;
	if (glfwCreateWindowSurface(instance, mainWindow, nullptr, &c_style_surface) != VK_SUCCESS) {
		if (debugMode) {
			std::cout << "Failed to abstract glfw surface for Vulkan\n";
		}
	}
	else if (debugMode) {
		std::cout << "Successfully abstracted glfw surface for Vulkan\n";
	}
	surface = c_style_surface;
}
void GraphicsEngine::make_debug_messenger()
{
	debugMessenger = vkInit::make_debug_messenger(instance, dldi);
}
void GraphicsEngine::choice_device()
{
	this->physicalDevice = vkInit::choose_physical_device(instance, debugMode);
	this->device = vkInit::create_logical_device(physicalDevice, surface,debugMode);
	std::array<vk::Queue, 2> queues = vkInit::get_Queues(physicalDevice, device, surface, debugMode);
	this->graphicsQueue = queues[0];
	this->presentQueue = queues[1];
	this->create_swapchain();
	frameNumber = 0;
}
void GraphicsEngine::create_pipeline()
{
	vkInit::GraphicsPipelineInBundle specification = {};
	specification.device = device;
	specification.vertexFilePath = "shaders/vert.spv";
	specification.fragmentFilePath = "shaders/frag.spv";
	specification.swapchainExtent = swapchainExtent;
	specification.swapchainImageFormat = swapchainFormat;

	vkInit::GraphicsPipelineOutBundle output = vkInit::create_graphic_pipeline(specification,debugMode);
	layout = output.layout;
	renderpass = output.renderpass;
	graphicsPipeline = output.graphicsPipeline;

}
void GraphicsEngine::create_swapchain()
{
	vkInit::SwapChainBundle bundle = vkInit::create_swapchain(physicalDevice, device, surface, screenSize, debugMode);
	this->swapchain = bundle.swapchain;
	this->swapchainFrames = bundle.frames;
	this->swapchainFormat = bundle.format;
	this->swapchainExtent = bundle.extent;
	//vkInit::query_swapchain_support(physicalDevice, surface, debugMode);
	maxFramesInFlight = static_cast<int>(swapchainFrames.size());
}
////////////////////////////////////
GraphicsEngine::GraphicsEngine(ivec2 screenSize, GLFWwindow* window, bool debugMode)
{
	this->screenSize = screenSize;
	this->mainWindow = window;
	this->debugMode = debugMode;
	if (debugMode) {
		std::cout << "Making a graphics engine\n";
	}

	
	make_instance();
	choice_device();
	create_pipeline();
	finalize_setup();
}



GraphicsEngine::~GraphicsEngine()
{
	device.waitIdle();
	if (debugMode) {
		std::cout << "End!\n";
	}
	
	device.destroyCommandPool(commandPool);
	device.destroyPipeline(graphicsPipeline);
	
	device.destroyRenderPass(renderpass);
	device.destroyPipelineLayout(layout);
	this->cleanup_swapchain();
	device.destroy();

	instance.destroySurfaceKHR(surface);

	if (debugMode) {
		instance.destroyDebugUtilsMessengerEXT(debugMessenger, nullptr, dldi);
	}
	
	instance.destroy();
	glfwTerminate();

}

void GraphicsEngine::cleanup_swapchain()
{
	for (vkUtil::SwapChainFrame frame : swapchainFrames) {
		device.destroyImageView(frame.imageView);
		device.destroyFramebuffer(frame.framebuffer);
		device.destroyFence(frame.inFlight);
		device.destroySemaphore(frame.imageAvailable);
		device.destroySemaphore(frame.renderFinished);
	}
	device.destroySwapchainKHR(swapchain);
}

void GraphicsEngine::finalize_setup()
{
	create_framebuffers();


	commandPool = vkInit::make_command_pool( physicalDevice, device, surface, debugMode);

	vkInit::commandBufferInputChunk commandBufferInput = { device, commandPool, swapchainFrames };
	maincommandBuffer = vkInit::make_command_buffer(commandBufferInput, debugMode);
	vkInit::make_frame_command_buffers(commandBufferInput, debugMode);

	create_sync_objects();

}

void GraphicsEngine::recreate_swapchain()
{
	this->screenSize.x = 0;
	this->screenSize.y = 0;
	while (this->screenSize.x == 0 || this->screenSize.y == 0) {
		glfwGetFramebufferSize(mainWindow, &this->screenSize.x, &this->screenSize.y);
		glfwWaitEvents();
	}

	device.waitIdle();

	cleanup_swapchain();
	create_swapchain();
	create_framebuffers();
	create_sync_objects();
	vkInit::commandBufferInputChunk commandBufferInput = { device, commandPool, swapchainFrames };
	vkInit::make_frame_command_buffers(commandBufferInput, debugMode);
}

void GraphicsEngine::record_draw_commands(vk::CommandBuffer commandBuffer, uint32_t imageIndex)
{
	vk::CommandBufferBeginInfo beginInfo = {};
	ang += 0.001f;
	transform.rotate(glm::vec3(0,0,1),0.01f);
	transform.computeModelMatrix();
	try {
		commandBuffer.begin(beginInfo);
	}
	catch (vk::SystemError err) {
		if (debugMode) {
			std::cout << "Failed to begin recording command buffer!" << std::endl;
		}
	}

	vk::RenderPassBeginInfo renderPassInfo = {};
	renderPassInfo.renderPass = renderpass;
	renderPassInfo.framebuffer = swapchainFrames[imageIndex].framebuffer;
	renderPassInfo.renderArea.offset.x = 0;
	renderPassInfo.renderArea.offset.y = 0;
	renderPassInfo.renderArea.extent = swapchainExtent;

	vk::ClearValue clearColor = { std::array<float, 4>{1.0f, 0.5f, 0.25f, 1.0f} };
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;

	commandBuffer.beginRenderPass(&renderPassInfo, vk::SubpassContents::eInline);

	commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, graphicsPipeline);
	vkUtil::ObjectData objectData;
	objectData.modelMatrix = this->transform.getModelMatrix();
	commandBuffer.pushConstants(layout,vk::ShaderStageFlagBits::eVertex,0,sizeof(objectData), &objectData);
	commandBuffer.draw(3, 1, 0, 0);

	commandBuffer.endRenderPass();

	try {
		commandBuffer.end();
	}
	catch (vk::SystemError err) {

		if (debugMode) {
			std::cout << "failed to record command buffer!" << std::endl;
		}
	}
}


void GraphicsEngine::render()
{
	
	
	device.waitForFences(1, &swapchainFrames[frameNumber].inFlight, VK_TRUE, UINT64_MAX);
	device.resetFences(1, &swapchainFrames[frameNumber].inFlight);
	
	//acquireNextImageKHR(vk::SwapChainKHR, timeout, semaphore_to_signal, fence)
	uint32_t imageIndex{ device.acquireNextImageKHR(swapchain, UINT64_MAX,  swapchainFrames[frameNumber].imageAvailable, nullptr).value };

	
	
	vk::CommandBuffer commandBuffer = swapchainFrames[frameNumber].commandBuffer;

	commandBuffer.reset();

	record_draw_commands(commandBuffer, imageIndex);

	vk::SubmitInfo submitInfo = {};

	vk::Semaphore waitSemaphores[] = { swapchainFrames[frameNumber].imageAvailable };
	vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	vk::Semaphore signalSemaphores[] = { swapchainFrames[frameNumber].renderFinished };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	try {
		graphicsQueue.submit(submitInfo, swapchainFrames[frameNumber].inFlight);
	}
	catch (vk::SystemError err) {

		if (debugMode) {
			std::cout << "failed to submit draw command buffer!" << std::endl;
		}
	}

	vk::PresentInfoKHR presentInfo = {};
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	vk::SwapchainKHR swapChains[] = { swapchain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;

	presentInfo.pImageIndices = &imageIndex;

	presentQueue.presentKHR(presentInfo);
	frameNumber = (frameNumber + 1) % maxFramesInFlight;
}

void GraphicsEngine::create_sync_objects()
{
	for (vkUtil::SwapChainFrame& frame : swapchainFrames) //referencja 
	{
		frame.inFlight = vkInit::make_fence(device, debugMode);
		frame.imageAvailable = vkInit::make_semaphore(device, debugMode);
		frame.renderFinished = vkInit::make_semaphore(device, debugMode);
	}
}

void GraphicsEngine::create_framebuffers()
{
	vkInit::framebufferInput frameBufferInput;
	frameBufferInput.device = device;
	frameBufferInput.renderpass = renderpass;
	frameBufferInput.swapchainExtent = swapchainExtent;
	vkInit::make_framebuffers(frameBufferInput, swapchainFrames, debugMode);
}
