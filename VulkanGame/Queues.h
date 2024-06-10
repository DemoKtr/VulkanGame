#pragma once
#include "config.h"
#include <optional>

namespace vkUtil {
	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily; //moze nie musi zawierac warttosci .has_value sprawdza
		std::optional<uint32_t> presentFamily;
		std::optional<uint32_t> computeFamily;
		bool isComplete() {
			return (graphicsFamily.has_value() && presentFamily.has_value() && computeFamily.has_value());
		}
	};

	QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice device, vk::SurfaceKHR surface, bool debugMode);
}

