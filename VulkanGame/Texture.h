#pragma once
#include "config.h"
#include "Image.h"


namespace vkImage {
	class Texture
	{
		int width, height, channels, normalchannels, normalwidth, normalheight;
		vk::Device logicalDevice;
		vk::PhysicalDevice physicalDevice;
		const char* filename;
		stbi_uc* pixels;
		stbi_uc* normalpixels;

		//res
		vk::Image image;
		vk::DeviceMemory imageMemory;
		vk::ImageView imageView;
		vk::Sampler sampler;


		//res normalMap
		vk::Image normalImage;
		vk::DeviceMemory normalImageMemory;
		vk::ImageView normalImageView;
		vk::Sampler normalSampler;


		//Resource Descriptors
		vk::DescriptorSetLayout layout;
		vk::DescriptorSet descriptorSet;
		vk::DescriptorPool descriptorPool;

		vk::CommandBuffer commandBuffer;
		vk::Queue queue;



		void load();
		void populate();
		void make_view();
		void make_sampler();
		void make_descriptor_set();



	public:
		Texture(TextureInputChunk info);

		void useTexture(vk::CommandBuffer commandBuffer, vk::PipelineLayout pipelineLayout);
		~Texture();
	};
}
