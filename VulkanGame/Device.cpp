#include "Device.h"
#include "Logging.h"
#include <set>
vk::PhysicalDevice vkInit::choose_physical_device(vk::Instance& instance, bool debugMode)
{
    if (debugMode) {
        std::cout << "Choosing physical device " << std::endl;
    }

    std::vector<vk::PhysicalDevice> avilableDevices = instance.enumeratePhysicalDevices();
    
    if (debugMode) {
        std::cout << "Are " << avilableDevices.size() << "Physical Device/s avilable" << std::endl;
    }

    for (vk::PhysicalDevice device : avilableDevices) {
        if (debugMode) {
            log_device_properties(device);
        }
        if (isSuitable(device, debugMode)) {
            return device;
        }
    }

    return nullptr;
}

bool vkInit::isSuitable(const vk::PhysicalDevice& device, bool debugMode)
{
	if (debugMode) {
		std::cout << "Checking if device is suitable\n";
	}

	/*
	* A device is suitable if it can present to the screen, ie support
	* the swapchain extension
	*/
	const std::vector<const char*> requestedExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	if (debugMode) {
		std::cout << "We are requesting device extensions:\n";

		for (const char* extension : requestedExtensions) {
			std::cout << "\t\"" << extension << "\"\n";
		}

	}

	if (bool extensionsSupported = checkDeviceExtensionSupport(device, requestedExtensions, debugMode)) {

		if (debugMode) {
			std::cout << "Device can support the requested extensions!\n";
		}
	}
	else {

		if (debugMode) {
			std::cout << "Device can't support the requested extensions!\n";
		}

		return false;
	}
	return true;
}



bool vkInit::checkDeviceExtensionSupport(
	const vk::PhysicalDevice& device,
	const std::vector<const char*>& requestedExtensions,
	const bool debugMode
) {

	/*
	* Check if a given physical device can satisfy a list of requested device
	* extensions.
	*/

	std::set<std::string> requiredExtensions(requestedExtensions.begin(), requestedExtensions.end());

	if (debugMode) {
		std::cout << "Device can support extensions:\n";
	}

	for (vk::ExtensionProperties& extension : device.enumerateDeviceExtensionProperties()) {

		if (debugMode) {
			std::cout << "\t\"" << extension.extensionName << "\"\n";
		}

		//remove this from the list of required extensions (set checks for equality automatically)
		requiredExtensions.erase(extension.extensionName);
	}

	//if the set is empty then all requirements have been satisfied
	return requiredExtensions.empty();
}

vkInit::QueueFamilyIndices vkInit::findQueueFamilies(vk::PhysicalDevice device,vk::SurfaceKHR surface ,bool debugMode)
{
	QueueFamilyIndices indices;
	std::vector<vk::QueueFamilyProperties> queueFamilies =device.getQueueFamilyProperties();
	if (debugMode) {
		std::cout << "There are " << queueFamilies.size() << "supported" << std::endl;
	}

	int i{ 0 };
	for (vk::QueueFamilyProperties queueFamily : queueFamilies) {
		if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) {
			indices.graphicsFamily = i;
			

			if (debugMode) {
				std::cout << "Queue Family " << i << "is suitable for graphic"<<std::endl;
			}
		}

		if (device.getSurfaceSupportKHR(i, surface)) {
			indices.presentFamily = i;
			if (debugMode) {
				std::cout << "Queue Family " << i << "is suitable for present" << std::endl;
			}
		}

		if (indices.isComplete()) {
			break;
		}
		i++;
	}

	return indices;
}

vk::Device vkInit::create_logical_device(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface, bool debugMode)
{
	QueueFamilyIndices indices = findQueueFamilies(physicalDevice,surface ,debugMode);
	std::vector<uint32_t> uniqueIndices;
	uniqueIndices.push_back(indices.graphicsFamily.value());
	if (indices.graphicsFamily.value() != indices.presentFamily.value()) {
		uniqueIndices.push_back(indices.presentFamily.value());
	}
	float queuePriority = 1.0f;
	std::vector<vk::DeviceQueueCreateInfo> queueCreateInfo;
	for (uint32_t queueFamilyIndex : uniqueIndices) {
		queueCreateInfo.push_back(vk::DeviceQueueCreateInfo(vk::DeviceQueueCreateFlags(), indices.graphicsFamily.value(), 1, &queuePriority));
	}
	
	std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	vk::PhysicalDeviceFeatures deviceFeatures = vk::PhysicalDeviceFeatures();
	//deviceFeatures.samplerAnisotropy = true;
	std::vector<const char*> enabledLayers;
	if (debugMode) {
		enabledLayers.push_back("VK_LAYER_KHRONOS_validation");
	}
	
	
	vk::DeviceCreateInfo deviceInfo = vk::DeviceCreateInfo(vk::DeviceCreateFlags(),
		queueCreateInfo.size(), queueCreateInfo.data(),
		enabledLayers.size(), enabledLayers.data(),
		deviceExtensions.size(), deviceExtensions.data(),
		&deviceFeatures);

	try {
		vk::Device device = physicalDevice.createDevice(deviceInfo);
		if (debugMode) {
			std::cout << "Device is successfully created" << std::endl;
		}
		return device;
	}
	catch (vk::SystemError err) {
		if (debugMode) {
			std::cout << "Device creation FAILED!!!" << std::endl;
			return nullptr;
		}
	}
	return nullptr;
}

std::array<vk::Queue,2> vkInit::get_Queues(vk::PhysicalDevice physicalDevice, vk::Device device,vk::SurfaceKHR surface, bool debugMode)
{
	QueueFamilyIndices indices = findQueueFamilies(physicalDevice, surface ,debugMode);
	return { { device.getQueue(indices.graphicsFamily.value(),0), device.getQueue(indices.presentFamily.value(),0)} };
}

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
		std::cout << "Swapchain can support the following surface capabilities:"<< std::endl;

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

		std::cout << "\tminimum supported extent: "<< std::endl;
		std::cout << "\t\twidth: " << support.capabilities.minImageExtent.width << std::endl;
		std::cout << "\t\theight: " << support.capabilities.minImageExtent.height << std::endl;

		std::cout << "\tmaximum supported extent: "<<std::endl;
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

		std::cout << "\tsupported alpha operations:"<< std::endl;;
		stringList = log_alpha_composite_bits(support.capabilities.supportedCompositeAlpha);
		for (std::string line : stringList) {
			std::cout << "\t\t" << line << std::endl;;
		}

		std::cout << "\tsupported image usage:"<< std::endl;
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
	return vk::PresentModeKHR::eFifo; //mobilki kiedy chcemy ograniczyæ power consumption
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

vkInit::SwapChainBundle vkInit::create_swapchain(vk::PhysicalDevice physicalDevice, vk::Device logicalDevice, vk::SurfaceKHR surface,ivec2 screenSize, bool debugMode)
{
	SwapChainSupportDetails support = query_swapchain_support(physicalDevice,surface,debugMode);
	vk::SurfaceFormatKHR format = choose_swapchain_surface_format(support.formats);
	vk::PresentModeKHR presentMode = choose_swapchain_present_mode(support.presentModes);
	vk::Extent2D extent = choose_swapchain_exten(screenSize.x, screenSize.y,support.capabilities);
	uint32_t imageCount = std::min(
		support.capabilities.maxImageCount,
		support.capabilities.minImageCount + 1
	);
	vk::SwapchainCreateInfoKHR createInfo = vk::SwapchainCreateInfoKHR(
		vk::SwapchainCreateFlagsKHR(), surface, imageCount, format.format, format.colorSpace,
		extent, 1, vk::ImageUsageFlagBits::eColorAttachment
	);
	QueueFamilyIndices indices = findQueueFamilies(physicalDevice, surface, debugMode);
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

	bundle.images = logicalDevice.getSwapchainImagesKHR(bundle.swapchain);
	bundle.format = format.format;
	bundle.extent = extent;

	return bundle;
}
