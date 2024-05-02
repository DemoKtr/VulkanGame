#pragma once
#include "config.h"
#include <optional>

namespace vkUtil {
	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily; //moze nie musi zawierac warttosci .has_value sprawdza
		std::optional<uint32_t> presentFamily;
		bool isComplete() {
			return (graphicsFamily.has_value() && presentFamily.has_value());
		}
	};

	QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice device, vk::SurfaceKHR surface, bool debugMode);
}

