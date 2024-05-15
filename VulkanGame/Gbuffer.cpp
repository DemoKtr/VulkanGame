#include "Gbuffer.h"
#include "Memory.h"
#include "Descrpitors.h"

void vkGbuffer::createGBufferAttachments(vk::PhysicalDevice physicalDevice, vk::Device logicalDevice, vkUtil::GbufferAttachments* gbuffer)
{
	createAttachment(physicalDevice,logicalDevice,vk::Format::eR16G16B16A16Sfloat ,vk::ImageUsageFlagBits::eColorAttachment, &gbuffer->position ,gbuffer->width, gbuffer->height);	// (World space) Positions
	createAttachment(physicalDevice, logicalDevice, vk::Format::eR16G16B16A16Sfloat, vk::ImageUsageFlagBits::eColorAttachment, &gbuffer->normal, gbuffer->width, gbuffer->height);		// (World space) Normals
	createAttachment(physicalDevice, logicalDevice, vk::Format::eR8G8B8A8Unorm, vk::ImageUsageFlagBits::eColorAttachment, &gbuffer->albedo, gbuffer->width, gbuffer->height);			// Albedo (color)
}

void vkGbuffer::createAttachment(vk::PhysicalDevice physicalDevice, vk::Device logicalDevice, vk::Format format, vk::ImageUsageFlags usage, vkUtil::FrameBufferAttachment* attachment, int32_t width, int32_t height)
{


	vk::ImageAspectFlags aspectMask = {};

	attachment->format = format;

	if (usage & vk::ImageUsageFlagBits::eColorAttachment)
	{
		
		aspectMask = vk::ImageAspectFlagBits::eColor;
	}
	if (usage &	vk::ImageUsageFlagBits::eDepthStencilAttachment)
	{
		aspectMask = vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil;
	}

	assert(aspectMask != vk::ImageAspectFlags{});

	vk::ImageCreateInfo image = {};
	image.imageType = vk::ImageType::e2D;
	image.format = format;
	image.extent.width = width;
	image.extent.height = height;
	image.extent.depth = 1;
	image.mipLevels = 1;
	image.arrayLayers = 1;
	image.samples = vk::SampleCountFlagBits::e1;
	image.tiling = vk::ImageTiling::eOptimal;
	// VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT flag is required for input attachments
	image.usage = usage | vk::ImageUsageFlagBits::eInputAttachment;
	image.initialLayout =  vk::ImageLayout::eUndefined;
	attachment->image = logicalDevice.createImage(image);

	vk::MemoryRequirements requirements = logicalDevice.getImageMemoryRequirements(attachment->image);
	vk::MemoryAllocateInfo allocation;
	allocation.allocationSize = requirements.size;
	allocation.memoryTypeIndex = vkUtil::findMemoryTypeIndex(physicalDevice, requirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal);
	try {
		vk::DeviceMemory imageMemory = logicalDevice.allocateMemory(allocation);
		logicalDevice.bindImageMemory(attachment->image, imageMemory, 0);
		attachment->mem = imageMemory;
	}
	catch (vk::SystemError err) {
		std::cout << "Unable to allocate GBuffer memory for image" << std::endl;
	}
	


	vk::ImageViewCreateInfo imageView = {};
	imageView.viewType = vk::ImageViewType::e2D;
	imageView.format = format;
	imageView.subresourceRange = vk::ImageSubresourceRange();
	imageView.subresourceRange.aspectMask = aspectMask;
	imageView.subresourceRange.baseMipLevel = 0;
	imageView.subresourceRange.levelCount = 1;
	imageView.subresourceRange.baseArrayLayer = 0;
	imageView.subresourceRange.layerCount = 1;
	imageView.image = attachment->image;
	attachment->view = logicalDevice.createImageView(imageView);
}



void vkGbuffer::make_descriptor_set(vk::Device logicalDevice, vk::DescriptorSet descriptorSet, vk::DescriptorPool descriptorPool, vk::DescriptorSetLayout layout, vkUtil::GbufferAttachments gbuffer)
{
	descriptorSet = vkInit::allocate_descriptor_set(logicalDevice, descriptorPool, layout);

	vk::DescriptorImageInfo posImageDescriptor;
	posImageDescriptor.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
	posImageDescriptor.imageView = gbuffer.position.view;
	posImageDescriptor.sampler = {nullptr};

	vk::WriteDescriptorSet posDescriptorWrite;
	posDescriptorWrite.dstSet = descriptorSet;
	posDescriptorWrite.dstBinding = 0;
	posDescriptorWrite.dstArrayElement = 0;
	posDescriptorWrite.descriptorType = vk::DescriptorType::eInputAttachment;
	posDescriptorWrite.descriptorCount = 1;
	posDescriptorWrite.pImageInfo = &posImageDescriptor;

	logicalDevice.updateDescriptorSets(posDescriptorWrite, nullptr);


	vk::DescriptorImageInfo normalImageDescriptor;
	normalImageDescriptor.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
	normalImageDescriptor.imageView = gbuffer.normal.view;
	normalImageDescriptor.sampler = { nullptr };

	vk::WriteDescriptorSet normalDescriptorWrite;
	normalDescriptorWrite.dstSet = descriptorSet;
	normalDescriptorWrite.dstBinding = 1;
	normalDescriptorWrite.dstArrayElement = 0;
	normalDescriptorWrite.descriptorType = vk::DescriptorType::eInputAttachment;
	normalDescriptorWrite.descriptorCount = 1;
	normalDescriptorWrite.pImageInfo = &normalImageDescriptor;

	logicalDevice.updateDescriptorSets(normalDescriptorWrite, nullptr);

	vk::DescriptorImageInfo albedoImageDescriptor;
	albedoImageDescriptor.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
	albedoImageDescriptor.imageView = gbuffer.albedo.view;
	albedoImageDescriptor.sampler = { nullptr };

	vk::WriteDescriptorSet albedoDescriptorWrite;
	albedoDescriptorWrite.dstSet = descriptorSet;
	albedoDescriptorWrite.dstBinding = 2;
	albedoDescriptorWrite.dstArrayElement = 0;
	albedoDescriptorWrite.descriptorType = vk::DescriptorType::eInputAttachment;
	albedoDescriptorWrite.descriptorCount = 1;
	albedoDescriptorWrite.pImageInfo = &albedoImageDescriptor;

	logicalDevice.updateDescriptorSets(normalDescriptorWrite, nullptr);
}

