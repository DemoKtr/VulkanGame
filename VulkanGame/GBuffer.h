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
	bool canUseAsSampledImage;
	vk::SampleCountFlagBits sampler;
};

namespace vkGbuffer {
	void createAttachment(attachmentBundle attachmentDescription);
	
	void createGbufferAttachment(vk::PhysicalDevice physicalDevice, vk::Device logicalDevice, vkUtil::Gbuffer* gbuffer, vkUtil::FrameBufferAttachment* attachmnent, vkUtil::FrameBufferAttachment* postProcessInputAttachment, vkUtil::FrameBufferAttachment* skyBoxAttachment, vkUtil::FrameBufferAttachment* sampledAttachment);
	void createMultiSampledAttachment(vk::PhysicalDevice physicalDevice, vk::Device logicalDevice, vkUtil::FrameBufferAttachment* attachmnent, vk::Extent2D ex, vk::Format format);
}