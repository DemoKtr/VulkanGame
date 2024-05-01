#pragma once
#include "config.h"

namespace vkUtil {
	void start_job(vk::CommandBuffer commandBuffer);
	void end_job(vk::CommandBuffer, vk::Queue queue);
}