#include "GraphicsEngine.h"
#include "Instance.h"
#include "Logging.h"
#include "Device.h"
#include "Pipeline.h"
#include "Swapchain.h"
#include "FrameBuffer.h"
#include "Commands.h"
#include "Synchronizer.h"
#include "Descrpitors.h"
#include "Obj_Mesh.h"
#include "Gbuffer.h"
#include <set>

void GraphicsEngine::make_assets(Scene* scene)
{
	meshes = new VertexMenagerie();

	std::unordered_map<meshTypes, std::array<char*,2>> model_filenames = {};// { { meshTypes::KITTY, { "box.obj","box.mtl" } }, { meshTypes::DOG, {"box.obj","box.mtl"} } };

	
	for (SceneObject* obj : scene->sceneObjects) {
		if (model_filenames.find(obj->objMaterial.meshType) == model_filenames.end()) {
			// Jeœli klucz nie istnieje, dodajemy do mapy
			model_filenames[obj->objMaterial.meshType][0] = obj->objMaterial.model;
			model_filenames[obj->objMaterial.meshType][1] = obj->objMaterial.material;
			instanceCounter[obj->objMaterial.meshType]++;
			models[obj->objMaterial.meshType].push_back(obj);
		}
		else { instanceCounter[obj->objMaterial.meshType]++; models[obj->objMaterial.meshType].push_back(obj);
		}
	}

	for (std::pair<meshTypes, std::array<char*,2>> pair : model_filenames) {
		vkMesh::ObjMesh model(pair.second[0], pair.second[1], glm::mat4(1.0f));
		meshes->consume(pair.first, model.vertices, model.indices);
		verticesonScene += model.vertices.size()/14;
	}



	FinalizationChunk finalizationChunk;
	finalizationChunk.logicalDevice = device;
	finalizationChunk.physicalDevice = physicalDevice;
	finalizationChunk.queue =graphicsQueue;
	finalizationChunk.commandBuffer = maincommandBuffer;
	meshes->finalize(finalizationChunk);

	//Materials
	std::unordered_map<meshTypes, std::array<char*,4>> filenames = {
	};
	for (SceneObject* obj : scene->sceneObjects) {
		if(filenames.find(obj->objMaterial.meshType) == filenames.end())
		{
			filenames[obj->objMaterial.meshType][0] = obj->objMaterial.diffuse;
			filenames[obj->objMaterial.meshType][1] = obj->objMaterial.normalMap;
			filenames[obj->objMaterial.meshType][2] = obj->objMaterial.armMap;
			filenames[obj->objMaterial.meshType][3] = obj->objMaterial.depthMap;
		}
	}
	
	
	//make Descriptor pool
	vkInit::descriptorSetLayoutData bindings;
	bindings.count = 4;
	bindings.types.push_back(vk::DescriptorType::eCombinedImageSampler);
	bindings.types.push_back(vk::DescriptorType::eCombinedImageSampler);
	bindings.types.push_back(vk::DescriptorType::eCombinedImageSampler);
	bindings.types.push_back(vk::DescriptorType::eCombinedImageSampler);
	meshDescriptorPool = vkInit::make_descriptor_pool(device, static_cast<uint32_t>(filenames.size()) + 1, bindings);


	vkImage::TextureInputChunk textureInfo;

	textureInfo.commandBuffer = maincommandBuffer;
	textureInfo.queue = graphicsQueue;
	textureInfo.logicalDevice = device;
	textureInfo.physicalDevice = physicalDevice;
	textureInfo.layout = meshSetLayout;
	textureInfo.descriptorPool = meshDescriptorPool;
	for (const auto& [obj, filename] : filenames) {
		textureInfo.diffusefilenames = filename[0];
		textureInfo.normalfilenames= filename[1];
		textureInfo.armfilenames = filename[2];
		textureInfo.depthfilenames = filename[3];
		materials[obj] = new vkImage::Texture(textureInfo);
	}
	/*
	textureInfo.layout = meshSetLayout;
	textureInfo.descriptorPool = meshDescriptorPool;
	textureInfo.filenames = { {
			"tex/front.bmp",  //x+
			"tex/back.bmp",   //x-
			"tex/left.bmp",   //y+
			"tex/right.bmp",  //y-
			"tex/bottom.bmp", //z+
			"tex/top.bmp",    //z-
	} };
	cubemap = new vkImage::Cubemap(textureInfo);
	*/
}

void GraphicsEngine::prepare_scene(vk::CommandBuffer commandBuffer)
{
	vk::Buffer vertexBuffers[] = { meshes->vertexBuffer.buffer };
	vk::DeviceSize offets[] = { 0 };
	commandBuffer.bindVertexBuffers(0, 1, vertexBuffers, offets);
	commandBuffer.bindIndexBuffer(meshes->indexBuffer.buffer,0,vk::IndexType::eUint32);
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
	
	this->create_swapchain();
	
	frameNumber = 0;
}
void GraphicsEngine::create_pipeline()
{
	vkInit::GraphicsPipelineInBundle specification = {};
	specification.device = device;
	specification.vertexFilePath = "shaders/geoVert.spv";
	specification.fragmentFilePath = "shaders/geoFrag.spv";
	specification.deferedVertexFilePath = "shaders/deferedVert.spv";
	specification.deferedFragmentFilePath = "shaders/deferedFrag.spv";
	specification.swapchainExtent = swapchainExtent;
	specification.swapchainImageFormat = swapchainFormat;
	specification.depthFormat = swapchainFrames[0].depthFormat;
	specification.geometryDescriptorSetLayouts = { frameSetLayout,meshSetLayout };
	specification.deferedDescriptorSetLayouts = { deferedSetLayout};
	specification.gbuffer = swapchainFrames[0].gbuffer;

	vkInit::GraphicsPipelineOutBundle output = vkInit::create_defered_pipelines(specification,debugMode);
	layout = output.layout;
	renderpass = output.renderpass;
	graphicsPipeline = output.graphicsPipeline;
	deferedGraphicsPipeline = output.deferedGraphicsPipeline;
	deferedLayout = output.deferedLayout;

	vkInit::ShadowGraphicsPipelineInBundle input = {};
	input.device = device;
	input.vertexFilePath = "shaders/ShadowMapVert.spv";
	input.geometryFilePath = "shaders/ShadowMapGeo.spv";
	input.swapchainExtent = swapchainExtent;
	input.depthFormat = swapchainFrames[0].depthFormat;
	input.shadowDescriptorSetLayout = { shadowSetLayout };
	input.shadowAttachmentBuffer = swapchainFrames[0].shadowMapBuffer;
	vkInit::ShadowGraphicsPipelineOutBundle out = vkInit::createShadowsPipeline(input,debugMode);
	shadowRenderPass = out.shadowRenderPass;
	shadowPipeline = out.shadowPipeline;
	shadowLayout = out.shadowPipelineLayout;
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

	for (vkUtil::SwapChainFrame& frame : swapchainFrames) {
		frame.logicalDevice = device;
		frame.physicalDevice = physicalDevice;
		frame.width = swapchainExtent.width;
		frame.height = swapchainExtent.height;
		frame.make_depth_resources();
	}
}
////////////////////////////////////
GraphicsEngine::GraphicsEngine(ivec2 screenSize, GLFWwindow* window,Scene* scene ,bool debugMode)
{
	this->screenSize = screenSize;
	this->mainWindow = window;
	this->debugMode = debugMode;
	if (debugMode) {
		std::cout << "Making a graphics engine\n";
	}

	
	make_instance();
	choice_device();

	create_descriptor_set_layouts();

	create_pipeline();
	finalize_setup();
	make_assets(scene);

}



GraphicsEngine::~GraphicsEngine()
{
	device.waitIdle();
	if (debugMode) {
		std::cout << "End!\n";
	}
	
	device.destroyCommandPool(commandPool);
	device.destroyPipeline(graphicsPipeline);
	device.destroyPipeline(deferedGraphicsPipeline);
	device.destroyPipeline(shadowPipeline);
	device.destroyPipelineLayout(layout);
	device.destroyPipelineLayout(deferedLayout);
	device.destroyPipelineLayout(shadowLayout);
	device.destroyRenderPass(renderpass);
	device.destroyRenderPass(shadowRenderPass);
	
	this->cleanup_swapchain();
	
	device.destroyDescriptorSetLayout(frameSetLayout);
	device.destroyDescriptorSetLayout(shadowSetLayout);
	device.destroyDescriptorSetLayout(meshSetLayout);
	device.destroyDescriptorPool(meshDescriptorPool);
	device.destroyDescriptorPool(shadowDescriptorPool);
	
	delete meshes;
	for (const auto& [key, texture] : materials) delete texture;
	for (const auto& [key, SceneObjects] : models) {
		for (SceneObject* obj : SceneObjects) delete obj;
	}
	delete cubemap;
	
	device.destroyDescriptorSetLayout(deferedSetLayout);
	device.destroyDescriptorPool(deferedDescriptorPool);
	
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
	for (vkUtil::SwapChainFrame& frame : swapchainFrames) {
		frame.destroy();

	}
	device.destroySwapchainKHR(swapchain);
	device.destroyDescriptorPool(frameDescriptorPool);
}

void GraphicsEngine::create_descriptor_set_layouts()
{
	vkInit::descriptorSetLayoutData bindings;
	bindings.count = 2;
	bindings.indices.push_back(0);
	bindings.types.push_back(vk::DescriptorType::eUniformBuffer);
	bindings.counts.push_back(1);
	bindings.stages.push_back(vk::ShaderStageFlagBits::eVertex);

	bindings.indices.push_back(1);
	bindings.types.push_back(vk::DescriptorType::eStorageBuffer);
	bindings.counts.push_back(1);
	bindings.stages.push_back(vk::ShaderStageFlagBits::eVertex);

	

	frameSetLayout = vkInit::make_descriptor_set_layout(device, bindings);

	bindings.count = 6;
	bindings.indices[0] = 0;
	bindings.indices[1] = 1;
	bindings.types[0] = vk::DescriptorType::eInputAttachment;
	bindings.types[1] = vk::DescriptorType::eInputAttachment;
	bindings.counts[0] = 1;
	bindings.counts[1] = 1;
	bindings.stages[0] = vk::ShaderStageFlagBits::eFragment;
	bindings.stages[1] = vk::ShaderStageFlagBits::eFragment;
	
	bindings.indices.push_back(2);
	bindings.types.push_back(vk::DescriptorType::eInputAttachment);
	bindings.counts.push_back(1);
	bindings.stages.push_back(vk::ShaderStageFlagBits::eFragment);

	bindings.indices.push_back(3);
	bindings.types.push_back(vk::DescriptorType::eInputAttachment);
	bindings.counts.push_back(1);
	bindings.stages.push_back(vk::ShaderStageFlagBits::eFragment);

	bindings.indices.push_back(4);
	bindings.types.push_back(vk::DescriptorType::eInputAttachment);
	bindings.counts.push_back(1);
	bindings.stages.push_back(vk::ShaderStageFlagBits::eFragment);

	bindings.indices.push_back(5);
	bindings.types.push_back(vk::DescriptorType::eUniformBuffer);
	bindings.counts.push_back(1);
	bindings.stages.push_back(vk::ShaderStageFlagBits::eFragment);

	deferedSetLayout = vkInit::make_descriptor_set_layout(device, bindings);

	bindings.count = 1;
	bindings.indices[0] = 0;
	bindings.types[0] = vk::DescriptorType::eUniformBuffer;
	bindings.stages[0] = vk::ShaderStageFlagBits::eGeometry;

	shadowSetLayout = vkInit::make_descriptor_set_layout(device, bindings);


	bindings.count = 4;
	bindings.indices[0] = 0;
	bindings.indices[1] = 1;
	bindings.indices[2] = 2;
	bindings.indices[3] = 3;
	bindings.types[0] = vk::DescriptorType::eCombinedImageSampler;
	bindings.types[1] = vk::DescriptorType::eCombinedImageSampler;
	bindings.types[2] = vk::DescriptorType::eCombinedImageSampler;
	bindings.types[3] = vk::DescriptorType::eCombinedImageSampler;
	bindings.counts[0] = 1;
	bindings.counts[1] = 1;
	bindings.counts[2] = 1;
	bindings.counts[3] = 1;
	bindings.stages[0] = vk::ShaderStageFlagBits::eFragment;
	bindings.stages[1] = vk::ShaderStageFlagBits::eFragment;
	bindings.stages[2] = vk::ShaderStageFlagBits::eFragment;
	bindings.stages[3] = vk::ShaderStageFlagBits::eFragment;


	meshSetLayout = vkInit::make_descriptor_set_layout(device, bindings);




}

void GraphicsEngine::finalize_setup()
{
	create_framebuffers();


	commandPool = vkInit::make_command_pool( physicalDevice, device, surface, debugMode);

	vkInit::commandBufferInputChunk commandBufferInput = { device, commandPool, swapchainFrames };
	maincommandBuffer = vkInit::make_command_buffer(commandBufferInput, debugMode);
	vkInit::make_frame_command_buffers(commandBufferInput, debugMode);

	create_frame_resources();
	

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
	create_frame_resources();
	vkInit::commandBufferInputChunk commandBufferInput = { device, commandPool, swapchainFrames };
	vkInit::make_frame_command_buffers(commandBufferInput, debugMode);
}

void GraphicsEngine::record_draw_commands(vk::CommandBuffer commandBuffer,vk::CommandBuffer shadowCommandBuffer ,uint32_t imageIndex,Scene* scene)
{
	vk::CommandBufferBeginInfo beginInfo = {};
	
	try {
		commandBuffer.begin(beginInfo);
	}
	catch (vk::SystemError err) {
		if (debugMode) {
			std::cout << "Failed to begin recording command buffer!" << std::endl;
		}
	}

	vk::ClearValue dClear;
	dClear.depthStencil = vk::ClearDepthStencilValue({ 1.0f, 0 });
	std::vector<vk::ClearValue> dclearValues = { dClear };
	vk::RenderPassBeginInfo subrenderPassInfo = {};
	subrenderPassInfo.renderPass = shadowRenderPass;
	subrenderPassInfo.framebuffer = swapchainFrames[imageIndex].shadowFramebuffer;
	subrenderPassInfo.renderArea.offset.x = 0;
	subrenderPassInfo.renderArea.offset.y = 0;
	subrenderPassInfo.renderArea.extent.height = 1024;
	subrenderPassInfo.renderArea.extent.width = 1024;
	subrenderPassInfo.clearValueCount = dclearValues.size();
	subrenderPassInfo.pClearValues = dclearValues.data();

	commandBuffer.beginRenderPass(&subrenderPassInfo,vk::SubpassContents::eSecondaryCommandBuffers);
	commandBuffer.executeCommands(shadowCommandBuffer);
	commandBuffer.endRenderPass();
	
	/*
	vk::ImageMemoryBarrier barrier = {};
	barrier.srcAccessMask = vk::AccessFlagBits::eDepthStencilAttachmentWrite;
	barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;
	barrier.oldLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
	barrier.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = swapchainFrames[frameNumber].shadowMapBuffer.shadowBufferDepthAttachment.image;
	barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eDepth;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 6;
	

	commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eLateFragmentTests, vk::PipelineStageFlagBits::eFragmentShader, vk::DependencyFlags(), nullptr, nullptr, barrier);
	*/
	vk::RenderPassBeginInfo renderPassInfo = {};
	renderPassInfo.renderPass = renderpass;
	renderPassInfo.framebuffer = swapchainFrames[imageIndex].framebuffer;
	renderPassInfo.renderArea.offset.x = 0;
	renderPassInfo.renderArea.offset.y = 0;
	renderPassInfo.renderArea.extent = swapchainExtent;

	vk::ClearValue colorClear;
	std::array<float, 4> colors = { 1.0f, 0.5f, 0.25f, 1.0f };
	std::array<float, 4> colorsd = { 0.0f, 0.0f, .0f, 0.0f };
	colorClear.color = vk::ClearColorValue(colors);
	vk::ClearValue depthClear;

	depthClear.depthStencil = vk::ClearDepthStencilValue({ 1.0f, 0 });
	std::vector<vk::ClearValue> clearValues = { {colorClear,vk::ClearColorValue(colorsd),vk::ClearColorValue(colorsd),vk::ClearColorValue(colorsd),vk::ClearColorValue(colorsd),vk::ClearColorValue(colorsd),depthClear}};

	renderPassInfo.clearValueCount = clearValues.size();
	renderPassInfo.pClearValues = clearValues.data();

	commandBuffer.beginRenderPass(&renderPassInfo, vk::SubpassContents::eInline);
	commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, graphicsPipeline);
	commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, layout, 0, swapchainFrames[imageIndex].descriptorSet, nullptr);
	prepare_scene(commandBuffer);
	uint32_t startInstance = 0;
	//Triangles
	
	for (std::pair<meshTypes, std::vector<SceneObject*>> pair : models) {
		render_objects(commandBuffer,pair.first, startInstance, static_cast<uint32_t>(pair.second.size()));
	}
	
	// W tym miejscu bym chcia³ postawiæ bariere 
	commandBuffer.nextSubpass(vk::SubpassContents::eInline);

	commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, deferedGraphicsPipeline);

	commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, deferedLayout, 0, swapchainFrames[imageIndex].deferedDescriptorSet, nullptr);
	

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
void GraphicsEngine::record_shadow_draw_commands(vk::CommandBuffer commandBuffer, uint32_t imageIndex, Scene* scene)
{
	vk::CommandBufferInheritanceInfo inh = {};
	inh.renderPass = shadowRenderPass;
	inh.subpass = 0;
	inh.framebuffer = swapchainFrames[imageIndex].shadowFramebuffer;


	vk::CommandBufferBeginInfo beginInfo;
	beginInfo.flags = vk::CommandBufferUsageFlagBits::eRenderPassContinue;
	beginInfo.pInheritanceInfo = &inh;
	try {
		commandBuffer.begin(beginInfo);
	}
	catch (vk::SystemError err) {
		if (debugMode) {
			std::cout << "Failed to begin recording shadow command buffer!" << std::endl;
		}
	}
	commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, shadowPipeline);
	commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, shadowLayout, 0, swapchainFrames[imageIndex].shadowDescriptorSet, nullptr);
	prepare_scene(commandBuffer);
	uint32_t startInstance = 0;
	//Triangles
	
	for (std::pair<meshTypes, std::vector<SceneObject*>> pair : models) {
		render_shadows_objects(commandBuffer, pair.first, startInstance, static_cast<uint32_t>(pair.second.size()));
	}


	try {
		commandBuffer.end();
	}
	catch (vk::SystemError err) {

		if (debugMode) {
			std::cout << "failed to record command buffer!" << std::endl;
		}
	}

}
void GraphicsEngine::render_objects(vk::CommandBuffer commandBuffer, meshTypes objectType, uint32_t& startInstance, uint32_t instanceCount) {
	//Triangles
	int indexCount = meshes->indexCounts.find(objectType)->second;
	int firstIndex = meshes->firstIndices.find(objectType)->second;
	materials[objectType]->useTexture(commandBuffer, layout);
	
	commandBuffer.drawIndexed(indexCount, instanceCount, firstIndex,0 ,startInstance);
	startInstance += instanceCount;
}

void GraphicsEngine::render_shadows_objects(vk::CommandBuffer commandBuffer, meshTypes objectType, uint32_t& starInstance, uint32_t instanceCount)
{
	//Triangles
	int indexCount = meshes->indexCounts.find(objectType)->second;
	int firstIndex = meshes->firstIndices.find(objectType)->second;

	commandBuffer.drawIndexed(indexCount, instanceCount, firstIndex, 0, starInstance);
	starInstance += instanceCount;
}

void GraphicsEngine::render(Scene *scene,int &verticesCounter)
{
	verticesCounter = verticesonScene;
	
	device.waitForFences(1, &swapchainFrames[frameNumber].inFlight, VK_TRUE, UINT64_MAX);
	device.resetFences(1, &swapchainFrames[frameNumber].inFlight);
	
	//acquireNextImageKHR(vk::SwapChainKHR, timeout, semaphore_to_signal, fence)
	uint32_t imageIndex;
	try {
		vk::ResultValue acquire = device.acquireNextImageKHR(
			swapchain, UINT64_MAX,
			swapchainFrames[frameNumber].imageAvailable, nullptr
		);
		imageIndex = acquire.value;
	}
	catch (vk::OutOfDateKHRError error) {
		std::cout << "Recreate" << std::endl;
		recreate_swapchain();
		return;
	}
	catch (vk::IncompatibleDisplayKHRError error) {
		std::cout << "Recreate" << std::endl;
		recreate_swapchain();
		return;
	}
	catch (vk::SystemError error) {
		std::cout << "Failed to acquire swapchain image!" << std::endl;
	}

	
	
	vk::CommandBuffer commandBuffer = swapchainFrames[frameNumber].commandBuffer;
	vk::CommandBuffer shadowCommandBuffer = swapchainFrames[frameNumber].shadowCommandBuffer;

	commandBuffer.reset();
	shadowCommandBuffer.reset();
	
	prepare_frame(imageIndex, scene);

	record_shadow_draw_commands(shadowCommandBuffer, imageIndex, scene);
	record_draw_commands(commandBuffer,shadowCommandBuffer ,imageIndex,scene);

	/*

	// Set up pipeline barrier to synchronize between render passes
	vk::ImageMemoryBarrier barrier = {};
	barrier.srcAccessMask = vk::AccessFlagBits::eDepthStencilAttachmentWrite;
	barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;
	barrier.oldLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
	barrier.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = swapchainFrames[frameNumber].shadowMapBuffer.shadowBufferDepthAttachment.image;
	barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eDepth;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 6;
	
	commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eLateFragmentTests, vk::PipelineStageFlagBits::eFragmentShader, vk::DependencyFlags(), nullptr, nullptr, barrier);
	*/

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

	vk::Result present;

	try {
		present = presentQueue.presentKHR(presentInfo);
	}
	catch (vk::OutOfDateKHRError error) {
		present = vk::Result::eErrorOutOfDateKHR;
	}

	if (present == vk::Result::eErrorOutOfDateKHR || present == vk::Result::eSuboptimalKHR) {
		std::cout << "Recreate" << std::endl;
		recreate_swapchain();
		return;
	}

	frameNumber = (frameNumber + 1) % maxFramesInFlight;
}

void GraphicsEngine::create_frame_resources()
{
	vkInit::descriptorSetLayoutData bindings;
	bindings.count = 2;
	bindings.types.push_back(vk::DescriptorType::eUniformBuffer);
	bindings.types.push_back(vk::DescriptorType::eStorageBuffer);
	
	

	vkInit::descriptorSetLayoutData gbindings;
	gbindings.count = 6;
	gbindings.types.push_back(vk::DescriptorType::eInputAttachment); //pos
	gbindings.types.push_back(vk::DescriptorType::eInputAttachment); //normals
	gbindings.types.push_back(vk::DescriptorType::eInputAttachment); //albedo
	gbindings.types.push_back(vk::DescriptorType::eInputAttachment); //arm
	gbindings.types.push_back(vk::DescriptorType::eInputAttachment); //T
	gbindings.types.push_back(vk::DescriptorType::eUniformBuffer); //pointlight
	

	vkInit::descriptorSetLayoutData shadowBindings;
	shadowBindings.count = 1;
	shadowBindings.types.push_back(vk::DescriptorType::eUniformBuffer);
	frameDescriptorPool = vkInit::make_descriptor_pool(device, static_cast<uint32_t>(swapchainFrames.size()), bindings);
	deferedDescriptorPool = vkInit::make_descriptor_pool(device, static_cast<uint32_t>(swapchainFrames.size()), gbindings);
	shadowDescriptorPool = vkInit::make_descriptor_pool(device, static_cast<uint32_t>(swapchainFrames.size()), shadowBindings);
	//deferedDescriptorPool = vkInit::make_descriptor_pool(device, static_cast<uint32_t>(swapchainFrames.size()), gbindings);
	
	for (vkUtil::SwapChainFrame& frame : swapchainFrames) //referencja 
	{
		frame.imageAvailable = vkInit::make_semaphore(device, debugMode);
		frame.renderFinished = vkInit::make_semaphore(device, debugMode);
		frame.inFlight = vkInit::make_fence(device, debugMode);

		frame.make_descriptor_resources();

		frame.descriptorSet = vkInit::allocate_descriptor_set(device, frameDescriptorPool, frameSetLayout);
		frame.deferedDescriptorSet = vkInit::allocate_descriptor_set(device, deferedDescriptorPool, deferedSetLayout);
		frame.shadowDescriptorSet = vkInit::allocate_descriptor_set(device, shadowDescriptorPool, shadowSetLayout);
		

	}
	

}

void GraphicsEngine::create_framebuffers()
{
	vkInit::framebufferInput frameBufferInput;
	frameBufferInput.device = device;
	frameBufferInput.renderpass = renderpass;
	frameBufferInput.swapchainExtent = swapchainExtent;
	vkInit::make_framebuffers_withGbuffer(frameBufferInput, swapchainFrames, debugMode);
	frameBufferInput.renderpass = shadowRenderPass;
	vkInit::make_shadow_framebuffers(frameBufferInput, swapchainFrames, debugMode);
}


void GraphicsEngine::prepare_frame(uint32_t imageIndex, Scene* scene)
{

	vkUtil::SwapChainFrame& _frame = swapchainFrames[imageIndex];

	glm::vec3 eye = { 0.0f, 0.0f, -1.0f };
	glm::vec3 center = { 0.0f, 0.0f, 0.0f };
	glm::vec3 up = { 0.0f, 1.0f, 0.0f };
	glm::mat4 view = glm::lookAt(eye, center, up);

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(swapchainExtent.width) / static_cast<float>(swapchainExtent.height), 0.1f, 10.0f);
	projection[1][1] *= -1;

	_frame.cameraData.view = view;
	_frame.cameraData.projection = projection;
	_frame.cameraData.viewProjection = projection * view;
	
	

	glm::vec3 position(0.0f,2.0f,0.0f);

	float constant= 0.7f;
	float linear = 0.09f;
	float quadratic = 0.032;

	glm::vec3 ambient =glm::vec3(0.05f);
	glm::vec3 diffuse = glm::vec3(0.8f);
	glm::vec3 specular = glm::vec3(1.0f);



	

	size_t i= 0;

	for(std::pair<meshTypes,std::vector<SceneObject*>> pair: models)
 {
		for (SceneObject* obj : pair.second) {
			obj->getTransform().rotate(glm::vec3(1, 1, 1), -0.0001f);
			obj->getTransform().computeModelMatrix();
			_frame.shadowData.modelPos[i] = glm::vec4(obj->getTransform().getGlobalPosition(), 1.0f);
			_frame.modelTransforms[i++] = obj->getTransform().getModelMatrix();
		}
		

}

	
	memcpy(_frame.modelBufferWriteLocation, _frame.modelTransforms.data(), i * sizeof(glm::mat4));
	size_t j = 0;
	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), 1024.0f / 1024.0f, 1.0f, 25.0f);
	std::vector<glm::mat4> shadowTransform;
	


	for (Light* light : scene->lights) {
		_frame.shadowData.mvp[j][0] = (shadowProj * glm::lookAt(light->transform.getGlobalPosition(), light->transform.getGlobalPosition() + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		_frame.shadowData.mvp[j][1] = (shadowProj * glm::lookAt(light->transform.getGlobalPosition(), light->transform.getGlobalPosition() + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		_frame.shadowData.mvp[j][2] = (shadowProj * glm::lookAt(light->transform.getGlobalPosition(), light->transform.getGlobalPosition() + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
		_frame.shadowData.mvp[j][3] = (shadowProj * glm::lookAt(light->transform.getGlobalPosition(), light->transform.getGlobalPosition() + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
		_frame.shadowData.mvp[j][4] = (shadowProj * glm::lookAt(light->transform.getGlobalPosition(), light->transform.getGlobalPosition() + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		_frame.shadowData.mvp[j++][5] = (shadowProj * glm::lookAt(light->transform.getGlobalPosition(), light->transform.getGlobalPosition() + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		
		//std::cout << light->transform.getGlobalPosition().x<< light->transform.getGlobalPosition().y<< light->transform.getGlobalPosition().z << std::endl;
	}
	_frame.cameraData.heightScale = 0.01f;
	
	_frame.lightData.position[0] = glm::vec3(0.0f, 255.5f, 0.0f);
	_frame.lightData.position[1] = glm::vec3(0.0f, -.5f, 0.0f);
	_frame.lightData.diffuse[0] = glm::vec3(0.0f,0.0f,0.0f);
	_frame.lightData.diffuse[1] = glm::vec3();
	_frame.lightData.camPos = eye;
	_frame.cameraData.camPos = eye;
	
	memcpy(_frame.cameraDataWriteLocation, &(_frame.cameraData), sizeof(vkUtil::UBO));
	memcpy(_frame.lightDataWriteLocation, &(_frame.lightData), sizeof(vkUtil::PointLight));
	memcpy(_frame.shadowDataWriteLocation, &(_frame.shadowData), i * sizeof(vkUtil::ShadowUBO));

	
	
	_frame.write_descriptor_set();
	_frame.writeGbufferDescriptor(_frame.deferedDescriptorSet, device);
	_frame.shadowDescripotrsWrite();
	//vkGbuffer::writeGbufferDescriptor(_frame.deferedDescriptorSet,device,_frame.gbuffer);
}
