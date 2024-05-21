#pragma once
#include "config.h"

namespace vkUtil {
	struct ObjectData {
		glm::mat4 modelMatrix;
	};
	
	
	struct FrameBufferAttachment {
		vk::Image image = VK_NULL_HANDLE;
		vk::DeviceMemory mem = VK_NULL_HANDLE;
		vk::ImageView view = VK_NULL_HANDLE;
		vk::Format format;
	};
	struct Gbuffer{
		FrameBufferAttachment position, normal, albedo;
		int32_t width;
		int32_t height;
		
	};
}