#pragma once
#include "config.h"
#include "Image.h"


namespace vkImage {
	class Texture
	{
		int width, height, channels, normalchannels, normalwidth, normalheight, armchannels,armheight,armwidth, depthchannels, depthheight, depthwidth;
		vk::Device logicalDevice;
		vk::PhysicalDevice physicalDevice;
		
		stbi_uc* pixels;
		stbi_uc* normalpixels;
		stbi_uc* armpixels;
		stbi_uc* depthpixels;

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

		// ARM
		vk::Image armImage;
		vk::DeviceMemory armImageMemory;
		vk::ImageView armImageView;
		vk::Sampler armSampler;

		// displacement
		vk::Image depthImage;
		vk::DeviceMemory depthImageMemory;
		vk::ImageView depthImageView;
		vk::Sampler depthSampler;


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
		const char* diffusefilename;
		const char* normalfilename;
		const char* armfilename;
		const char* depthfilename;
		Texture(TextureInputChunk info);

		void useTexture(vk::CommandBuffer commandBuffer, vk::PipelineLayout pipelineLayout);
		~Texture();
	};
}
