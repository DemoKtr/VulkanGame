#pragma once
#include "config.h"
#include "Image.h"


namespace vkImage {

	struct NoiseTextureInputChunk {
		vk::Device logicalDevice;
		vk::PhysicalDevice physicalDevice;
		vk::CommandBuffer commandBuffer;
		vk::Queue queue;

	};
	class NoiseTexture
	{

		int noisewidth{4}; 
		int noiseheight{4};

		

		vk::PhysicalDevice physicalDevice;
		vk::Device logicalDevice;

		std::vector<glm::vec4> ssaoNoise;

		vk::Image noiseImage;
		vk::DeviceMemory noiseimageMemory;
		vk::ImageView noiseimageView;
		vk::Sampler noisesampler;

		vk::CommandBuffer commandBuffer;
		vk::Queue queue;

		void generate();
		void populate();
		void make_view();
		void make_sampler();

	public:

		NoiseTexture(NoiseTextureInputChunk info);
		~NoiseTexture();

	};

}




