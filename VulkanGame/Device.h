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
		std::vector<vk::PresentModeKHR> presendModes; // wybiera jak dobieraæ obrazy
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
}

