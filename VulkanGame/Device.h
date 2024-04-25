#pragma once
#include "config.h"

namespace vkInit {




	vk::PhysicalDevice	choose_physical_device(vk::Instance& instance,bool debugMode);
	bool isSuitable(const vk::PhysicalDevice& device, bool debugMode);
	bool checkDeviceExtensionSupport(
		const vk::PhysicalDevice& device,
		const std::vector<const char*>& requestedExtensions,
		const bool debug
	);

	//QueueFamily
	

	vk::Device create_logical_device(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface, bool debugMode);
	std::array<vk::Queue, 2> get_Queues(vk::PhysicalDevice physicalDevice, vk::Device device,vk::SurfaceKHR surface, bool debugMode);
	
	
}

