#include "GraphicsEngine.h"
#include "Instance.h"
#include "Logging.h"
#include "Device.h"
#include "Pipeline.h"
#include "Swapchain.h"
#include "FrameBuffer.h"

void GraphicsEngine::build_glfw_window()
{
	//initialize glfw
	glfwInit();

	//no default rendering client, we'll hook vulkan up
	//to the window later
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	//resizing breaks the swapchain, we'll disable it for now
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	//GLFWwindow* glfwCreateWindow (int width, int height, const char *title, GLFWmonitor *monitor, GLFWwindow *share)
	if (mainWindow = glfwCreateWindow(screenSize.x, screenSize.y, this->appName, nullptr, nullptr)) {
		if (debugMode) {
			std::cout << "Successfully made a glfw window , width: " << screenSize.x << ", height: " << screenSize.y << '\n';
		}
	}
	else {
		if (debugMode) {
			std::cout << "GLFW window creation failed\n";
		}
	}
}
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
	vkInit::SwapChainBundle bundle = vkInit::create_swapchain(physicalDevice, device, surface, screenSize, debugMode);
	this->swapchainFrames = bundle.frames;
	this->swapchainFormat = bundle.format;
	this->swapchainExtent = bundle.extent;
	//vkInit::query_swapchain_support(physicalDevice, surface, debugMode);
}
void GraphicsEngine::create_pipeline()
{
	vkInit::GraphicsPipelineInBundle specification = {};
	specification.device = device;
	specification.vertexFilePath = "shaders/shader.vert.spv";
	specification.fragmentFilePath = "shaders/shader.frag.spv";
	specification.swapchainExtent = swapchainExtent;
	specification.swapchainImageFormat = swapchainFormat;

	vkInit::GraphicsPipelineOutBundle output = vkInit::create_graphic_pipeline(specification,debugMode);
	layout = output.layout;
	renderpass = output.renderpass;
	graphicsPipeline = output.graphicsPipeline;

}
////////////////////////////////////
GraphicsEngine::GraphicsEngine()
{
	if (debugMode) {
		std::cout << "Making a graphics engine\n";
	}

	build_glfw_window();
	make_instance();
	choice_device();
	create_pipeline();
	finalize_setup();
}

GraphicsEngine::~GraphicsEngine()
{

	if (debugMode) {
		std::cout << "End!\n";
	}
	device.destroyPipeline(graphicsPipeline);
	
	device.destroyRenderPass(renderpass);
	device.destroyPipelineLayout(layout);
	for (vkUtil::SwapChainFrame frame : swapchainFrames) {
		device.destroyImageView(frame.imageView);
	}
	device.destroySwapchainKHR(swapchain);
	device.destroy();

	instance.destroySurfaceKHR(surface);

	if (debugMode) {
		instance.destroyDebugUtilsMessengerEXT(debugMessenger, nullptr, dldi);
	}
	
	instance.destroy();
	glfwTerminate();

}

void GraphicsEngine::finalize_setup()
{
	vkInit::framebufferInput frameBufferInput;
	frameBufferInput.device = device;
	frameBufferInput.renderpass = renderpass;
	frameBufferInput.swapchainExtent = swapchainExtent;
	vkInit::make_framebuffers(frameBufferInput, swapchainFrames, debugMode);

	
}

void GraphicsEngine::record_draw_commands(vk::CommandBuffer commandBuffer, uint32_t imageIndex)
{
}
