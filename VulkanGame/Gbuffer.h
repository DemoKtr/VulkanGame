#pragma once
#include "config.h"
#include "RenderStruct.h"
namespace vkGbuffer {
	void createGBufferAttachments(vk::PhysicalDevice physicalDevice, vk::Device logicalDevice, vkUtil::GbufferAttachments* gbuffer);
	void createAttachment(vk::PhysicalDevice physicalDevice, vk::Device logicalDevice, vk::Format format, vk::ImageUsageFlags usage, vkUtil::FrameBufferAttachment* attachment, int32_t width, int32_t height);
}