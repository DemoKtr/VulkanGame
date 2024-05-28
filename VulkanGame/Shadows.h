#pragma once
#include "config.h"
#include "GBuffer.h"
namespace vkShadows{

	void createShadowsAttachment(vk::PhysicalDevice physicalDevice, vk::Device logicalDevice, vkUtil::shadowMapBuffer* shadowMapBuffer, vk::Format depthFormat);
	void createAttachment(attachmentBundle attachmentDescription);
}

