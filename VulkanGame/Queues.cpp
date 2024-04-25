#include "Queues.h"
vkUtil::QueueFamilyIndices vkUtil::findQueueFamilies(vk::PhysicalDevice device, vk::SurfaceKHR surface, bool debugMode)
{
	QueueFamilyIndices indices;
	std::vector<vk::QueueFamilyProperties> queueFamilies = device.getQueueFamilyProperties();
	if (debugMode) {
		std::cout << "There are " << queueFamilies.size() << "supported" << std::endl;
	}

	int i{ 0 };
	for (vk::QueueFamilyProperties queueFamily : queueFamilies) {
		if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) {
			indices.graphicsFamily = i;


			if (debugMode) {
				std::cout << "Queue Family " << i << "is suitable for graphic" << std::endl;
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