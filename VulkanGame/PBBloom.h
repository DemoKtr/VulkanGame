#pragma once
#include "config.h"

namespace vkBloom {
	class PBBloom
	{
		uint32_t mipNumber = 6;

		vk::Device logicalDevice;
		vk::PhysicalDevice physicalDevice;

		
		std::vector<vk::DeviceMemory> mipImagesMemory;
		vk::Sampler sampler;
		
		
		
		void createMipImages();
		void createMipImageViews();
		void createSampler();
		void createPipelines();
		void draw(vk::CommandBuffer commandBuffer,vk::Framebuffer framebuffer);


	public:
		PBBloom(float width, float heigh, vk::Device device, vk::PhysicalDevice physicalDevice);
		std::vector<glm::vec2> mipSize;
		std::vector<glm::ivec2> intMipSize;
		vk::RenderPass downScaleRenderpass;
		vk::RenderPass upScaleRenderpass;
		vk::RenderPass finalRenderpass;
		vk::PipelineLayout downScalePipelineLayout;
		vk::PipelineLayout upScalePipelineLayout;
		std::vector<vk::Pipeline> downScalepipeline;
		std::vector<vk::Pipeline> upScalepipeline;
		std::vector<vk::Image> mipImages;
		std::vector<vk::ImageView> mipImagesView;
		std::vector<vk::DescriptorSet> downScaleDescriptorsSet;
		std::vector<vk::DescriptorSet> upScaleDescriptorsSet;
		~PBBloom();
		void wirte_descriptor_set();
	};

}