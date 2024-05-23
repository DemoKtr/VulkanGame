#include "Gbuffer.h"
#include "Memory.h"
#include "Descrpitors.h"

void vkGbuffer::createAttachment(attachmentBundle attachmentDescription)
{
	vk::ImageAspectFlags aspectMask(0);
	vk::ImageLayout imageLayout;
	attachmentDescription.attachment->format = attachmentDescription.format;
	if (attachmentDescription.usage & vk::ImageUsageFlagBits::eColorAttachment) {
		aspectMask = vk::ImageAspectFlagBits::eColor;
		imageLayout = vk::ImageLayout::eColorAttachmentOptimal;
	}
	if (attachmentDescription.usage & vk::ImageUsageFlagBits::eDepthStencilAttachment) {
		aspectMask = vk::ImageAspectFlagBits::eDepth;
		if (attachmentDescription.format >= vk::Format::eD16UnormS8Uint) {
			aspectMask |= vk::ImageAspectFlagBits::eStencil;
		}
		imageLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
	}

	assert(static_cast<uint32_t>(aspectMask) > 0);
	vk::ImageCreateInfo imageInfo = {};
	imageInfo.imageType = vk::ImageType::e2D;
	imageInfo.format = attachmentDescription.format;
	imageInfo.extent.width = attachmentDescription.width;
	imageInfo.extent.height = attachmentDescription.height;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.samples = vk::SampleCountFlagBits::e1;
	imageInfo.tiling = vk::ImageTiling::eOptimal;
	imageInfo.extent.depth = 1;
	imageInfo.usage = attachmentDescription.usage | vk::ImageUsageFlagBits::eInputAttachment;
	
	attachmentDescription.attachment->image = attachmentDescription.logicalDevice.createImage(imageInfo);
	vk::MemoryRequirements requirements = attachmentDescription.logicalDevice.getImageMemoryRequirements(attachmentDescription.attachment->image);
	vk::MemoryAllocateInfo allocation;
	allocation.allocationSize = requirements.size;
	allocation.memoryTypeIndex = vkUtil::findMemoryTypeIndex(attachmentDescription.physicalDevice,requirements.memoryTypeBits,vk::MemoryPropertyFlagBits::eDeviceLocal);
	attachmentDescription.attachment->mem = attachmentDescription.logicalDevice.allocateMemory(allocation);
	attachmentDescription.logicalDevice.bindImageMemory(attachmentDescription.attachment->image, attachmentDescription.attachment->mem, 0);

	vk::ImageViewCreateInfo  imageViewInfo = {};
	imageViewInfo.viewType = vk::ImageViewType::e2D;
	imageViewInfo.format = attachmentDescription.format;
	imageViewInfo.subresourceRange;
	imageViewInfo.subresourceRange.aspectMask = aspectMask;
	imageViewInfo.subresourceRange.baseMipLevel = 0;
	imageViewInfo.subresourceRange.levelCount = 1;
	imageViewInfo.subresourceRange.baseArrayLayer = 0;
	imageViewInfo.subresourceRange.layerCount = 1;
	imageViewInfo.image = attachmentDescription.attachment->image;
	attachmentDescription.attachment->view = attachmentDescription.logicalDevice.createImageView(imageViewInfo);

}

void vkGbuffer::createGbufferAttachment(vk::PhysicalDevice physicalDevice,vk::Device logicalDevice,vkUtil::Gbuffer* gbuffer)
{
	attachmentBundle attachmentDescription;
	attachmentDescription.logicalDevice = logicalDevice;
	attachmentDescription.physicalDevice = physicalDevice;
	attachmentDescription.width = gbuffer->width;
	attachmentDescription.height = gbuffer->height;
	attachmentDescription.attachment = &gbuffer->position;
	attachmentDescription.format = vk::Format::eR16G16B16A16Sfloat;
	attachmentDescription.usage = vk::ImageUsageFlagBits::eColorAttachment;
	createAttachment(attachmentDescription);
	attachmentDescription.attachment = &gbuffer->normal;
	createAttachment(attachmentDescription);
	attachmentDescription.attachment = &gbuffer->albedo;
	attachmentDescription.format = vk::Format::eR8G8B8A8Unorm;
	createAttachment(attachmentDescription);
}

void vkGbuffer::writeGbufferDescriptor(vk::DescriptorSet descriptorSet, vk::Device logicalDevice, vkUtil::Gbuffer gBuffer)
{
	vk::DescriptorImageInfo imageDescriptorPos;
	imageDescriptorPos.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
	imageDescriptorPos.imageView = gBuffer.position.view;
	imageDescriptorPos.sampler = VK_NULL_HANDLE;

	vk::WriteDescriptorSet descriptorWritePos;
	descriptorWritePos.dstSet = descriptorSet;
	descriptorWritePos.dstBinding = 0;
	descriptorWritePos.dstArrayElement = 0;
	descriptorWritePos.descriptorType = vk::DescriptorType::eInputAttachment;
	descriptorWritePos.descriptorCount = 1;
	descriptorWritePos.pImageInfo = &imageDescriptorPos;

	//logicalDevice.updateDescriptorSets(descriptorWritePos, nullptr);

	vk::DescriptorImageInfo imageDescriptorNormal;
	imageDescriptorNormal.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
	imageDescriptorNormal.imageView = gBuffer.normal.view;
	imageDescriptorNormal.sampler = VK_NULL_HANDLE;

	vk::WriteDescriptorSet descriptorWriteNormal;
	descriptorWriteNormal.dstSet = descriptorSet;
	descriptorWriteNormal.dstBinding = 1;
	descriptorWriteNormal.dstArrayElement = 0;
	descriptorWriteNormal.descriptorType = vk::DescriptorType::eInputAttachment;
	descriptorWriteNormal.descriptorCount = 1;
	descriptorWriteNormal.pImageInfo = &imageDescriptorNormal;

	//logicalDevice.updateDescriptorSets(descriptorWriteNormal, nullptr);

	vk::DescriptorImageInfo imageDescriptoralbedo;
	imageDescriptoralbedo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
	imageDescriptoralbedo.imageView = gBuffer.albedo.view;
	imageDescriptoralbedo.sampler = VK_NULL_HANDLE;

	vk::WriteDescriptorSet descriptorWriteAlbedo;
	descriptorWriteAlbedo.dstSet = descriptorSet;
	descriptorWriteAlbedo.dstBinding = 2;
	descriptorWriteAlbedo.dstArrayElement = 0;
	descriptorWriteAlbedo.descriptorType = vk::DescriptorType::eInputAttachment;
	descriptorWriteAlbedo.descriptorCount = 1;
	descriptorWriteAlbedo.pImageInfo = &imageDescriptoralbedo;
	std::vector<vk::WriteDescriptorSet> writeDescriptorSets = {
		descriptorWritePos,descriptorWriteNormal,descriptorWriteAlbedo
	};

	logicalDevice.updateDescriptorSets(writeDescriptorSets, nullptr);

}
