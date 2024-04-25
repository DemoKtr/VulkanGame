#pragma once
#include "config.h"

namespace vkInit
{

	struct SwapChainSupportDetails {
		vk::SurfaceCapabilitiesKHR capabilities; ///ile obrazow w swapchain
		std::vector<vk::SurfaceFormatKHR> formats; //pixelformat
		std::vector<vk::PresentModeKHR> presentModes; // wybiera jak dobiera� obrazy
	};
	struct SwapChainFrame {
		vk::Image image;
		vk::ImageView imageView;
	};
	struct SwapChainBundle {
		vk::SwapchainKHR swapchain;
		std::vector<SwapChainFrame> frames;
		vk::Format format;
		vk::Extent2D extent;
	};

	

	SwapChainSupportDetails query_swapchain_support(vk::PhysicalDevice device, vk::SurfaceKHR surface, bool debugMode);
	vk::SurfaceFormatKHR choose_swapchain_surface_format(std::vector<vk::SurfaceFormatKHR> formats);
	vk::PresentModeKHR choose_swapchain_present_mode(std::vector<vk::PresentModeKHR> presentModes);
	vk::Extent2D choose_swapchain_exten(uint32_t width, uint32_t height, vk::SurfaceCapabilitiesKHR capabilities);
	SwapChainBundle create_swapchain(vk::PhysicalDevice physicalDevice, vk::Device logicalDevice, vk::SurfaceKHR surface, ivec2 screenSize, bool debugMode);

};
