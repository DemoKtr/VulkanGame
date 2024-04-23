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
