#include "Image.h"
#define STB_IMAGE_IMPLEMENTATION
#include "vkImage/stb_image.h"
#include "Memory.h"
#include "Logging.h"
#include "single_time_commands.h"
#include "Descrpitors.h"



vk::Image vkImage::make_image(ImageInputChunk input)
{
	/*
	typedef struct VkImageCreateInfo {
		VkStructureType          sType;
		const void* pNext;
		VkImageCreateFlags       flags;
		VkImageType              imageType;
		VkFormat                 format;
		VkExtent3D               extent;
		uint32_t                 mipLevels;
		uint32_t                 arrayLayers;
		VkSampleCountFlagBits    samples;
		VkImageTiling            tiling;
		VkImageUsageFlags        usage;
		VkSharingMode            sharingMode;
		uint32_t                 queueFamilyIndexCount;
		const uint32_t* pQueueFamilyIndices;
		VkImageLayout            initialLayout;
	} VkImageCreateInfo;
	*/

	vk::ImageCreateInfo imageInfo;
	imageInfo.flags = vk::ImageCreateFlagBits();
	imageInfo.imageType = vk::ImageType::e2D;
	imageInfo.extent = vk::Extent3D(input.width, input.height, 1);
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.format = input.format;
	imageInfo.tiling = input.tiling;
	imageInfo.initialLayout = vk::ImageLayout::eUndefined;
	imageInfo.usage = input.usage;
	imageInfo.sharingMode = vk::SharingMode::eExclusive;
	imageInfo.samples = vk::SampleCountFlagBits::e1;
	
	try {
		return input.logicalDevice.createImage(imageInfo);
	}
	catch (vk::SystemError err) {
		std::cout<<"Unable to make image"<<std::endl;
	}
}

vk::DeviceMemory vkImage::make_image_memory(ImageInputChunk input, vk::Image image)
{
	vk::MemoryRequirements requirements = input.logicalDevice.getImageMemoryRequirements(image);

	vk::MemoryAllocateInfo allocation;
	allocation.allocationSize = requirements.size;
	allocation.memoryTypeIndex = vkUtil::findMemoryTypeIndex(
		input.physicalDevice, requirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal
	);

	try {
		vk::DeviceMemory imageMemory = input.logicalDevice.allocateMemory(allocation);
		input.logicalDevice.bindImageMemory(image, imageMemory, 0);
		return imageMemory;
	}
	catch (vk::SystemError err) {
		std::cout<<"Unable to allocate memory for image"<<std::endl;
	}
}

void vkImage::transition_image_layout(ImageLayoutTransitionJob job)
{
	vkUtil::start_job(job.commandBuffer);

	/*
	typedef struct VkImageSubresourceRange {
		VkImageAspectFlags    aspectMask;
		uint32_t              baseMipLevel;
		uint32_t              levelCount;
		uint32_t              baseArrayLayer;
		uint32_t              layerCount;
	} VkImageSubresourceRange;
	*/
	vk::ImageSubresourceRange access;
	access.aspectMask = vk::ImageAspectFlagBits::eColor;
	access.baseMipLevel = 0;
	access.levelCount = 1;
	access.baseArrayLayer = 0;
	access.layerCount = 1;

	/*
	typedef struct VkImageMemoryBarrier {
		VkStructureType            sType;
		const void* pNext;
		VkAccessFlags              srcAccessMask;
		VkAccessFlags              dstAccessMask;
		VkImageLayout              oldLayout;
		VkImageLayout              newLayout;
		uint32_t                   srcQueueFamilyIndex;
		uint32_t                   dstQueueFamilyIndex;
		VkImage                    image;
		VkImageSubresourceRange    subresourceRange;
	} VkImageMemoryBarrier;
	*/
	vk::ImageMemoryBarrier barrier;
	barrier.oldLayout = job.oldLayout;
	barrier.newLayout = job.newLayout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = job.image;
	barrier.subresourceRange = access;

	vk::PipelineStageFlags sourceStage, destinationStage;

	if (job.oldLayout == vk::ImageLayout::eUndefined
		&& job.newLayout == vk::ImageLayout::eTransferDstOptimal) {

		barrier.srcAccessMask = vk::AccessFlagBits::eNoneKHR;
		barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;

		sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;
		destinationStage = vk::PipelineStageFlagBits::eTransfer;
	}
	else {

		barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
		barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

		sourceStage = vk::PipelineStageFlagBits::eTransfer;
		destinationStage = vk::PipelineStageFlagBits::eFragmentShader;
	}

	job.commandBuffer.pipelineBarrier(sourceStage, destinationStage, vk::DependencyFlags(), nullptr, nullptr, barrier);

	vkUtil::end_job(job.commandBuffer, job.queue);
}

void vkImage::copy_buffer_to_image(BufferImageCopyJob job)
{
	vkUtil::start_job(job.commandBuffer);

	/*
	typedef struct VkBufferImageCopy {
		VkDeviceSize                bufferOffset;
		uint32_t                    bufferRowLength;
		uint32_t                    bufferImageHeight;
		VkImageSubresourceLayers    imageSubresource;
		VkOffset3D                  imageOffset;
		VkExtent3D                  imageExtent;
	} VkBufferImageCopy;
	*/
	vk::BufferImageCopy copy;
	copy.bufferOffset = 0;
	copy.bufferRowLength = 0;
	copy.bufferImageHeight = 0;

	vk::ImageSubresourceLayers access;
	access.aspectMask = vk::ImageAspectFlagBits::eColor;
	access.mipLevel = 0;
	access.baseArrayLayer = 0;
	access.layerCount = 1;
	copy.imageSubresource = access;

	copy.imageOffset = vk::Offset3D(0, 0, 0);
	copy.imageExtent = vk::Extent3D(
		job.width,
		job.height,
		1.0f
	);

	job.commandBuffer.copyBufferToImage(
		job.srcBuffer, job.dstImage, vk::ImageLayout::eTransferDstOptimal, copy
	);

	vkUtil::end_job(job.commandBuffer, job.queue);
}

vk::ImageView vkImage::make_image_view(vk::Device logicalDevice, vk::Image image, vk::Format format, vk::ImageAspectFlags aspect)
{
	/*
	* ImageViewCreateInfo( VULKAN_HPP_NAMESPACE::ImageViewCreateFlags flags_ = {},
		VULKAN_HPP_NAMESPACE::Image                image_ = {},
		VULKAN_HPP_NAMESPACE::ImageViewType    viewType_  = VULKAN_HPP_NAMESPACE::ImageViewType::e1D,
		VULKAN_HPP_NAMESPACE::Format           format_    = VULKAN_HPP_NAMESPACE::Format::eUndefined,
		VULKAN_HPP_NAMESPACE::ComponentMapping components_            = {},
		VULKAN_HPP_NAMESPACE::ImageSubresourceRange subresourceRange_ = {} ) VULKAN_HPP_NOEXCEPT
	*/

	vk::ImageViewCreateInfo createInfo = {};
	createInfo.image = image;
	createInfo.viewType = vk::ImageViewType::e2D;
	createInfo.format = format;
	createInfo.components.r = vk::ComponentSwizzle::eIdentity;
	createInfo.components.g = vk::ComponentSwizzle::eIdentity;
	createInfo.components.b = vk::ComponentSwizzle::eIdentity;
	createInfo.components.a = vk::ComponentSwizzle::eIdentity;
	createInfo.subresourceRange.aspectMask = aspect;
	createInfo.subresourceRange.baseMipLevel = 0;
	createInfo.subresourceRange.levelCount = 1;
	createInfo.subresourceRange.baseArrayLayer = 0;
	createInfo.subresourceRange.layerCount = 1;

	return logicalDevice.createImageView(createInfo);
}

vk::Format vkImage::find_supported_format(vk::PhysicalDevice physicalDevice, const std::vector<vk::Format>& candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features)
{
	for (vk::Format format : candidates) {
		vk::FormatProperties properties = physicalDevice.getFormatProperties(format);
		/*
		typedef struct VkFormatProperties {
			VkFormatFeatureFlags    linearTilingFeatures;
			VkFormatFeatureFlags    optimalTilingFeatures;
			VkFormatFeatureFlags    bufferFeatures;
		} VkFormatProperties;
		*/

		if (tiling == vk::ImageTiling::eLinear
			&& (properties.linearTilingFeatures & features) == features) {
			return format;
		}

		if (tiling == vk::ImageTiling::eOptimal
			&& (properties.optimalTilingFeatures & features) == features) {
			return format;
		}
		std::cout << "Error can't find suitable format" << std::endl;
		
	}
}
