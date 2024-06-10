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

	//command
	vk::CommandPool commandPool;
	vk::CommandBuffer maincommandBuffer;
	
	//compute
	vk::CommandPool computeCommandPool;
	vk::CommandBuffer computeCommandBuffer;

	//synchronizers 
	int maxFramesInFlight, frameNumber;

	VertexMenagerie* meshes;
	std::unordered_map<meshTypes, vkImage::Texture*>  materials;
	std::unordered_map<meshTypes, int> instanceCounter;
	std::unordered_map<meshTypes, std::vector<SceneObject*>> models;
	
	//Descriptor objects

	vk::DescriptorSetLayout frameSetLayout;
	vk::DescriptorPool frameDescriptorPool;
	vk::DescriptorSetLayout meshSetLayout;
	vk::DescriptorPool meshDescriptorPool;

	vk::DescriptorSetLayout deferedSetLayout;
	vk::DescriptorPool deferedDescriptorPool;


	vk::DescriptorSetLayout shadowSetLayout;
	vk::DescriptorPool shadowDescriptorPool;

	//vk::DescriptorSetLayout deferedSetLayout;
	//vk::DescriptorPool deferedDescriptorPool;
	vkImage::Cubemap* cubemap;

	void make_assets(Scene* scene);
	void prepare_scene(vk::CommandBuffer commandBuffer);
	void prepare_frame(uint32_t imageIndex, Scene* scene);


	//Scene
	Transform transform;
	float ang{ 0 };

	void make_instance(); //instance Setup
	void make_debug_messenger();
	void choice_device();
	void create_swapchain();
	void recreate_swapchain();
	void create_descriptor_set_layouts();
	void create_pipeline();
	void finalize_setup();
	void create_frame_resources();
	void create_framebuffers();
	void cleanup_swapchain();

	void record_draw_commands(vk::CommandBuffer commandBuffer,uint32_t imageIndex,Scene *scene);
	void record_shadow_draw_commands(vk::CommandBuffer commandBuffer, uint32_t imageIndex,Scene *scene);


	void render_objects(vk::CommandBuffer commandBuffer, meshTypes objectType, uint32_t& starInstance, uint32_t instanceCount);
	void render_shadows_objects(vk::CommandBuffer commandBuffer, meshTypes objectType, uint32_t& starInstance, uint32_t instanceCount);
public:
	GraphicsEngine(ivec2 screenSize, GLFWwindow* window,Scene* scene ,bool debugMode);
	void render(Scene *scene,int &verticesCounter);
	~GraphicsEngine();
};

