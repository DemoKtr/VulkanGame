#pragma once
#include "vkImage/stb_image.h"
#include "config.h"


namespace vkImage {

	struct TextureInputChunk{
		vk::Device logicalDevice;
		vk::PhysicalDevice physicalDevice;
		const char* diffusefilenames;
		const char* normalfilenames;
		const char* armfilenames;
		const char* depthfilenames;
		vk::CommandBuffer commandBuffer;
		vk::Queue queue;
		vk::DescriptorSetLayout layout;
		vk::DescriptorPool descriptorPool;

	};

	struct skyBoxTextureInputChunk {
		vk::Device logicalDevice;
		vk::PhysicalDevice physicalDevice;
		std::vector<const char*> filenames;
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
		vk::Format format;
		uint32_t arrayCount;
		vk::ImageCreateFlags flags;
	};



	struct BufferImageCopyJob {
		vk::CommandBuffer commandBuffer;
		vk::Queue queue;
		vk::Buffer srcBuffer;
		vk::Image dstImage;
		int width, height;
		uint32_t arrayCount;
		
	};

	struct ImageLayoutTransitionJob {
		vk::CommandBuffer commandBuffer;
		vk::Queue queue;
		vk::Image image;
		vk::ImageLayout oldLayout, newLayout;
		uint32_t arrayCount;
	};

	vk::Image make_image(ImageInputChunk input);

	vk::DeviceMemory make_image_memory(ImageInputChunk input, vk::Image image);
	void transition_image_layout(ImageLayoutTransitionJob job);
	void copy_buffer_to_image(BufferImageCopyJob job);
	vk::ImageView make_image_view(vk::Device logicalDevice, vk::Image image, vk::Format format, vk::ImageAspectFlags aspect, vk::ImageViewType type, uint32_t arrayCount);

	vk::Format find_supported_format(
		vk::PhysicalDevice physicalDevice, const std::vector<vk::Format>& candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features
	);


	


	
}


