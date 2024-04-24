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


	vk::PhysicalDevice	choose_physical_device(vk::Instance& instance,bool debugMode);
	bool isSuitable(const vk::PhysicalDevice& device, bool debugMode);
	bool checkDeviceExtensionSupport(
		const vk::PhysicalDevice& device,
		const std::vector<const char*>& requestedExtensions,
		const bool debug
	);

	//QueueFamily
	QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice device, bool debugMode);
}

