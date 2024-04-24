#pragma once
#include "config.h"

namespace vkInit {

	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily; //moze nie musi zawierac warttosci .has_value sprawdza
		std::optional<uint32_t> presentFamily;
		bool isComplete() {
			return (graphicsFamily.has_value() && presentFamily.has_value());
		}
	};

	struct SwapChainSupportDetails {
		vk::SurfaceCapabilitiesKHR capabilities; ///ile obrazow w swapchain
		std::vector<vk::SurfaceFormatKHR> formats; //pixelformat
		std::vector<vk::PresentModeKHR> presentModes; // wybiera jak dobieraæ obrazy
	};

	struct SwapChainBundle {
		vk::SwapchainKHR swapchain;
		std::vector<vk::Image> images;
		vk::Format format;
		vk::Extent2D extent;
	};

	vk::PhysicalDevice	choose_physical_device(vk::Instance& instance,bool debugMode);
	bool isSuitable(const vk::PhysicalDevice& device, bool debugMode);
	bool checkDeviceExtensionSupport(
		const vk::PhysicalDevice& device,
		const std::vector<const char*>& requestedExtensions,
		const bool debug
	);

	//QueueFamily
	QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice device, vk::SurfaceKHR surface,bool debugMode);

	vk::Device create_logical_device(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface, bool debugMode);
	std::array<vk::Queue, 2> get_Queues(vk::PhysicalDevice physicalDevice, vk::Device device,vk::SurfaceKHR surface, bool debugMode);
	
	SwapChainSupportDetails query_swapchain_support(vk::PhysicalDevice device, vk::SurfaceKHR surface, bool debugMode);
	vk::SurfaceFormatKHR choose_swapchain_surface_format(std::vector<vk::SurfaceFormatKHR> formats);
	vk::PresentModeKHR choose_swapchain_present_mode(std::vector<vk::PresentModeKHR> presentModes);
	vk::Extent2D choose_swapchain_exten(uint32_t width, uint32_t height, vk::SurfaceCapabilitiesKHR capabilities);
	SwapChainBundle create_swapchain(vk::PhysicalDevice physicalDevice, vk::Device logicalDevice, vk::SurfaceKHR surface, ivec2 screenSize, bool debugMode);
}

