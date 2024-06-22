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
	this->particles = new ParticleMenagerie();
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



	std::vector<const char*> particlefilenames = {"res/textures/fir.jpg","res/textures/gradient.png" };
	


	bindings.count = 2;
	bindings.types[0] = vk::DescriptorType::eCombinedImageSampler;
	bindings.types[1] = vk::DescriptorType::eCombinedImageSampler;
	particleTextureGraphicDescriptorPool = vkInit::make_descriptor_pool(device, static_cast<uint32_t>(particlefilenames.size()) + 1, bindings);


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
	
	for (ParticleEmiter* obj : scene->particleEmiters) {
		particles->consume();
	}
	particles->finalization(finalizationChunk);
	particles->make_descriptors_resources();
	vkImage::ParticleTextureInputChunk particleTextureInfo;
	particleTextureInfo.commandBuffer = maincommandBuffer;
	particleTextureInfo.queue = graphicsQueue;
	particleTextureInfo.logicalDevice = device;
	particleTextureInfo.physicalDevice = physicalDevice;
	particleTextureInfo.layout = particleTextureGraphicSetLayout;
	particleTextureInfo.descriptorPool = particleTextureGraphicDescriptorPool;
	particleTextureInfo.colorfilenames = particlefilenames[0];
	particleTextureInfo.gradientfilenames = particlefilenames[1];
	particleTexture = new vkImage::ParticleTexture(particleTextureInfo);




	bindings.count = 1;
	bindings.types[0] = vk::DescriptorType::eCombinedImageSampler;
	skyBoxTextureDescriptorPool = vkInit::make_descriptor_pool(device, static_cast<uint32_t>(filenames.size()) + 1, bindings);

	vkImage::skyBoxTextureInputChunk skyBoxTextureInput;
	skyBoxTextureInput.commandBuffer = maincommandBuffer;
	skyBoxTextureInput.queue = graphicsQueue;
	skyBoxTextureInput.logicalDevice = device;
	skyBoxTextureInput.physicalDevice = physicalDevice;
	skyBoxTextureInput.layout = skyBoxTextureSetLayout;
	skyBoxTextureInput.descriptorPool = skyBoxTextureDescriptorPool;
	skyBoxTextureInput.filenames = { {
			"tex/left.jpg",
			
			"tex/right.jpg",
			"tex/top.jpg",//x+
			"tex/bottom.jpg",
			   //y+
			 "tex/back.jpg",
			"tex/front.jpg",    //z-
	} };
	cubemap = new vkImage::Cubemap(skyBoxTextureInput);


	cubeMapMesh = new CubeMapMesh();
	vkMesh::FinalizationChunk cubemapfinalizationChunk;
	cubemapfinalizationChunk.physicalDevice = physicalDevice;
	cubemapfinalizationChunk.logicalDevice = device;
	cubemapfinalizationChunk.queue = graphicsQueue;
	cubemapfinalizationChunk.commandBuffer = maincommandBuffer;
	cubeMapMesh->finalize(cubemapfinalizationChunk);
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
	std::array<vk::Queue, 3> queues = vkInit::get_Queues(physicalDevice, device, surface, debugMode);
	
	this->graphicsQueue = queues[0];
	
	this->presentQueue = queues[1];

	this->computeQueue = queues[2];
	
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
	specification.postProcessImageInput = swapchainFrames[0].postProcessInputAttachment;

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

	vkInit::ParticleGraphicsPipelineInBundle particleInput;
	particleInput.computeFilePath = "shaders/particleCompute.spv";
	particleInput.vertexFilePath = "shaders/particleVert.spv";
	particleInput.fragmentFilePath = "shaders/particleFrag.spv";
	particleInput.depthFormat = swapchainFrames[0].depthFormat;
	particleInput.device = device;
	particleInput.swapchainExtent = swapchainExtent;
	particleInput.particleComputeDescriptorSetLayout = { particleComputeSetLayout };
	particleInput.particleAttachment = swapchainFrames[0].particleAttachment;
	particleInput.particleGraphicDescriptorSetLayout = {particleCameraGraphicSetLayout,particleTextureGraphicSetLayout};

	vkInit::ParticleGraphicsPipelineOutBundle particleOutput = vkInit::createParticlePipeline(particleInput,debugMode);
	particleGraphicPipeline = particleOutput.particleGrphicPipeline;
	particleGraphicsLayout = particleOutput.particlePipelineLayout;
	particleComputeLayout = particleOutput.particleComputePipelineLayout;
	particleComputePipeline = particleOutput.particleComputePipeline;
	particleRenderPass = particleOutput.particleRenderPass;



	vkInit::PostProcessPipelineInBundle postProcessPipelineInput;
	postProcessPipelineInput.device = device;
	postProcessPipelineInput.vertexFilePath = "shaders/postProcessCombinedImageVert.spv";
	postProcessPipelineInput.fragmentFilePath = "shaders/postProcessCombinedImageFrag.spv";
	postProcessPipelineInput.postProcessSetLayout = {postProcessDescriptorSetLayout};
	postProcessPipelineInput.swapchainExtent = swapchainExtent;
	postProcessPipelineInput.swapchainImageFormat = swapchainFormat;
	vkInit::PostProcessGraphicsPipelineOutBundle postProcessOutput = vkInit::create_postprocess_pipelines(postProcessPipelineInput,debugMode);
	postProcessRenderPass = postProcessOutput.renderpass;
    postProcessPipelineLayout = postProcessOutput.postProcessPipelineLayout;
	postProcessPipeline = postProcessOutput.postProcessgraphicsPipeline;




	vkInit::skyBoxPipelineInBundle skyBoxInput;
	skyBoxInput.device = device;
	skyBoxInput.vertexFilePath = "shaders/skyBoxVert.spv";
	skyBoxInput.fragmentFilePath = "shaders/skyBoxFrag.spv";
	skyBoxInput.swapchainExtent = swapchainExtent;
	skyBoxInput.swapchainImageFormat = swapchainFormat;
	skyBoxInput.skyBoxSetLayout = { skyBoxDescriptorSetLayout, skyBoxTextureSetLayout };
	skyBoxInput.depthFormat = swapchainFrames[0].depthFormat;
	skyBoxInput.Attachment = swapchainFrames[0].skyBoxAttachment;
	vkInit::skyBoxGraphicsPipelineOutBundle skyBoxOutput = vkInit::create_skybox_pipeline(skyBoxInput,debugMode);
	skyBoxPipelineLayout = skyBoxOutput.skyBoxPipelineLayout;
	skyBoxPipeline = skyBoxOutput.skyBoxgraphicsPipeline;
	skyBoxRenderPass = skyBoxOutput.renderpass;



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
	device.destroyCommandPool(computeCommandPool);
	device.destroyPipeline(particleGraphicPipeline);
	device.destroyPipeline(particleComputePipeline);
	device.destroyPipeline(graphicsPipeline);
	device.destroyPipeline(postProcessPipeline);
	device.destroyPipeline(skyBoxPipeline);
	device.destroyPipeline(deferedGraphicsPipeline);
	device.destroyPipeline(shadowPipeline);
	device.destroyPipelineLayout(layout);
	device.destroyPipelineLayout(deferedLayout);
	device.destroyPipelineLayout(shadowLayout);
	device.destroyPipelineLayout(particleComputeLayout);
	device.destroyPipelineLayout(particleGraphicsLayout);
	device.destroyPipelineLayout(postProcessPipelineLayout);
	device.destroyPipelineLayout(skyBoxPipelineLayout);
	device.destroyRenderPass(renderpass);
	device.destroyRenderPass(shadowRenderPass);
	device.destroyRenderPass(particleRenderPass);
	device.destroyRenderPass(postProcessRenderPass);
	device.destroyRenderPass(skyBoxRenderPass);
	
	this->cleanup_swapchain();
	
	device.destroyDescriptorSetLayout(frameSetLayout);
	device.destroyDescriptorSetLayout(shadowSetLayout);
	device.destroyDescriptorSetLayout(particleComputeSetLayout);
	device.destroyDescriptorSetLayout(meshSetLayout);
	device.destroyDescriptorSetLayout(particleCameraGraphicSetLayout);
	device.destroyDescriptorSetLayout(particleTextureGraphicSetLayout);
	device.destroyDescriptorSetLayout(postProcessDescriptorSetLayout);
	device.destroyDescriptorSetLayout(skyBoxTextureSetLayout);
	device.destroyDescriptorSetLayout(skyBoxDescriptorSetLayout);
	device.destroyDescriptorPool(meshDescriptorPool);
	device.destroyDescriptorPool(particleTextureGraphicDescriptorPool);
	device.destroyDescriptorPool(particleComputeDescriptorPool);
	device.destroyDescriptorPool(particleCameraGraphicDescriptorPool);
	device.destroyDescriptorPool(postProcessDescriptorPool);
	device.destroyDescriptorPool(skyBoxTextureDescriptorPool);
	device.destroyDescriptorPool(skyBoxDescriptorPool);
	
	device.destroyDescriptorPool(shadowDescriptorPool);
	
	delete meshes;
	delete particles;
	delete particleTexture;
	delete cubeMapMesh;
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

	bindings.count = 9;
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
	bindings.types.push_back(vk::DescriptorType::eStorageBuffer);
	bindings.counts.push_back(1);
	bindings.stages.push_back(vk::ShaderStageFlagBits::eFragment);
	bindings.indices.push_back(6);
	bindings.types.push_back(vk::DescriptorType::eUniformBuffer);
	bindings.counts.push_back(1);
	bindings.stages.push_back(vk::ShaderStageFlagBits::eFragment);
	bindings.indices.push_back(7);
	bindings.types.push_back(vk::DescriptorType::eCombinedImageSampler);
	bindings.counts.push_back(1);
	bindings.stages.push_back(vk::ShaderStageFlagBits::eFragment);
	bindings.indices.push_back(8);
	bindings.types.push_back(vk::DescriptorType::eInputAttachment);
	bindings.counts.push_back(1);
	bindings.stages.push_back(vk::ShaderStageFlagBits::eFragment);
	bindings.indices.push_back(9);
	deferedSetLayout = vkInit::make_descriptor_set_layout(device, bindings);

	bindings.count = 2;
	bindings.indices[0] = 0;
	bindings.types[0] = vk::DescriptorType::eStorageBuffer;
	bindings.stages[0] = vk::ShaderStageFlagBits::eVertex;
	bindings.indices[1] = 1;
	bindings.types[1] = vk::DescriptorType::eStorageBuffer;
	bindings.stages[1] = vk::ShaderStageFlagBits::eGeometry;

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

	bindings.count = 2;
	bindings.indices[0] = 0;
	bindings.indices[1] = 1;
	bindings.types[0] = vk::DescriptorType::eCombinedImageSampler;
	bindings.types[1] = vk::DescriptorType::eCombinedImageSampler;
	bindings.counts[0] = 1;
	bindings.counts[1] = 1;
	bindings.stages[0] = vk::ShaderStageFlagBits::eFragment;
	bindings.stages[1] = vk::ShaderStageFlagBits::eFragment;



	particleTextureGraphicSetLayout = vkInit::make_descriptor_set_layout(device, bindings);


	bindings.count = 2;
	bindings.indices[0] = 0;
	bindings.indices[1] = 1;
	bindings.types[0] = vk::DescriptorType::eUniformBuffer;
	bindings.types[1] = vk::DescriptorType::eStorageBuffer;
	bindings.counts[0] = 1;
	bindings.counts[1] = 1;
	bindings.stages[0] = vk::ShaderStageFlagBits::eCompute;
	bindings.stages[1] = vk::ShaderStageFlagBits::eCompute;



	particleComputeSetLayout = vkInit::make_descriptor_set_layout(device, bindings);
	


	bindings.count = 1;
	bindings.indices[0] = 0;
	bindings.types[0] = vk::DescriptorType::eUniformBuffer;
	bindings.counts[0] = 1;
	bindings.stages[0] = vk::ShaderStageFlagBits::eVertex;
	particleCameraGraphicSetLayout = vkInit::make_descriptor_set_layout(device, bindings);


	bindings.count = 3;
	bindings.indices[0] = 0;
	bindings.indices[1] = 1;

	bindings.types[0] = vk::DescriptorType::eCombinedImageSampler;
	bindings.types[1] = vk::DescriptorType::eCombinedImageSampler;
	bindings.types[2] = vk::DescriptorType::eCombinedImageSampler;
	bindings.counts[0] = 1;
	bindings.counts[1] = 1;
	bindings.counts[2] = 1;
	bindings.stages[0] = vk::ShaderStageFlagBits::eFragment;
	bindings.stages[1] = vk::ShaderStageFlagBits::eFragment;
	bindings.stages[2] = vk::ShaderStageFlagBits::eFragment;



	postProcessDescriptorSetLayout = vkInit::make_descriptor_set_layout(device, bindings);


	bindings.count = 1;
	bindings.indices[0] = 0;
	bindings.types[0] = vk::DescriptorType::eUniformBuffer;
	bindings.counts[0] = 1;
	bindings.stages[0] = vk::ShaderStageFlagBits::eVertex;
	skyBoxDescriptorSetLayout = vkInit::make_descriptor_set_layout(device, bindings);


	bindings.count = 1;
	bindings.indices[0] = 0;
	bindings.types[0] = vk::DescriptorType::eCombinedImageSampler;
	bindings.counts[0] = 1;
	bindings.stages[0] = vk::ShaderStageFlagBits::eFragment;
	skyBoxTextureSetLayout = vkInit::make_descriptor_set_layout(device, bindings);


}

void GraphicsEngine::finalize_setup()
{
	create_framebuffers();


	commandPool = vkInit::make_command_pool( physicalDevice, device, surface, debugMode);
	computeCommandPool = vkInit::make_compute_command_pool(physicalDevice, device, surface,debugMode);;
	vkInit::commandBufferInputChunk commandBufferInput = { device, commandPool,computeCommandPool ,swapchainFrames };
	vkInit::commandBufferOutput output = vkInit::make_command_buffer(commandBufferInput, debugMode);
	maincommandBuffer = output.graphicCommandBuffer;
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
	vkInit::commandBufferInputChunk commandBufferInput = { device, commandPool,computeCommandPool ,swapchainFrames };
	vkInit::make_frame_command_buffers(commandBufferInput, debugMode);
	
}

void GraphicsEngine::record_draw_commands(vk::CommandBuffer commandBuffer, vk::CommandBuffer particleCommandBuffer, vk::CommandBuffer skyboxCommandBuffer,uint32_t imageIndex)
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

	vk::ClearValue cC;
	std::array<float, 4> c = { 0.0f, 0.0f, 0.0f, 1.0f };
	cC.color = vk::ClearColorValue(c);
	vk::ClearValue dC;
	dC.depthStencil = vk::ClearDepthStencilValue({ 1.0f, 0 });
	std::vector<vk::ClearValue> cV = { {cC,dC} };
	vk::RenderPassBeginInfo particleGraphicRenderPass = {};
	particleGraphicRenderPass.renderPass = particleRenderPass;
	particleGraphicRenderPass.framebuffer = swapchainFrames[imageIndex].particleFramebuffer;
	particleGraphicRenderPass.renderArea.offset.x = 0;
	particleGraphicRenderPass.renderArea.offset.y = 0;
	particleGraphicRenderPass.renderArea.extent = swapchainExtent;
	particleGraphicRenderPass.clearValueCount = cV.size();
	particleGraphicRenderPass.pClearValues = cV.data();


	vk::BufferMemoryBarrier bufferBarrier = {  // sType                                  // pNext
	vk::AccessFlagBits::eMemoryRead,          // srcAccessMask
	vk::AccessFlagBits::eVertexAttributeRead,           // dstAccessMask
	vk::QueueFamilyIgnored,                   // srcQueueFamilyIndex
	vk::QueueFamilyIgnored,                   // dstQueueFamilyIndex
	particles->particleBuffer.buffer,                 // buffer
	0,                                         // offset
	particles->getBufferSize()
	};

	// Pipeline barrier to ensure the proper ordering of buffer accesses
	commandBuffer.pipelineBarrier(
		vk::PipelineStageFlagBits::eComputeShader,     // srcStageMask
		vk::PipelineStageFlagBits::eVertexInput, // dstStageMask
		vk::DependencyFlags(),                     // dependencyFlags
		nullptr,                                   // memoryBarriers
		bufferBarrier,                             // bufferBarriers
		nullptr                                    // imageBarriers
	);


	commandBuffer.beginRenderPass(&particleGraphicRenderPass,vk::SubpassContents::eSecondaryCommandBuffers);
	
	commandBuffer.executeCommands(particleCommandBuffer);
	commandBuffer.endRenderPass();


	vk::ClearValue cCc;
	std::array<float, 4> ccc = { 1.0f, 0.0f, 0.0f, 1.0f };
	cCc.color = vk::ClearColorValue(ccc);
	vk::ClearValue dCc;
	dCc.depthStencil = vk::ClearDepthStencilValue({ 1.0f, 0 });
	std::vector<vk::ClearValue> cVc = { {cCc,dCc} };
	vk::RenderPassBeginInfo skyboxrenderPassInfo = {};
	skyboxrenderPassInfo.renderPass = skyBoxRenderPass;
	skyboxrenderPassInfo.framebuffer = swapchainFrames[imageIndex].skyBoxFramebuffer;
	skyboxrenderPassInfo.renderArea.offset.x = 0;
	skyboxrenderPassInfo.renderArea.offset.y = 0;
	skyboxrenderPassInfo.renderArea.extent = swapchainExtent;
	skyboxrenderPassInfo.clearValueCount = cVc.size();
	skyboxrenderPassInfo.pClearValues = cVc.data();



	commandBuffer.beginRenderPass(&skyboxrenderPassInfo, vk::SubpassContents::eSecondaryCommandBuffers);
	commandBuffer.executeCommands(skyboxCommandBuffer);
	commandBuffer.endRenderPass();


	vk::BufferMemoryBarrier bufferBarrierparticle = {  // sType                                  // pNext
	vk::AccessFlagBits::eVertexAttributeRead,          // srcAccessMask
	vk::AccessFlagBits::eMemoryRead,           // dstAccessMask
	vk::QueueFamilyIgnored,                   // srcQueueFamilyIndex
	vk::QueueFamilyIgnored,                   // dstQueueFamilyIndex
	particles->particleBuffer.buffer,                 // buffer
	0,                                         // offset
	particles->getBufferSize()
	};
	// Pipeline barrier to ensure the proper ordering of buffer accesses
	commandBuffer.pipelineBarrier(
		vk::PipelineStageFlagBits::eVertexInput,     // srcStageMask
		vk::PipelineStageFlagBits::eComputeShader, // dstStageMask
		vk::DependencyFlags(),                     // dependencyFlags
		nullptr,                                   // memoryBarriers
		bufferBarrierparticle,                             // bufferBarriers
		nullptr                                    // imageBarriers
	);


	

	//commandBuffer.executeCommands(particleCommandBuffer);

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

	commandBuffer.beginRenderPass(&subrenderPassInfo,vk::SubpassContents::eInline);
	
	commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, shadowPipeline);
	commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, shadowLayout, 0, swapchainFrames[imageIndex].shadowDescriptorSet, nullptr);
	prepare_scene(commandBuffer);
	uint32_t startInstance = 0;
	//Triangles

	for (std::pair<meshTypes, std::vector<SceneObject*>> pair : models) {
		render_shadows_objects(commandBuffer, pair.first, startInstance, static_cast<uint32_t>(pair.second.size()));
	}


	
	
	
	commandBuffer.endRenderPass();




	

	

	vk::RenderPassBeginInfo renderPassInfo = {};
	renderPassInfo.renderPass = renderpass;
	renderPassInfo.framebuffer = swapchainFrames[imageIndex].framebuffer;
	renderPassInfo.renderArea.offset.x = 0;
	renderPassInfo.renderArea.offset.y = 0;
	renderPassInfo.renderArea.extent = swapchainExtent;

	vk::ClearValue colorClear;
	std::array<float, 4> colors = { 1.0f, 0.5f, 0.25f, 1.0f };
	std::array<float, 4> colorsd = { 0.0f, 0.0f, .0f, 1.0f };
	colorClear.color = vk::ClearColorValue(colors);
	vk::ClearValue depthClear;

	depthClear.depthStencil = vk::ClearDepthStencilValue({ 1.0f, 0 });
	std::vector<vk::ClearValue> clearValues = { {colorClear,vk::ClearColorValue(colorsd),vk::ClearColorValue(colorsd),vk::ClearColorValue(colorsd),vk::ClearColorValue(colorsd),vk::ClearColorValue(colorsd),vk::ClearColorValue(colorsd),depthClear}};

	renderPassInfo.clearValueCount = clearValues.size();
	renderPassInfo.pClearValues = clearValues.data();

	commandBuffer.beginRenderPass(&renderPassInfo, vk::SubpassContents::eInline);
	commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, graphicsPipeline);
	commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, layout, 0, swapchainFrames[imageIndex].descriptorSet, nullptr);
	prepare_scene(commandBuffer);
	startInstance = 0;
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




	

	vk::ImageMemoryBarrier particlebarrier;
	particlebarrier.oldLayout = vk::ImageLayout::eColorAttachmentOptimal;
	particlebarrier.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
	particlebarrier.srcAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
	particlebarrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;
	particlebarrier.image = swapchainFrames[imageIndex].particleAttachment.image; // Twoje vk::Image
	particlebarrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
	particlebarrier.subresourceRange.baseMipLevel = 0;
	particlebarrier.subresourceRange.levelCount = 1;
	particlebarrier.subresourceRange.baseArrayLayer = 0;
	particlebarrier.subresourceRange.layerCount = 1;

	commandBuffer.pipelineBarrier(
		vk::PipelineStageFlagBits::eColorAttachmentOutput, // source stage
		vk::PipelineStageFlagBits::eFragmentShader, // destination stage
		{}, // dependency flags
		nullptr, // memory barriers
		nullptr, // buffer memory barriers
		particlebarrier // image memory barriers
	);
	vk::ImageMemoryBarrier lightbarrier;
	lightbarrier.oldLayout = vk::ImageLayout::eColorAttachmentOptimal;
	lightbarrier.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
	lightbarrier.srcAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
	lightbarrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;
	lightbarrier.image = swapchainFrames[imageIndex].postProcessInputAttachment.image; // Twoje vk::Image
	lightbarrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
	lightbarrier.subresourceRange.baseMipLevel = 0;
	lightbarrier.subresourceRange.levelCount = 1;
	lightbarrier.subresourceRange.baseArrayLayer = 0;
	lightbarrier.subresourceRange.layerCount = 1;

	commandBuffer.pipelineBarrier(
		vk::PipelineStageFlagBits::eColorAttachmentOutput, // source stage
		vk::PipelineStageFlagBits::eFragmentShader, // destination stage
		{}, // dependency flags
		nullptr, // memory barriers
		nullptr, // buffer memory barriers
		lightbarrier // image memory barriers
	);


	vk::ImageMemoryBarrier skyboxbarrier;
	skyboxbarrier.oldLayout = vk::ImageLayout::eColorAttachmentOptimal;
	skyboxbarrier.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
	skyboxbarrier.srcAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
	skyboxbarrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;
	skyboxbarrier.image = swapchainFrames[imageIndex].skyBoxAttachment.image; // Twoje vk::Image
	skyboxbarrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
	skyboxbarrier.subresourceRange.baseMipLevel = 0;
	skyboxbarrier.subresourceRange.levelCount = 1;
	skyboxbarrier.subresourceRange.baseArrayLayer = 0;
	skyboxbarrier.subresourceRange.layerCount = 1;

	commandBuffer.pipelineBarrier(
		vk::PipelineStageFlagBits::eColorAttachmentOutput, // source stage
		vk::PipelineStageFlagBits::eFragmentShader, // destination stage
		{}, // dependency flags
		nullptr, // memory barriers
		nullptr, // buffer memory barriers
		skyboxbarrier // image memory barriers
	);

	vk::RenderPassBeginInfo skyBoxRenderpassInfo = {};
	skyBoxRenderpassInfo.renderPass = postProcessRenderPass;
	skyBoxRenderpassInfo.framebuffer = swapchainFrames[imageIndex].postProcessFramebuffer;
	skyBoxRenderpassInfo.renderArea.offset.x = 0;
	skyBoxRenderpassInfo.renderArea.offset.y = 0;
	skyBoxRenderpassInfo.renderArea.extent = swapchainExtent;

	

	std::vector<vk::ClearValue> PostProcessclearValues = { {colorClear} };

	skyBoxRenderpassInfo.clearValueCount = PostProcessclearValues.size();
	skyBoxRenderpassInfo.pClearValues = PostProcessclearValues.data();

	commandBuffer.beginRenderPass(&skyBoxRenderpassInfo, vk::SubpassContents::eInline);
	commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, postProcessPipeline);
	commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, postProcessPipelineLayout, 0, swapchainFrames[imageIndex].postProcessDescriptorSet, nullptr);


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
void GraphicsEngine::record_compute_commands(vk::CommandBuffer commandBuffer, uint32_t imageIndex)
{


	vk::CommandBufferBeginInfo beginInfo = {};

	try {
		commandBuffer.begin(beginInfo);
	}
	catch (vk::SystemError err) {
		if (debugMode) {
			std::cout << "Failed to begin recording compute command buffer!" << std::endl;
		}
	}
	
	vk::BufferMemoryBarrier bufferBarrier = {  // sType                                  // pNext
	vk::AccessFlagBits::eMemoryRead,          // srcAccessMask
	vk::AccessFlagBits::eShaderRead,           // dstAccessMask
	vk::QueueFamilyIgnored,                   // srcQueueFamilyIndex
	vk::QueueFamilyIgnored,                   // dstQueueFamilyIndex
	particles->particleBuffer.buffer,                 // buffer
	0,                                         // offset
	particles->getBufferSize()
	};
	
	// Pipeline barrier to ensure the proper ordering of buffer accesses
	commandBuffer.pipelineBarrier(
		vk::PipelineStageFlagBits::eVertexInput,     // srcStageMask
		vk::PipelineStageFlagBits::eComputeShader, // dstStageMask
		vk::DependencyFlags(),                     // dependencyFlags
		nullptr,                                   // memoryBarriers
		bufferBarrier,                             // bufferBarriers
		nullptr                                    // imageBarriers
	);
	
	
	// Dispatch the compute job
	commandBuffer.bindPipeline(vk::PipelineBindPoint::eCompute, particleComputePipeline);
	commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eCompute, particleComputeLayout, 0, swapchainFrames[imageIndex].particleDescriptorSet, nullptr);
	commandBuffer.dispatch(particles->burstParticleCount/256*particles->numberOfEmiter,1,1);

	// Add barrier to ensure that compute shader has finished writing to the buffer
	// Without this the (rendering) vertex shader may display incomplete results (partial data from last frame)
	

	
	vk::BufferMemoryBarrier bufferBarrier2 = {  // sType                                  // pNext
	vk::AccessFlagBits::eShaderRead,          // srcAccessMask
	vk::AccessFlagBits::eMemoryRead,           // dstAccessMask
	vk::QueueFamilyIgnored,                   // srcQueueFamilyIndex
	vk::QueueFamilyIgnored,                   // dstQueueFamilyIndex
	particles->particleBuffer.buffer,                 // buffer
	0,                                         // offset
	particles->getBufferSize()
	};

	// Pipeline barrier to ensure the proper ordering of buffer accesses
	commandBuffer.pipelineBarrier(
		vk::PipelineStageFlagBits::eComputeShader,     // srcStageMask
		vk::PipelineStageFlagBits::eVertexInput, // dstStageMask
		vk::DependencyFlags(),                     // dependencyFlags
		nullptr,                                   // memoryBarriers
		bufferBarrier2,                             // bufferBarriers
		nullptr                                    // imageBarriers
	);

	


	commandBuffer.end();
	//vkEndCommandBuffer(compute.commandBuffer);
}

void GraphicsEngine::record_particle_draw_commands(vk::CommandBuffer commandBuffer ,uint32_t imageIndex)
{

	vk::CommandBufferInheritanceInfo inh = {};
	inh.renderPass = particleRenderPass;
	inh.subpass = 0;
	inh.framebuffer = swapchainFrames[imageIndex].particleFramebuffer;


	vk::CommandBufferBeginInfo beginInfo;
	beginInfo.flags = vk::CommandBufferUsageFlagBits::eRenderPassContinue | vk::CommandBufferUsageFlagBits::eSimultaneousUse;
	beginInfo.pInheritanceInfo = &inh;
	try {
		commandBuffer.begin(beginInfo);
	}
	catch (vk::SystemError err) {
		if (debugMode) {
			std::cout << "Failed to begin recording shadow command buffer!" << std::endl;
		}
	}
	vk::DeviceSize offset[1] = { 0 };
	commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, particleGraphicPipeline);
	commandBuffer.bindVertexBuffers(0,1,&particles->particleBuffer.buffer, offset);
	commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, particleGraphicsLayout, 0, swapchainFrames[imageIndex].particleCameraDescriptorSet, nullptr);
	particleTexture->useTexture(commandBuffer, particleGraphicsLayout);
	commandBuffer.draw(particles->burstParticleCount * particles->numberOfEmiter,1,0,0);
	//commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, particleGraphicsLayout, 0,nullptr, nullptr);
	

	

	try {
		commandBuffer.end();
	}
	catch (vk::SystemError err) {

		if (debugMode) {
			std::cout << "failed to record command buffer!" << std::endl;
		}
	}
}

void GraphicsEngine::record_skybox_draw_commands(vk::CommandBuffer commandBuffer, uint32_t imageIndex)
{
	vk::CommandBufferInheritanceInfo inh = {};
	inh.renderPass = skyBoxRenderPass;
	inh.subpass = 0;
	inh.framebuffer = swapchainFrames[imageIndex].skyBoxFramebuffer;


	vk::CommandBufferBeginInfo beginInfo;
	beginInfo.flags = vk::CommandBufferUsageFlagBits::eRenderPassContinue | vk::CommandBufferUsageFlagBits::eSimultaneousUse;
	beginInfo.pInheritanceInfo = &inh;
	try {
		commandBuffer.begin(beginInfo);
	}
	catch (vk::SystemError err) {
		if (debugMode) {
			std::cout << "Failed to begin recording skybox command buffer!" << std::endl;
		}
	}
	vk::DeviceSize offset[1] = { 0 };
	commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, skyBoxPipeline);
	commandBuffer.bindVertexBuffers(0, 1, &cubeMapMesh->vertexBuffer.buffer, offset);
	commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, skyBoxPipelineLayout, 0, swapchainFrames[imageIndex].skyBoxDescriptorSet, nullptr);
	cubemap->use(commandBuffer, skyBoxPipelineLayout);
	commandBuffer.draw(36, 1, 0, 0);
	//commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, particleGraphicsLayout, 0,nullptr, nullptr);

	try {
		commandBuffer.end();
	}
	catch (vk::SystemError err) {

		if (debugMode) {
			std::cout << "failed to record skybox command buffer!" << std::endl;
		}
	}




}




void GraphicsEngine::record_shadow_draw_commands(vk::CommandBuffer commandBuffer, uint32_t imageIndex)
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

void GraphicsEngine::render(Scene *scene,int &verticesCounter,float deltaTime,Camera::Camera camera)
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
	vk::CommandBuffer computeParticleCommandBuffer = swapchainFrames[frameNumber].computeCommandBuffer;
	vk::CommandBuffer graphicParticleCommandBuffer = swapchainFrames[frameNumber].particleSeccondaryCommandBuffer;
	vk::CommandBuffer graphicSkyBoxCommandBuffer = swapchainFrames[frameNumber].skyBoxSeccondaryCommandBuffer;

	commandBuffer.reset();
	computeParticleCommandBuffer.reset();
	graphicParticleCommandBuffer.reset();
	graphicSkyBoxCommandBuffer.reset();
	
	prepare_frame(imageIndex, scene,deltaTime,camera);

	record_compute_commands(computeParticleCommandBuffer,imageIndex);
	record_skybox_draw_commands(graphicSkyBoxCommandBuffer,imageIndex);
	record_particle_draw_commands(graphicParticleCommandBuffer, imageIndex);
	record_draw_commands(commandBuffer,graphicParticleCommandBuffer, graphicSkyBoxCommandBuffer ,imageIndex);

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



	vk::PipelineStageFlags waitStageMask = vk::PipelineStageFlagBits::eComputeShader;
	vk::Semaphore waitSemaphores[] = { swapchainFrames[frameNumber].imageAvailable,swapchainFrames[frameNumber].computeFinished };
	// Submit compute commands
	vk::SubmitInfo computeSubmitInfo = {};
	computeSubmitInfo.commandBufferCount = 1;
	computeSubmitInfo.pCommandBuffers = &computeParticleCommandBuffer;
	computeSubmitInfo.waitSemaphoreCount = 1;
	computeSubmitInfo.pWaitSemaphores = &swapchainFrames[frameNumber].imageAvailable;
	computeSubmitInfo.pWaitDstStageMask = &waitStageMask;
	computeSubmitInfo.signalSemaphoreCount = 2;
	computeSubmitInfo.pSignalSemaphores = waitSemaphores;

	try {
		computeQueue.submit(computeSubmitInfo);
	}
	catch (vk::SystemError err) {

		if (debugMode) {
			std::cout << "failed to submit compute command buffer!" << std::endl;
		}
	}

	vk::SubmitInfo submitInfo = {};

	//vk::Semaphore waitSemaphores[] = { swapchainFrames[frameNumber].imageAvailable,swapchainFrames[frameNumber].computeFinished };
	vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput,vk::PipelineStageFlagBits::eComputeShader };
	submitInfo.waitSemaphoreCount = 2;
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
	gbindings.count = 9;
	gbindings.types.push_back(vk::DescriptorType::eInputAttachment); //pos
	gbindings.types.push_back(vk::DescriptorType::eInputAttachment); //normals
	gbindings.types.push_back(vk::DescriptorType::eInputAttachment); //albedo
	gbindings.types.push_back(vk::DescriptorType::eInputAttachment); //arm
	gbindings.types.push_back(vk::DescriptorType::eInputAttachment); //T
	gbindings.types.push_back(vk::DescriptorType::eStorageBuffer); //pointlight
	gbindings.types.push_back(vk::DescriptorType::eUniformBuffer); //camPos
	gbindings.types.push_back(vk::DescriptorType::eCombinedImageSampler); //camPos
	gbindings.types.push_back(vk::DescriptorType::eInputAttachment); //worldPos
	

	vkInit::descriptorSetLayoutData shadowBindings;
	shadowBindings.count = 2;
	shadowBindings.types.push_back(vk::DescriptorType::eStorageBuffer);
	shadowBindings.types.push_back(vk::DescriptorType::eStorageBuffer);

	vkInit::descriptorSetLayoutData CameraBindings;
	CameraBindings.count = 1;
	CameraBindings.types.push_back(vk::DescriptorType::eUniformBuffer);
	
	
	frameDescriptorPool = vkInit::make_descriptor_pool(device, static_cast<uint32_t>(swapchainFrames.size()), bindings);
	
	deferedDescriptorPool = vkInit::make_descriptor_pool(device, static_cast<uint32_t>(swapchainFrames.size()), gbindings);
	
	shadowDescriptorPool = vkInit::make_descriptor_pool(device, static_cast<uint32_t>(swapchainFrames.size()), shadowBindings);
	particleComputeDescriptorPool = vkInit::make_descriptor_pool(device, static_cast<uint32_t>(swapchainFrames.size()), bindings);
	particleCameraGraphicDescriptorPool = vkInit::make_descriptor_pool(device, static_cast<uint32_t>(swapchainFrames.size()), CameraBindings);

	vkInit::descriptorSetLayoutData skyBoxBindings;
	skyBoxBindings.count = 3;
	skyBoxBindings.types.push_back(vk::DescriptorType::eCombinedImageSampler);
	skyBoxBindings.types.push_back(vk::DescriptorType::eCombinedImageSampler);
	skyBoxBindings.types.push_back(vk::DescriptorType::eCombinedImageSampler);
	postProcessDescriptorPool = vkInit::make_descriptor_pool(device, static_cast<uint32_t>(swapchainFrames.size()), skyBoxBindings);
	skyBoxBindings.count = 1;
	skyBoxBindings.types.push_back(vk::DescriptorType::eUniformBuffer);
	skyBoxDescriptorPool = vkInit::make_descriptor_pool(device, static_cast<uint32_t>(swapchainFrames.size()), skyBoxBindings);
	
	//deferedDescriptorPool = vkInit::make_descriptor_pool(device, static_cast<uint32_t>(swapchainFrames.size()), gbindings);
	
	for (vkUtil::SwapChainFrame& frame : swapchainFrames) //referencja 
	{
		
		frame.imageAvailable = vkInit::make_semaphore(device, debugMode);
		frame.renderFinished = vkInit::make_semaphore(device, debugMode);
		frame.computeFinished = vkInit::make_semaphore(device, debugMode);
		frame.inFlight = vkInit::make_fence(device, debugMode);
		
		frame.make_descriptor_resources();
		
		frame.descriptorSet = vkInit::allocate_descriptor_set(device, frameDescriptorPool, frameSetLayout);
	
		frame.deferedDescriptorSet = vkInit::allocate_descriptor_set(device, deferedDescriptorPool, deferedSetLayout);
		
		frame.shadowDescriptorSet = vkInit::allocate_descriptor_set(device, shadowDescriptorPool, shadowSetLayout);
		
		frame.particleDescriptorSet = vkInit::allocate_descriptor_set(device, particleComputeDescriptorPool, particleComputeSetLayout);
		
		frame.particleCameraDescriptorSet = vkInit::allocate_descriptor_set(device, particleCameraGraphicDescriptorPool, particleCameraGraphicSetLayout);

		frame.postProcessDescriptorSet = vkInit::allocate_descriptor_set(device, postProcessDescriptorPool, postProcessDescriptorSetLayout);
		
		frame.skyBoxDescriptorSet = vkInit::allocate_descriptor_set(device, skyBoxDescriptorPool, skyBoxDescriptorSetLayout);
		
		

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
	
	frameBufferInput.renderpass = particleRenderPass;
	vkInit::make_particle_framebuffers(frameBufferInput, swapchainFrames, debugMode);
	frameBufferInput.renderpass = postProcessRenderPass;
	vkInit::make_postprocess_framebuffers(frameBufferInput, swapchainFrames, debugMode);
	frameBufferInput.renderpass = skyBoxRenderPass;
	vkInit::make_skybox_framebuffers(frameBufferInput, swapchainFrames, debugMode);
	
	
}



void GraphicsEngine::prepare_frame(uint32_t imageIndex, Scene* scene,float deltaTime,Camera::Camera camera)
{

	vkUtil::SwapChainFrame& _frame = swapchainFrames[imageIndex];

	glm::vec3 eye = { 0.0f, 0.0f, -1.0f };
	glm::vec3 center = { 0.0f, 0.0f, 0.0f };
	glm::vec3 up = { 0.0f, 1.0f, 0.0f };
	glm::mat4 view = glm::lookAt(eye, center, up);

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(swapchainExtent.width) / static_cast<float>(swapchainExtent.height), 0.1f, 1024.0f);
	projection[1][1] *= -1;

	_frame.cameraData.view = camera.GetViewMatrix();
	_frame.cameraData.projection = projection;
	_frame.cameraData.viewProjection = projection * camera.GetViewMatrix();
	_frame.cameraData.heightScale = glm::vec4( 0.0001f);
	_frame.camPos = glm::vec4(camera.Position, 1.0f);
	
	_frame.particleCameraUBOData.model = scene->particleEmiters[0]->transform.getModelMatrix();
	_frame.particleCameraUBOData.view = camera.GetViewMatrix();
	_frame.particleCameraUBOData.projection = projection;
	
	


	

	size_t i= 0;

	for(std::pair<meshTypes,std::vector<SceneObject*>> pair: models)
 {
		for (SceneObject* obj : pair.second) {
			
			_frame.modelTransforms[i++] = obj->getTransform().getModelMatrix();
		}
		

}

	
	
	
	glm::mat4 shadowProj = glm::perspective(glm::radians(120.0f), static_cast < float>(1)/ static_cast < float>(1), 0.1f,3.0f);
	std::vector<glm::mat4> shadowTransform;
	shadowProj[1][1] *= -1;

	size_t j = 0;
	for (Light* light : scene->lights) {
		
		//if(j==0)light->move();
		_frame.LightTransforms[j].mvp[5] = (shadowProj * glm::lookAt(light->transform.getGlobalPosition(), light->transform.getGlobalPosition() + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		_frame.LightTransforms[j].mvp[0] = (shadowProj * glm::lookAt(light->transform.getGlobalPosition(), light->transform.getGlobalPosition() + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		_frame.LightTransforms[j].mvp[1] = (shadowProj * glm::lookAt(light->transform.getGlobalPosition(), light->transform.getGlobalPosition() + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		_frame.LightTransforms[j].mvp[2] = (shadowProj * glm::lookAt(light->transform.getGlobalPosition(), light->transform.getGlobalPosition() + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
		_frame.LightTransforms[j].mvp[3] = (shadowProj * glm::lookAt(light->transform.getGlobalPosition(), light->transform.getGlobalPosition() + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
		_frame.LightTransforms[j].mvp[4] = (shadowProj * glm::lookAt(light->transform.getGlobalPosition(), light->transform.getGlobalPosition() + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		_frame.LightTransforms[j].position = glm::vec4(light->transform.getGlobalPosition(), 1.0f);
		_frame.LightTransforms[j++].diffuse = light->diffuse;
		
		//std::cout << light->transform.getGlobalPosition().x<< light->transform.getGlobalPosition().y<< light->transform.getGlobalPosition().z << std::endl;
	}
	
	_frame.particleUBOData.deltaT =  deltaTime;
	_frame.particleUBOData.particleCount = particles->burstParticleCount * particles->numberOfEmiter;
	
	_frame.skyboxData.view = camera.GetViewMatrix();
	_frame.skyboxData.projection = projection;
	//_frame.skyboxData.up = glm::vec4(glm::normalize(camera.Up),1.0f);
	
	memcpy(_frame.cameraDataWriteLocation, &(_frame.cameraData), sizeof(vkUtil::UBO));
	memcpy(_frame.particleCameraUBOWriteLoacation, &(_frame.particleCameraUBOData), sizeof(vkUtil::UBOCameraParticle));
	memcpy(_frame.camPosWriteLoacation, &(_frame.camPos), sizeof(glm::vec4));
	memcpy(_frame.particleUBOWriteLoacation, &(_frame.particleUBOData), sizeof(vkUtil::particleUBO));
	memcpy(_frame.skyboxUBOWriteLoacation, &(_frame.skyboxData), sizeof(vkUtil::SkyBoxUBO));

	memcpy(_frame.lightDataWriteLocation, (_frame.LightTransforms.data()), j * sizeof(vkUtil::PointLight));
	memcpy(_frame.modelBufferWriteLocation, _frame.modelTransforms.data(), i * sizeof(glm::mat4));
	
	_frame.write_descriptor_set();
	_frame.writeGbufferDescriptor(_frame.deferedDescriptorSet, device);
	_frame.shadowDescripotrsWrite();
	_frame.writeParticleDescriptor(particles->particleBufferDescriptor);
	_frame.write_skybox_descriptor();
}
