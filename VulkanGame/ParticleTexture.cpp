#include "ParticleTexture.h"
#include "Memory.h"
#include "Descrpitors.h"

void vkImage::ParticleTexture::load()
{
	colorpixels = stbi_load(colorfilename, &colorwidth, &colorheight, &colorchannels, STBI_rgb_alpha);
	if (!colorpixels) {
		std::cout << "Unable to load: " << colorfilename << std::endl;
	}
	gradientpixels = stbi_load(gradientfilename, &gradientwidth, &gradientheight, &gradientchannels, STBI_rgb_alpha);
	if (!gradientpixels) {
		std::cout << "Unable to load: " << gradientfilename << std::endl;
	}

}

void vkImage::ParticleTexture::populate()
{
	///color texture
	//First create a CPU-visible buffer...
	BufferInputChunk input;
	input.logicalDevice = logicalDevice;
	input.physicalDevice = physicalDevice;
	input.memoryProperties = vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible;
	input.usage = vk::BufferUsageFlagBits::eTransferSrc;
	input.size = colorwidth * colorheight * 4;

	Buffer stagingBuffer = vkUtil::createBuffer(input);

	//...then fill it,
	void* writeLocation = logicalDevice.mapMemory(stagingBuffer.bufferMemory, 0, input.size);
	memcpy(writeLocation, colorpixels, input.size);
	logicalDevice.unmapMemory(stagingBuffer.bufferMemory);

	//then transfer it to image memory
	ImageLayoutTransitionJob transitionJob;
	transitionJob.commandBuffer = commandBuffer;
	transitionJob.queue = queue;
	transitionJob.image = colorimage;
	transitionJob.oldLayout = vk::ImageLayout::eUndefined;
	transitionJob.newLayout = vk::ImageLayout::eTransferDstOptimal;
	transitionJob.arrayCount = 1;
	transition_image_layout(transitionJob);

	BufferImageCopyJob copyJob;
	copyJob.commandBuffer = commandBuffer;
	copyJob.queue = queue;
	copyJob.srcBuffer = stagingBuffer.buffer;
	copyJob.dstImage = colorimage;
	copyJob.width = colorwidth;
	copyJob.height = colorheight;
	copyJob.arrayCount = 1;
	copy_buffer_to_image(copyJob);

	transitionJob.oldLayout = vk::ImageLayout::eTransferDstOptimal;
	transitionJob.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
	transition_image_layout(transitionJob);

	//Now the staging buffer can be destroyed
	logicalDevice.freeMemory(stagingBuffer.bufferMemory);
	logicalDevice.destroyBuffer(stagingBuffer.buffer);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//normal map
	BufferInputChunk normalInput;
	normalInput.logicalDevice = logicalDevice;
	normalInput.physicalDevice = physicalDevice;
	normalInput.memoryProperties = vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible;
	normalInput.usage = vk::BufferUsageFlagBits::eTransferSrc;
	normalInput.size = gradientwidth * gradientheight * 4;

	/*//...then fill it,
	void* writeLocation = logicalDevice.mapMemory(stagingBuffer.bufferMemory, 0, input.size);
	memcpy(writeLocation, pixels, input.size);
	logicalDevice.unmapMemory(stagingBuffer.bufferMemory);
	*/

	Buffer normalBuffer = vkUtil::createBuffer(normalInput);

	void* normalWriteLocation = logicalDevice.mapMemory(normalBuffer.bufferMemory, 0, normalInput.size);

	memcpy(normalWriteLocation, gradientpixels, normalInput.size);

	logicalDevice.unmapMemory(normalBuffer.bufferMemory);

	//then transfer it to image memory

	ImageLayoutTransitionJob normalTransitionJob;
	normalTransitionJob.commandBuffer = commandBuffer;
	normalTransitionJob.queue = queue;
	normalTransitionJob.image = gradientImage;
	normalTransitionJob.oldLayout = vk::ImageLayout::eUndefined;
	normalTransitionJob.newLayout = vk::ImageLayout::eTransferDstOptimal;
	normalTransitionJob.arrayCount = 1;
	transition_image_layout(normalTransitionJob);

	BufferImageCopyJob normalcopyJob;
	normalcopyJob.commandBuffer = commandBuffer;
	normalcopyJob.queue = queue;
	normalcopyJob.srcBuffer = normalBuffer.buffer;
	normalcopyJob.dstImage = gradientImage;
	normalcopyJob.width = gradientwidth;
	normalcopyJob.height = gradientheight;
	normalcopyJob.arrayCount = 1;
	copy_buffer_to_image(normalcopyJob);

	normalTransitionJob.oldLayout = vk::ImageLayout::eTransferDstOptimal;
	normalTransitionJob.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
	transition_image_layout(normalTransitionJob);
	logicalDevice.freeMemory(normalBuffer.bufferMemory);
	logicalDevice.destroyBuffer(normalBuffer.buffer);

	
}

void vkImage::ParticleTexture::make_view()
{
	colorimageView = make_image_view(logicalDevice, colorimage, vk::Format::eR8G8B8A8Unorm, vk::ImageAspectFlagBits::eColor, vk::ImageViewType::e2D, 1);
	gradientImageView = make_image_view(logicalDevice, gradientImage, vk::Format::eR8G8B8A8Unorm, vk::ImageAspectFlagBits::eColor, vk::ImageViewType::e2D, 1);

}

void vkImage::ParticleTexture::make_sampler()
{
	vk::SamplerCreateInfo samplerInfo;
	samplerInfo.flags = vk::SamplerCreateFlags();
	samplerInfo.minFilter = vk::Filter::eNearest;
	samplerInfo.magFilter = vk::Filter::eLinear;
	samplerInfo.addressModeU = vk::SamplerAddressMode::eRepeat;
	samplerInfo.addressModeV = vk::SamplerAddressMode::eRepeat;
	samplerInfo.addressModeW = vk::SamplerAddressMode::eRepeat;

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
		colorsampler = logicalDevice.createSampler(samplerInfo);
		gradientSampler = logicalDevice.createSampler(samplerInfo);
	}
	catch (vk::SystemError err) {
		std::cout << "Failed to make sampler." << std::endl;
	}
}

void vkImage::ParticleTexture::make_descriptor_set()
{
	descriptorSet = vkInit::allocate_descriptor_set(logicalDevice, descriptorPool, layout);

	vk::DescriptorImageInfo imageDescriptor;
	imageDescriptor.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
	imageDescriptor.imageView = colorimageView;
	imageDescriptor.sampler = colorsampler;

	vk::WriteDescriptorSet descriptorWrite;
	descriptorWrite.dstSet = descriptorSet;
	descriptorWrite.dstBinding = 0;
	descriptorWrite.dstArrayElement = 0;
	descriptorWrite.descriptorType = vk::DescriptorType::eCombinedImageSampler;
	descriptorWrite.descriptorCount = 1;
	descriptorWrite.pImageInfo = &imageDescriptor;
	logicalDevice.updateDescriptorSets(descriptorWrite, nullptr);


	vk::DescriptorImageInfo normalimageDescriptor;
	normalimageDescriptor.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
	normalimageDescriptor.imageView = gradientImageView;
	normalimageDescriptor.sampler = gradientSampler;

	vk::WriteDescriptorSet normaldescriptorWrite;
	normaldescriptorWrite.dstSet = descriptorSet;
	normaldescriptorWrite.dstBinding = 1;
	normaldescriptorWrite.dstArrayElement = 0;
	normaldescriptorWrite.descriptorType = vk::DescriptorType::eCombinedImageSampler;
	normaldescriptorWrite.descriptorCount = 1;
	normaldescriptorWrite.pImageInfo = &normalimageDescriptor;
	logicalDevice.updateDescriptorSets(normaldescriptorWrite, nullptr);


}

vkImage::ParticleTexture::ParticleTexture(ParticleTextureInputChunk info)
{
	logicalDevice = info.logicalDevice;
	physicalDevice = info.physicalDevice;
	colorfilename = info.colorfilenames;
	gradientfilename = info.gradientfilenames;
	commandBuffer = info.commandBuffer;
	queue = info.queue;
	layout = info.layout;
	descriptorPool = info.descriptorPool;

	load();

	ImageInputChunk colorimageInput;
	colorimageInput.logicalDevice = logicalDevice;
	colorimageInput.physicalDevice = physicalDevice;
	colorimageInput.width =colorwidth;
	colorimageInput.height = colorheight;
	colorimageInput.format = vk::Format::eR8G8B8A8Unorm;
	colorimageInput.arrayCount = 1;
	colorimageInput.tiling = vk::ImageTiling::eOptimal;
	colorimageInput.usage = vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled;
	colorimageInput.memoryProperties = vk::MemoryPropertyFlagBits::eDeviceLocal;
	colorimage = make_image(colorimageInput);
	colorimageMemory = make_image_memory(colorimageInput, colorimage);

	ImageInputChunk gradientImageInput;
	gradientImageInput.logicalDevice = logicalDevice;
	gradientImageInput.physicalDevice = physicalDevice;
	gradientImageInput.width = gradientwidth;
	gradientImageInput.height = gradientheight;
	gradientImageInput.arrayCount = 1;
	gradientImageInput.format = vk::Format::eR8G8B8A8Unorm;
	gradientImageInput.tiling = vk::ImageTiling::eOptimal;
	gradientImageInput.usage = vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled;
	gradientImageInput.memoryProperties = vk::MemoryPropertyFlagBits::eDeviceLocal;
	gradientImage = make_image(gradientImageInput);
	gradientImageMemory = make_image_memory(gradientImageInput, gradientImage);

	


	populate();

	free(colorpixels);
	free(gradientpixels);


	make_view();

	make_sampler();

	make_descriptor_set();

}

void vkImage::ParticleTexture::useTexture(vk::CommandBuffer commandBuffer, vk::PipelineLayout pipelineLayout)
{
	commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipelineLayout, 1, descriptorSet, nullptr);
}

vkImage::ParticleTexture::~ParticleTexture()
{
	logicalDevice.freeMemory(colorimageMemory);
	logicalDevice.freeMemory(gradientImageMemory);
	logicalDevice.destroyImage(colorimage);
	logicalDevice.destroyImage(gradientImage);
	logicalDevice.destroyImageView(colorimageView);
	logicalDevice.destroyImageView(gradientImageView);
	logicalDevice.destroySampler(colorsampler);
	logicalDevice.destroySampler(gradientSampler);
}
