#pragma once
#include "config.h"

namespace vkBloom {
	class PBBloom
	{
		uint32_t mipNumber = 6;

		vk::Device logicalDevice;
		vk::PhysicalDevice physicalDevice;

		vk::Image mipImages[6];
		vk::ImageView mipImagesView[6];
		vk::DeviceMemory mipImagesMemory[6];
		vk::Sampler sampler;
		
		
		
		void createMipImages();
		void createMipImageViews();
		void createSampler();
		void createPipelines();
		void draw(vk::CommandBuffer commandBuffer);


	public:
		PBBloom(float width, float heigh, vk::Device device, vk::PhysicalDevice physicalDevice);
		std::vector<glm::vec2> mipSize;
		std::vector<glm::ivec2> intMipSize;
		vk::RenderPass renderpass;
		vk::PipelineLayout downScalePipelineLayout;
		vk::PipelineLayout upScalePipelineLayout;
		std::vector<vk::Pipeline> downScalepipeline;
		std::vector<vk::Pipeline> upScalepipeline;
		vk::Framebuffer frameBuffer;
		~PBBloom();
	};

}