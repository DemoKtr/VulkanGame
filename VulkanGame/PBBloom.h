#pragma once
#include "config.h"

namespace vkBloom {


	
	class PBBloom
	{
		uint32_t mipNumber = 6;

		vk::Device logicalDevice;
		vk::PhysicalDevice physicalDevice;

		
		std::vector<vk::DeviceMemory> mipImagesMemory;
		
		
		glm::vec2 initialSize;
		
		void createMipImages();
		void createMipImageViews();
		void createSampler();


		

		

	public:
		vk::Sampler sampler;
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
		
		void draw(vk::CommandBuffer commandBuffer, std::vector<vk::Framebuffer> downscaleframebuffer, std::vector<vk::Framebuffer> upscaleframebuffer, std::vector<vk::DescriptorSet>downScaleDescriptorsSet, std::vector<vk::DescriptorSet>upScaleDescriptorsSet, vk::Extent2D swapchainext);
		~PBBloom();
		
	};

}