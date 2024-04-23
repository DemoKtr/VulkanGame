#pragma once
#include <GLFW/glfw3.h>
#include "config.h"


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

	//physical device
	vk::PhysicalDevice physicalDevice{ nullptr };

	void build_glfw_window(); //glfw Setup
	void make_instance(); //instance Setup
	void make_debug_messenger();
	void choice_device();
public:
	GraphicsEngine();
	~GraphicsEngine();
};

