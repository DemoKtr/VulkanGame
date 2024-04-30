#pragma once
#include "config.h"
#include <GLFW/glfw3.h>
#include "Frame.h"
#include "Transform.h"
#include "VertexMenagerie.h"


class GraphicsEngine
{
	ivec2 screenSize;
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
	vk::Device device{ nullptr };
	vk::Queue graphicsQueue{ nullptr };
	vk::Queue presentQueue{ nullptr };

	//swapchain
	vk::SwapchainKHR swapchain;
	std::vector<vkUtil::SwapChainFrame> swapchainFrames;
	vk::Format swapchainFormat;
	vk::Extent2D swapchainExtent;

	//pipeline
	vk::PipelineLayout layout;
	vk::RenderPass renderpass;
	vk::Pipeline graphicsPipeline;

	//command
	vk::CommandPool commandPool;
	vk::CommandBuffer maincommandBuffer;
	
	//synchronizers 
	int maxFramesInFlight, frameNumber;

	VertexMenagerie* meshes;

	//Descriptor objects

	vk::DescriptorSetLayout descriptorSetLayout;
	vk::DescriptorPool descriptorPool;



	void make_assets();
	void prepare_scene(vk::CommandBuffer commandBuffer);
	void prepare_frame(uint32_t imageIndex);


	//Scene
	Transform transform;
	float ang{ 0 };

	void make_instance(); //instance Setup
	void make_debug_messenger();
	void choice_device();
	void create_swapchain();
	void recreate_swapchain();
	void create_descriptor_set_layout();
	void create_pipeline();
	void finalize_setup();
	void create_frame_resources();
	void create_framebuffers();
	void cleanup_swapchain();

	void record_draw_commands(vk::CommandBuffer commandBuffer, uint32_t imageIndex);
public:
	GraphicsEngine(ivec2 screenSize, GLFWwindow* window, bool debugMode);
	void render();
	~GraphicsEngine();
};

