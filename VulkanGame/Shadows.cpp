#include "Shadows.h"
#include "Memory.h"

void vkShadows::createShadowsAttachment(vk::PhysicalDevice physicalDevice, vk::Device logicalDevice, vkUtil::shadowMapBuffer* shadowMapBuffer, vk::Format depthFormat)
{

	attachmentBundle attachmentDescription;
	attachmentDescription.logicalDevice = logicalDevice;
	attachmentDescription.physicalDevice = physicalDevice;
	attachmentDescription.width = shadowMapBuffer->width;
	attachmentDescription.height = shadowMapBuffer->height;
	attachmentDescription.attachment = &shadowMapBuffer->shadowBufferDepthAttachment;
	attachmentDescription.format = depthFormat;
	attachmentDescription.usage = vk::ImageUsageFlagBits::eDepthStencilAttachment;
	createAttachment(attachmentDescription);
}

void vkShadows::createAttachment(attachmentBundle attachmentDescription)
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
	imageInfo.arrayLayers = 6;
	imageInfo.samples = vk::SampleCountFlagBits::e1;
	imageInfo.tiling = vk::ImageTiling::eOptimal;
	imageInfo.extent.depth = 1;
	imageInfo.usage = attachmentDescription.usage | vk::ImageUsageFlagBits::eInputAttachment;

	attachmentDescription.attachment->image = attachmentDescription.logicalDevice.createImage(imageInfo);
	vk::MemoryRequirements requirements = attachmentDescription.logicalDevice.getImageMemoryRequirements(attachmentDescription.attachment->image);
	vk::MemoryAllocateInfo allocation;
	allocation.allocationSize = requirements.size;
	allocation.memoryTypeIndex = vkUtil::findMemoryTypeIndex(attachmentDescription.physicalDevice, requirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal);
	attachmentDescription.attachment->mem = attachmentDescription.logicalDevice.allocateMemory(allocation);
	attachmentDescription.logicalDevice.bindImageMemory(attachmentDescription.attachment->image, attachmentDescription.attachment->mem, 0);

	vk::ImageViewCreateInfo  imageViewInfo = {};
	imageViewInfo.viewType = vk::ImageViewType::e2DArray;
	imageViewInfo.format = attachmentDescription.format;
	imageViewInfo.subresourceRange.aspectMask = aspectMask;
	imageViewInfo.subresourceRange.baseMipLevel = 0;
	imageViewInfo.subresourceRange.levelCount = 1;
	imageViewInfo.subresourceRange.baseArrayLayer = 0;
	imageViewInfo.subresourceRange.layerCount = 6;
	imageViewInfo.image = attachmentDescription.attachment->image;
	attachmentDescription.attachment->view = attachmentDescription.logicalDevice.createImageView(imageViewInfo);
}
