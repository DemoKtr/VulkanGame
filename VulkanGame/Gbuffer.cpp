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
	if(!attachmentDescription.canUseAsSampledImage)
	imageInfo.usage = attachmentDescription.usage | vk::ImageUsageFlagBits::eInputAttachment;
	else
	imageInfo.usage = attachmentDescription.usage | vk::ImageUsageFlagBits::eSampled;
	
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

void vkGbuffer::createGbufferAttachment(vk::PhysicalDevice physicalDevice,vk::Device logicalDevice,vkUtil::Gbuffer* gbuffer, vkUtil::FrameBufferAttachment* attachmnent, vkUtil::FrameBufferAttachment* postProcessInputAttachment, vkUtil::FrameBufferAttachment* skyBoxAttachment)
{
	attachmentBundle attachmentDescription;
	attachmentDescription.logicalDevice = logicalDevice;
	attachmentDescription.physicalDevice = physicalDevice;
	attachmentDescription.width = gbuffer->width;
	attachmentDescription.height = gbuffer->height;
	attachmentDescription.attachment = &gbuffer->position;
	attachmentDescription.format = vk::Format::eR16G16B16A16Sfloat;
	attachmentDescription.usage = vk::ImageUsageFlagBits::eColorAttachment;
	attachmentDescription.canUseAsSampledImage = false;
	createAttachment(attachmentDescription);
	attachmentDescription.attachment = &gbuffer->normal;
	createAttachment(attachmentDescription);
	attachmentDescription.attachment = &gbuffer->arm;
	createAttachment(attachmentDescription);
	attachmentDescription.attachment = &gbuffer->T;
	createAttachment(attachmentDescription);
	attachmentDescription.attachment = &gbuffer->worldPos;
	createAttachment(attachmentDescription);
	attachmentDescription.canUseAsSampledImage = true;
	attachmentDescription.attachment = attachmnent;
	createAttachment(attachmentDescription);
	attachmentDescription.attachment = postProcessInputAttachment;
	createAttachment(attachmentDescription);
	attachmentDescription.canUseAsSampledImage = false;
	attachmentDescription.attachment = &gbuffer->albedo;
	attachmentDescription.format = vk::Format::eR8G8B8A8Unorm;
	createAttachment(attachmentDescription);
	attachmentDescription.attachment = skyBoxAttachment;
	attachmentDescription.format = vk::Format::eR8G8B8A8Unorm;
	createAttachment(attachmentDescription);
}




