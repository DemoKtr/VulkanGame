#pragma once
#include "config.h"
#include "Frame.h"
namespace vkInit
{
	struct SwapChainSupportDetails {
		vk::SurfaceCapabilitiesKHR capabilities; ///ile obrazow w swapchain
		std::vector<vk::SurfaceFormatKHR> formats; //pixelformat
		std::vector<vk::PresentModeKHR> presentModes; // wybiera jak dobieraæ obrazy
	};
	
	struct SwapChainBundle {
		vk::SwapchainKHR swapchain;
		std::vector<vkUtil::SwapChainFrame> frames;
		vk::Format format;
		vk::Extent2D extent;
	};

	

	SwapChainSupportDetails query_swapchain_support(vk::PhysicalDevice device, vk::SurfaceKHR surface, bool debugMode);
	vk::SurfaceFormatKHR choose_swapchain_surface_format(std::vector<vk::SurfaceFormatKHR> formats);
	vk::PresentModeKHR choose_swapchain_present_mode(std::vector<vk::PresentModeKHR> presentModes);
	vk::Extent2D choose_swapchain_exten(uint32_t width, uint32_t height, vk::SurfaceCapabilitiesKHR capabilities);
	SwapChainBundle create_swapchain(vk::PhysicalDevice physicalDevice, vk::Device logicalDevice, vk::SurfaceKHR surface, ivec2 screenSize, bool debugMode);

};

