#pragma once
#include "config.h"
namespace vkInit {
	VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData
	);

	/**
		Make a debug messenger

		\param instance The Vulkan instance which will be debugged.
		\param dldi dynamically loads instance based dispatch functions
		\returns the created messenger
	*/
	vk::DebugUtilsMessengerEXT make_debug_messenger(vk::Instance& instance, vk::DispatchLoaderDynamic& dldi);
	void log_device_properties(const vk::PhysicalDevice& device);

	std::vector<std::string> log_transform_bits(vk::SurfaceTransformFlagsKHR bits);
	std::vector<std::string> log_alpha_composite_bits(vk::CompositeAlphaFlagsKHR bits);
	std::vector<std::string> log_image_usage_bits(vk::ImageUsageFlags bits);
	std::string log_present_mode(vk::PresentModeKHR presentMode);
		



}

