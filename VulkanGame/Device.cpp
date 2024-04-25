#include "Device.h"
#include "Logging.h"
#include "Queues.h"
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



vk::Device vkInit::create_logical_device(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface, bool debugMode)
{
	vkUtil::QueueFamilyIndices indices = vkUtil::findQueueFamilies(physicalDevice, surface, debugMode);
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
	vkUtil::QueueFamilyIndices indices = vkUtil::findQueueFamilies(physicalDevice, surface ,debugMode);
	return { { device.getQueue(indices.graphicsFamily.value(),0), device.getQueue(indices.presentFamily.value(),0)} };
}


