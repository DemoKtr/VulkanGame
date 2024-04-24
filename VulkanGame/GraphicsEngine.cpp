#include "GraphicsEngine.h"
#include "Instance.h"
#include "Logging.h"
#include "Device.h"
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
	this->swapchainImages = bundle.images;
	this->swapchainFormat = bundle.format;
	this->swapchainExtent = bundle.extent;
	//vkInit::query_swapchain_support(physicalDevice, surface, debugMode);
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
}

GraphicsEngine::~GraphicsEngine()
{

	if (debugMode) {
		std::cout << "End!\n";
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
