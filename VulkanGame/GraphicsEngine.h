#pragma once
#include "config.h"
#include <GLFW/glfw3.h>



class GraphicsEngine
{
	ivec2 screenSize{ 640, 480 };
	bool debugMode = { true };
	GLFWwindow* mainWindow{ nullptr };
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
	std::vector<vk::Image> swapchainImages;
	vk::Format swapchainFormat;
	vk::Extent2D swapchainExtent;

	void build_glfw_window(); //glfw Setup
	void make_instance(); //instance Setup
	void make_debug_messenger();
	void choice_device();
public:
	GraphicsEngine();
	~GraphicsEngine();
};

