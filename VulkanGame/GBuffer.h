#pragma once
#include "config.h"
#include "RenderStruct.h"

struct attachmentBundle {
	vk::Format format;
	vk::ImageUsageFlagBits usage;
	vkUtil::FrameBufferAttachment* attachment;
	int32_t width, height;
	vk::Device logicalDevice;
	vk::PhysicalDevice physicalDevice;
};

namespace vkGbuffer {
	void createAttachment(attachmentBundle attachmentDescription);
	void createGbufferAttachment(vk::PhysicalDevice physicalDevice, vk::Device logicalDevice, vkUtil::Gbuffer gbuffer);
}