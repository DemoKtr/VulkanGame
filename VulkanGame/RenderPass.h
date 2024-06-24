#pragma once
#include "config.h"
#include "RenderStruct.h"

namespace vkInit {

	vk::RenderPass create_defered_renderpass(vk::Device logicalDevice, vkUtil::Gbuffer gBuffer, vk::Format swapchainImageFormat, vk::Format depthFormat, vkUtil::FrameBufferAttachment postProcessImageInput);
	vk::RenderPass create_shadows_renderpass(vk::Device logicalDevice, vkUtil::shadowMapBuffer* shadowmapBuffer, vk::Format depthFormat);
	vk::RenderPass create_particle_renderpass(vk::Device logicalDevice, vk::Format depthFormat, vkUtil::FrameBufferAttachment particleAttachment);
	vk::RenderPass create_combinedImage_renderpass(vk::Device logicalDevice, vk::Format swapchainImageFormat);
	vk::RenderPass create_skybox_renderpass(vk::Device logicalDevice, vk::Format swapchainImageFormat, vk::Format depthFormat, vkUtil::FrameBufferAttachment skyBox);
	vk::RenderPass create_updownscale_renderpass(vk::Device logicalDevice);
}

