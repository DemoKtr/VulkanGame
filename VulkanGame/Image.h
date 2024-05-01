#pragma once
#include "vkImage/stb_image.h"
#include "config.h"


namespace vkImage {

	struct TextureInputChunk{
		vk::Device logicalDevice;
		vk::PhysicalDevice physicalDevice;
		const char* filename;
		vk::CommandBuffer commandBuffer;
		vk::Queue queue;
		vk::DescriptorSetLayout layout;
		vk::DescriptorPool descriptorPool;

	};
	struct ImageInputChunk {
		vk::Device logicalDevice;
		vk::PhysicalDevice physicalDevice;
		int width, height;
		vk::ImageTiling tiling;
		vk::ImageUsageFlags usage;
		vk::MemoryPropertyFlags memoryProperties;

	};

	struct BufferImageCopyJob {
		vk::CommandBuffer commandBuffer;
		vk::Queue queue;
		vk::Buffer srcBuffer;
		vk::Image dstImage;
		int width, height;
	};

	struct ImageLayoutTransitionJob {
		vk::CommandBuffer commandBuffer;
		vk::Queue queue;
		vk::Image image;
		vk::ImageLayout oldLayout, newLayout;
	};

	class Texture
	{
		int width, height, channels;
		vk::Device logicalDevice;
		vk::PhysicalDevice physicalDevice;
		const char* filename;
		stbi_uc* pixels;

		//res
		vk::Image image;
		vk::DeviceMemory imageMemory;
		vk::ImageView imageView;
		vk::Sampler sampler;
		

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


	vk::Image make_image(ImageInputChunk input);

	vk::DeviceMemory make_image_memory(ImageInputChunk input, vk::Image image);
	void transition_image_layout(ImageLayoutTransitionJob job);
	void copy_buffer_to_image(BufferImageCopyJob job);
	vk::ImageView make_image_view(vk::Device logicalDevice,vk::Image image,vk::Format format);
}


