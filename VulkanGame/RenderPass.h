#pragma once
#include "config.h"
#include "RenderStruct.h"
namespace vkInit {

	vk::RenderPass create_defered_renderpass(vk::Device logicalDevice, vkUtil::Gbuffer gBuffer, vk::Format swapchainImageFormat, vk::Format depthFormat);
	vk::RenderPass create_shadows_renderpass(vk::Device logicalDevice, vkUtil::shadowMapBuffer* shadowmapBuffer, vk::Format attachmentFormat, vk::Format depthFormat);

}

