#include "PBBloom.h"
#include "Memory.h"
//#include "Pipeline.h"
void vkBloom::PBBloom::createMipImages()
{
	for (uint32_t i = 0; i < mipNumber; ++i) {
		vk::ImageCreateInfo imageInfo = {};
		imageInfo.imageType = vk::ImageType::e2D;
		imageInfo.format = vk::Format::eB10G11R11UfloatPack32;
		imageInfo.extent.width = mipSize[i].x;
		imageInfo.extent.height = mipSize[i].y;
		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = 1;
		imageInfo.samples = vk::SampleCountFlagBits::e1;
		imageInfo.tiling = vk::ImageTiling::eLinear;
		imageInfo.usage = vk::ImageUsageFlagBits::eSampled | vk::ImageUsageFlagBits::eColorAttachment;
		imageInfo.extent.depth = 1;

		mipImages.push_back(logicalDevice.createImage(imageInfo));
		vk::MemoryRequirements requirements = logicalDevice.getImageMemoryRequirements(mipImages[i]);
		vk::MemoryAllocateInfo allocation;
		allocation.allocationSize = requirements.size;
		allocation.memoryTypeIndex = vkUtil::findMemoryTypeIndex(physicalDevice, requirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal);
		mipImagesMemory.push_back(logicalDevice.allocateMemory(allocation));
		logicalDevice.bindImageMemory(mipImages[i], mipImagesMemory[i], 0);
	}


}

void vkBloom::PBBloom::createMipImageViews()
{
	for (uint32_t i = 0; i < mipNumber; ++i) {
		vk::ImageViewCreateInfo  imageViewInfo = {};
		imageViewInfo.viewType = vk::ImageViewType::e2D;
		imageViewInfo.format = vk::Format::eB10G11R11UfloatPack32;
		imageViewInfo.subresourceRange;
		imageViewInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
		imageViewInfo.subresourceRange.baseMipLevel = 0;
		imageViewInfo.subresourceRange.levelCount = 1;
		imageViewInfo.subresourceRange.baseArrayLayer = 0;
		imageViewInfo.subresourceRange.layerCount = 1;
		imageViewInfo.image = mipImages[i];
		mipImagesView.push_back(logicalDevice.createImageView(imageViewInfo));
	}
	
}

void vkBloom::PBBloom::createSampler()
{
	vk::SamplerCreateInfo samplerInfo;
	samplerInfo.flags = vk::SamplerCreateFlags();
	samplerInfo.minFilter = vk::Filter::eLinear;
	samplerInfo.magFilter = vk::Filter::eLinear;
	samplerInfo.addressModeU = vk::SamplerAddressMode::eClampToEdge;
	samplerInfo.addressModeV = vk::SamplerAddressMode::eClampToEdge;
	samplerInfo.addressModeW = vk::SamplerAddressMode::eClampToEdge;

	samplerInfo.anisotropyEnable = false;
	samplerInfo.maxAnisotropy = 1.0f;

	samplerInfo.borderColor = vk::BorderColor::eIntOpaqueBlack;
	samplerInfo.unnormalizedCoordinates = false;
	samplerInfo.compareEnable = false;
	samplerInfo.compareOp = vk::CompareOp::eAlways;

	samplerInfo.mipmapMode = vk::SamplerMipmapMode::eLinear;
	samplerInfo.mipLodBias = 0.0f;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = 0.0f;

	try {
		sampler = logicalDevice.createSampler(samplerInfo);
	}
	catch (vk::SystemError err) {
		std::cout << "Failed to make sampler." << std::endl;
	}
}

void vkBloom::PBBloom::createPipelines()
{
	//vkInit::updownGraphicsPipelineInBundle input;
	//vkInit::updownGraphicsPipelineOutBundle output;
}

void vkBloom::PBBloom::draw(vk::CommandBuffer commandBuffer, vk::Framebuffer framebuffer)
{

}

vkBloom::PBBloom::PBBloom(float width, float heigh, vk::Device device, vk::PhysicalDevice physicalDevice)
{
	this->physicalDevice = physicalDevice;
	logicalDevice = device;

	glm::vec2 mipSizefloat = glm::vec2(width, heigh);
	glm::ivec2 mipSizeInt = glm::vec2((int)width, (int)heigh);
	for (uint32_t i = 0; i < mipNumber; ++i) {
			mipSizefloat *= 0.5f;
			mipSizeInt /= 2;
			mipSize.push_back(mipSizefloat);
			intMipSize.push_back(mipSizeInt);
		}
	createMipImages();
	createMipImageViews();
	createSampler();
	createPipelines();
}

vkBloom::PBBloom::~PBBloom()
{
	for (uint32_t i = 0; i < mipNumber; ++i) {
		logicalDevice.freeMemory(mipImagesMemory[i]);
		logicalDevice.destroyImage(mipImages[i]);
		logicalDevice.destroyImageView(mipImagesView[i]);
		logicalDevice.destroyPipeline(downScalepipeline[i]);
		logicalDevice.destroyPipeline(upScalepipeline[i]);
		
	}

	
	

	logicalDevice.destroySampler(sampler);
	logicalDevice.destroyRenderPass(renderpass);
	logicalDevice.destroyPipelineLayout(downScalePipelineLayout);
	logicalDevice.destroyPipelineLayout(upScalePipelineLayout);
	
}

void vkBloom::PBBloom::wirte_descriptor_set()
{



}
