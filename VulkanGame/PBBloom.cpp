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
		imageInfo.tiling = vk::ImageTiling::eOptimal;
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

void vkBloom::PBBloom::draw(vk::CommandBuffer commandBuffer, std::vector<vk::Framebuffer> downscaleframebuffer,std::vector<vk::Framebuffer> upscaleframebuffer, std::vector<vk::DescriptorSet>downScaleDescriptorsSet, std::vector<vk::DescriptorSet>upScaleDescriptorsSet, vk::Extent2D swapchainext)
{

	vk::ClearValue colorClear;
	std::array<float, 4> colors = { 1.0f, 1.0f, 1.0f, 1.0f };
	std::array<float, 4> colorsd = { 0.0f, 0.0f, .0f, 1.0f };
	colorClear.color = vk::ClearColorValue(colors);
	std::vector<vk::ClearValue> PostProcessclearValues = { {colorClear} };

	for (uint32_t i = 0; i < downScalepipeline.size(); ++i) {
		vk::RenderPassBeginInfo finalRenderpassInfo = {};
		finalRenderpassInfo.renderPass = downScaleRenderpass;
		finalRenderpassInfo.framebuffer = downscaleframebuffer[i];
		finalRenderpassInfo.renderArea.offset.x = 0;
		finalRenderpassInfo.renderArea.offset.y = 0;
		finalRenderpassInfo.renderArea.extent.width = intMipSize[i].x;
		finalRenderpassInfo.renderArea.extent.height = intMipSize[i].y;
		finalRenderpassInfo.clearValueCount = PostProcessclearValues.size();
		finalRenderpassInfo.pClearValues = PostProcessclearValues.data();
		commandBuffer.beginRenderPass(&finalRenderpassInfo, vk::SubpassContents::eInline);
		commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, downScalepipeline[i]);
		commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, downScalePipelineLayout, 0, downScaleDescriptorsSet[i], nullptr);


		commandBuffer.draw(3, 1, 0, 0);
		commandBuffer.endRenderPass();
		
	}

	
	
	for (uint32_t i = 0; i < mipImages.size()-1; ++i) {
		vk::ImageMemoryBarrier lightbarrier;
		lightbarrier.oldLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
		lightbarrier.newLayout = vk::ImageLayout::eColorAttachmentOptimal;
		lightbarrier.srcAccessMask = vk::AccessFlagBits::eColorAttachmentRead;
		lightbarrier.dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
		lightbarrier.image = mipImages[i]; // Twoje vk::Image
		lightbarrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
		lightbarrier.subresourceRange.baseMipLevel = 0;
		lightbarrier.subresourceRange.levelCount = 1;
		lightbarrier.subresourceRange.baseArrayLayer = 0;
		lightbarrier.subresourceRange.layerCount = 1;

		commandBuffer.pipelineBarrier(
			vk::PipelineStageFlagBits::eFragmentShader, // source stage
			vk::PipelineStageFlagBits::eColorAttachmentOutput, // destination stage
			{}, // dependency flags
			nullptr, // memory barriers
			nullptr, // buffer memory barriers
			lightbarrier // image memory barriers
		);
		
	}

	

	for (uint32_t i = 0; i < upScalepipeline.size()-1; ++i) {
		vk::RenderPassBeginInfo finalRenderpassInfo = {};
		finalRenderpassInfo.renderPass = upScaleRenderpass;
		finalRenderpassInfo.framebuffer = upscaleframebuffer[i];
		finalRenderpassInfo.renderArea.offset.x = 0;
		finalRenderpassInfo.renderArea.offset.y = 0;
		
		finalRenderpassInfo.renderArea.extent.width = intMipSize[upScalepipeline.size() - 2-i].x;
		finalRenderpassInfo.renderArea.extent.height = intMipSize[upScalepipeline.size() - 2-i].y;
		finalRenderpassInfo.clearValueCount = PostProcessclearValues.size();
		finalRenderpassInfo.pClearValues = PostProcessclearValues.data();

		commandBuffer.beginRenderPass(&finalRenderpassInfo, vk::SubpassContents::eInline);
		commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, upScalepipeline[i]);
		commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, upScalePipelineLayout, 0, upScaleDescriptorsSet[i], nullptr);


		commandBuffer.draw(3, 1, 0, 0);
		commandBuffer.endRenderPass();
		
	}
	
	
	

	vk::RenderPassBeginInfo finaleRenderpassInfo = {};
	finaleRenderpassInfo.renderPass = finalRenderpass;
	finaleRenderpassInfo.framebuffer = upscaleframebuffer[upscaleframebuffer.size()-1];
	finaleRenderpassInfo.renderArea.offset.x = 0;
	finaleRenderpassInfo.renderArea.offset.y = 0;
	finaleRenderpassInfo.renderArea.extent.width = swapchainext.width;
	finaleRenderpassInfo.renderArea.extent.height = swapchainext.height;



	finaleRenderpassInfo.clearValueCount = PostProcessclearValues.size();
	finaleRenderpassInfo.pClearValues = PostProcessclearValues.data();

	commandBuffer.beginRenderPass(&finaleRenderpassInfo, vk::SubpassContents::eInline);
	commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, upScalepipeline[upScalepipeline.size()-1]);
	commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, upScalePipelineLayout, 0, upScaleDescriptorsSet[upScaleDescriptorsSet.size()-1], nullptr);


	commandBuffer.draw(3, 1, 0, 0);
	commandBuffer.endRenderPass();
	
	
}

vkBloom::PBBloom::PBBloom(float width, float heigh, vk::Device device, vk::PhysicalDevice physicalDevice)
{
	this->physicalDevice = physicalDevice;
	logicalDevice = device;

	glm::vec2 mipSizefloat = glm::vec2(width, heigh);
	glm::ivec2 mipSizeInt = glm::vec2((int)width, (int)heigh);
	initialSize = mipSizefloat;
	for (uint32_t i = 0; i < mipNumber; ++i) {
			mipSizefloat *= 0.5f;
			mipSizeInt /= 2;
			mipSize.push_back(mipSizefloat);
			intMipSize.push_back(mipSizeInt);
		}
	createMipImages();
	createMipImageViews();
	createSampler();


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
	logicalDevice.destroyRenderPass(downScaleRenderpass);
	logicalDevice.destroyRenderPass(upScaleRenderpass);
	logicalDevice.destroyRenderPass(finalRenderpass);
	logicalDevice.destroyPipelineLayout(downScalePipelineLayout);
	logicalDevice.destroyPipelineLayout(upScalePipelineLayout);


	
	
}
