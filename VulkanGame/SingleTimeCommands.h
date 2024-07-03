#pragma once
#include "config.h"

namespace vkUtil {
	void startJob(vk::CommandBuffer commandBuffer);
	void endJob(vk::CommandBuffer, vk::Queue queue);
}