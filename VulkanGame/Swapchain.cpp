#include "Swapchain.h"
#include "Logging.h"
#include "Queues.h"
#include "Image.h"
#include "Gbuffer.h"
vkInit::SwapChainSupportDetails vkInit::query_swapchain_support(vk::PhysicalDevice device, vk::SurfaceKHR surface, bool debugMode)
{
	SwapChainSupportDetails support;
	/*
* typedef struct VkSurfaceCapabilitiesKHR {
	uint32_t                         minImageCount;
	uint32_t                         maxImageCount;
	VkExtent2D                       currentExtent;
	VkExtent2D                       minImageExtent;
	VkExtent2D                       maxImageExtent;
	uint32_t                         maxImageArrayLayers;
	VkSurfaceTransformFlagsKHR       supportedTransforms;
	VkSurfaceTransformFlagBitsKHR    currentTransform;
	VkCompositeAlphaFlagsKHR         supportedCompositeAlpha;
	VkImageUsageFlags                supportedUsageFlags;
} VkSurfaceCapabilitiesKHR;
*/
	support.capabilities = device.getSurfaceCapabilitiesKHR(surface);
	if (debugMode) {
		std::cout << "Swapchain can support the following surface capabilities:" << std::endl;

		std::cout << "tminimum image count: " << support.capabilities.minImageCount << std::endl;
		std::cout << "tmaximum image count: " << support.capabilities.maxImageCount << std::endl;

		std::cout << "\tcurrent extent: \n";
		/*typedef struct VkExtent2D {
			uint32_t    width;
			uint32_t    height;
		} VkExtent2D;
		*/
		std::cout << "\t\twidth: " << support.capabilities.currentExtent.width << std::endl;
		std::cout << "\t\theight: " << support.capabilities.currentExtent.height << std::endl;

		std::cout << "\tminimum supported extent: " << std::endl;
		std::cout << "\t\twidth: " << support.capabilities.minImageExtent.width << std::endl;
		std::cout << "\t\theight: " << support.capabilities.minImageExtent.height << std::endl;

		std::cout << "\tmaximum supported extent: " << std::endl;
		std::cout << "\t\twidth: " << support.capabilities.maxImageExtent.width << std::endl;
		std::cout << "\t\theight: " << support.capabilities.maxImageExtent.height << std::endl;

		std::cout << "\tmaximum image array layers: " << support.capabilities.maxImageArrayLayers << std::endl;

		std::cout << "\tsupported transforms: " << std::endl;;
		std::vector<std::string> stringList = log_transform_bits(support.capabilities.supportedTransforms);
		for (std::string line : stringList) {
			std::cout << "\t\t" << line << std::endl;;
		}

		std::cout << "\tcurrent transform:\n";
		stringList = log_transform_bits(support.capabilities.currentTransform);
		for (std::string line : stringList) {
			std::cout << "\t\t" << line << std::endl;
		}

		std::cout << "\tsupported alpha operations:" << std::endl;;
		stringList = log_alpha_composite_bits(support.capabilities.supportedCompositeAlpha);
		for (std::string line : stringList) {
			std::cout << "\t\t" << line << std::endl;;
		}

		std::cout << "\tsupported image usage:" << std::endl;
		stringList = log_image_usage_bits(support.capabilities.supportedUsageFlags);
		for (std::string line : stringList) {
			std::cout << "\t\t" << line << std::endl;
		}
	}

	support.formats = device.getSurfaceFormatsKHR(surface);

	if (debugMode) {

		for (vk::SurfaceFormatKHR supportedFormat : support.formats) {
			/*
			* typedef struct VkSurfaceFormatKHR {
				VkFormat           format;
				VkColorSpaceKHR    colorSpace;
			} VkSurfaceFormatKHR;
			*/

			std::cout << "supported pixel format: " << vk::to_string(supportedFormat.format) << '\n';
			std::cout << "supported color space: " << vk::to_string(supportedFormat.colorSpace) << '\n';
		}
	}

	support.presentModes = device.getSurfacePresentModesKHR(surface);

	for (vk::PresentModeKHR presentMode : support.presentModes) {
		std::cout << '\t' << log_present_mode(presentMode) << '\n';
	}

	return support;
}

vk::SurfaceFormatKHR vkInit::choose_swapchain_surface_format(std::vector<vk::SurfaceFormatKHR> formats)
{
	for (vk::SurfaceFormatKHR format : formats) {
		if (format.format == vk::Format::eB8G8R8A8Unorm && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
			return format;
		}
	}
}



vk::PresentModeKHR vkInit::choose_swapchain_present_mode(std::vector<vk::PresentModeKHR> presentModes) {
	for (vk::PresentModeKHR presentMode : presentModes) {
		if (presentMode == vk::PresentModeKHR::eMailbox) return presentMode;
	}
	return vk::PresentModeKHR::eFifo; //mobilki kiedy chcemy ograniczyæ power consumption z jakiegoœ powodu ustawia siê na domyœlne
}

vk::Extent2D vkInit::choose_swapchain_exten(uint32_t width, uint32_t height, vk::SurfaceCapabilitiesKHR capabilities) {
	if (capabilities.currentExtent.width != UINT32_MAX) {
		return capabilities.currentExtent;
	}
	else {
		vk::Extent2D extent = { width,height };
		extent.width = std::min(
			capabilities.maxImageExtent.width,
			std::max(capabilities.minImageExtent.width, width)
		);
		extent.height = std::min(
			capabilities.maxImageExtent.height,
			std::max(capabilities.minImageExtent.height, height)
		);
		return extent;
	}
}

vkInit::SwapChainBundle vkInit::create_swapchain(vk::PhysicalDevice physicalDevice, vk::Device logicalDevice, vk::SurfaceKHR surface, ivec2 screenSize, bool debugMode)
{
	SwapChainSupportDetails support = query_swapchain_support(physicalDevice, surface, debugMode);
	vk::SurfaceFormatKHR format = choose_swapchain_surface_format(support.formats);
	vk::PresentModeKHR presentMode = choose_swapchain_present_mode(support.presentModes);
	
	vk::Extent2D extent = choose_swapchain_exten(screenSize.x, screenSize.y, support.capabilities);
	uint32_t imageCount = std::min(
		support.capabilities.maxImageCount,
		support.capabilities.minImageCount + 1
	);	
	vk::SwapchainCreateInfoKHR createInfo = vk::SwapchainCreateInfoKHR(
		vk::SwapchainCreateFlagsKHR(), surface, imageCount, format.format, format.colorSpace,
		extent, 1, vk::ImageUsageFlagBits::eColorAttachment
	);
	vkUtil::QueueFamilyIndices indices = vkUtil::findQueueFamilies(physicalDevice, surface, debugMode);
	uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };
	
	if (indices.graphicsFamily != indices.presentFamily) {
		createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else {
		createInfo.imageSharingMode = vk::SharingMode::eExclusive;
	}
	createInfo.preTransform = support.capabilities.currentTransform;
	createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;

	createInfo.oldSwapchain = vk::SwapchainKHR(nullptr);
	
	SwapChainBundle bundle{};
	try {
		bundle.swapchain = logicalDevice.createSwapchainKHR(createInfo);
	}
	catch (vk::SystemError err) {
		throw std::runtime_error("failed to create swap chain!");
	}

	std::vector<vk::Image> images = logicalDevice.getSwapchainImagesKHR(bundle.swapchain);
	bundle.frames.resize(images.size());

	for (size_t i = 0; i < images.size();  i++) {
		
		

		bundle.frames[i].image = images[i];
		bundle.frames[i].imageView = vkImage::make_image_view(logicalDevice, images[i], format.format, vk::ImageAspectFlagBits::eColor);
		//vkGbuffer::createGbufferAttachment(physicalDevice,logicalDevice,&bundle.frames[i].gbuffer);
		

	}
	bundle.format = format.format;
	bundle.extent = extent;

	return bundle;
}