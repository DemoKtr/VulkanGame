#pragma once
#include "config.h"
#include <GLFW/glfw3.h>
#include "Frame.h"
#include "Transform.h"
#include "VertexMenagerie.h"
#include "Scene.h"
#include "Image.h"
#include "Texture.h"
#include "Cubemap.h"
#include "ParticleMenagerie.h"
#include "ParticleTexture.h"
#include "Camera.h"
#include "CubeMapMesh.h"
#include "NoiseTexture.h"
#include "PBBloom.h"

class GraphicsEngine
{
	ivec2 screenSize;
	int verticesonScene = {0};
	bool debugMode;;
	GLFWwindow* mainWindow;
	const char* appName{ "VulkanGame" };
	//Vulkan Instance
	vk::Instance instance{ nullptr };
	//debug callback
	vk::DebugUtilsMessengerEXT debugMessenger{ nullptr };
	//dynamic instance dispatcher
	vk::DispatchLoaderDynamic dldi;
	vk::SurfaceKHR surface;

	//physical device
	vk::PhysicalDevice physicalDevice{ nullptr };
	vk::Device device {nullptr};
	vk::Queue graphicsQueue{ nullptr };
	vk::Queue presentQueue{ nullptr };
	vk::Queue computeQueue{ nullptr };

	//swapchain
	vk::SwapchainKHR swapchain;
	std::vector<vkUtil::SwapChainFrame> swapchainFrames;
	vk::Format swapchainFormat;
	vk::Extent2D swapchainExtent;

	//pipeline
	vk::PipelineLayout layout;
	vk::PipelineLayout deferedLayout;
	vk::RenderPass renderpass;
	vk::Pipeline graphicsPipeline;
	vk::Pipeline deferedGraphicsPipeline;

	//shadows
	vk::Pipeline shadowPipeline;
	vk::RenderPass shadowRenderPass;
	vk::PipelineLayout shadowLayout;


	//Particles
	vk::Pipeline particleGraphicPipeline;
	vk::PipelineLayout   particleGraphicsLayout;
	vk::Pipeline particleComputePipeline;
	vk::PipelineLayout   particleComputeLayout;
	vk::RenderPass particleRenderPass;
	//command
	vk::CommandPool commandPool;
	vk::CommandBuffer maincommandBuffer;
	
	//compute
	vk::CommandPool computeCommandPool;
	
	///

	vk::DescriptorSetLayout downScaleDescriptorSetLayout;
	vk::DescriptorPool downScaleDescriptorPool;
	vk::DescriptorSetLayout upScaleDescriptorSetLayout;
	vk::DescriptorPool upScaleDescriptorPool;
	////
	//postProcess
	vk::Pipeline postProcessPipeline;
	vk::RenderPass postProcessRenderPass;
	vk::PipelineLayout postProcessPipelineLayout;

	//skyBox
	vk::Pipeline skyBoxPipeline;
	vk::RenderPass skyBoxRenderPass;
	vk::PipelineLayout skyBoxPipelineLayout;



	vk::DescriptorSetLayout finalDescriptorSetLayout;
	vk::DescriptorPool finalDescriptorPool;
	vk::Pipeline finalPipeline;
	vk::RenderPass finalRenderPass;
	vk::PipelineLayout finalPipelineLayou;

	//synchronizers 
	int maxFramesInFlight, frameNumber;

	VertexMenagerie* meshes;
	VertexMenagerie* animatedMeshes;
	ParticleMenagerie* particles;

	std::unordered_map<meshTypes, vkImage::Texture*>  materials;
	std::unordered_map<animatedModelTypes, vkImage::Texture*>  animatedMaterials;
	vkImage::ParticleTexture*  particleTexture;
	std::unordered_map<meshTypes, int> instanceCounter;
	std::unordered_map<animatedModelTypes, int> animatedinstanceCounter;
	std::unordered_map<meshTypes, std::vector<SceneObject*>> models;
	std::unordered_map<animatedModelTypes, std::vector<AnimatedSceneObjects*>> animatiedModels;
	
	//Descriptor objects

	vk::DescriptorSetLayout frameSetLayout;
	vk::DescriptorPool frameDescriptorPool;
	vk::DescriptorSetLayout meshSetLayout;
	vk::DescriptorPool meshDescriptorPool;

	vk::DescriptorSetLayout deferedSetLayout;
	vk::DescriptorPool deferedDescriptorPool;

	vk::DescriptorSetLayout particleComputeSetLayout;
	vk::DescriptorPool particleComputeDescriptorPool;


	///Particle Graphic
	vk::DescriptorSetLayout particleTextureGraphicSetLayout;
	vk::DescriptorPool particleTextureGraphicDescriptorPool;

	///Particle Graphic
	vk::DescriptorSetLayout particleCameraGraphicSetLayout;
	vk::DescriptorPool particleCameraGraphicDescriptorPool;


	vk::DescriptorSetLayout shadowSetLayout;
	vk::DescriptorPool shadowDescriptorPool;


	vk::DescriptorSetLayout postProcessDescriptorSetLayout;
	vk::DescriptorPool postProcessDescriptorPool;


	vk::DescriptorSetLayout skyBoxDescriptorSetLayout;
	vk::DescriptorPool skyBoxDescriptorPool;

	vk::DescriptorSetLayout skyBoxTextureSetLayout;
	vk::DescriptorPool skyBoxTextureDescriptorPool;

	vk::DescriptorSetLayout ssaoDescriptorSetLayout;
	vk::DescriptorPool ssaoDescriptorPool;


	vk::DescriptorSetLayout blurDescriptorSetLayout;
	vk::DescriptorPool blurDescriptorPool;

	vk::DescriptorSetLayout animationDescriptorSetLayout;
	vk::DescriptorPool animationDescriptorPool;

	vk::DescriptorSetLayout animationTextureDescriptorSetLayout;
	vk::DescriptorPool animationTextureDescriptorPool;



	CubeMapMesh* cubeMapMesh;

	vkImage::Cubemap* cubemap;

	vkImage::NoiseTexture* noiseTexture;

	vkBloom::PBBloom* bloom;

	void make_assets(Scene* scene);
	void prepare_scene(vk::CommandBuffer commandBuffer);
	void prepare_animated_scene(vk::CommandBuffer commandBuffer);
	void prepare_frame(uint32_t imageIndex, Scene* scene, float deltaTime, Camera::Camera camera);


	//Scene
	Transform transform;
	float ang{ 0 };

	void make_instance(); //instance Setup
	void make_debug_messenger();
	void choice_device();
	void create_swapchain();
	void recreate_swapchain(Scene* scene);
	void create_descriptor_set_layouts();
	void create_pipeline();
	void finalize_setup(Scene* scene);
	void create_frame_resources(Scene* scene);
	void create_framebuffers();
	void cleanup_swapchain();

	void record_draw_commands(vk::CommandBuffer commandBuffer, vk::CommandBuffer particleCommandBuffer,vk::CommandBuffer skyboxCommandBuffer,uint32_t imageIndex);
	void record_compute_commands(vk::CommandBuffer commandBuffer,uint32_t imageIndex);
	void record_particle_draw_commands(vk::CommandBuffer commandBuffer,uint32_t imageIndex);
	void record_skybox_draw_commands(vk::CommandBuffer commandBuffer,uint32_t imageIndex);
	void record_shadow_draw_commands(vk::CommandBuffer commandBuffer, uint32_t imageIndex);

	float ourLerp(float a, float b, float f);
	void render_objects(vk::CommandBuffer commandBuffer, meshTypes objectType, uint32_t& starInstance, uint32_t instanceCount);
	void render_shadows_objects(vk::CommandBuffer commandBuffer, meshTypes objectType, uint32_t& starInstance, uint32_t instanceCount);
public:
	GraphicsEngine(ivec2 screenSize, GLFWwindow* window,Scene* scene ,bool debugMode);
	void render(Scene *scene,int &verticesCounter,float deltaTime,Camera::Camera camera);
	~GraphicsEngine();
};

