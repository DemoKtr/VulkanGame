#include "Texture.h"
#include "Descrpitors.h"
#include "Memory.h"

void vkImage::Texture::load()
{
	pixels = stbi_load(filename, &width, &height, &channels, STBI_rgb_alpha);
	if (!pixels) {
		std::cout << "Unable to load: " << filename << std::endl;
	}
	normalpixels = stbi_load("tex/normal.jpg", &normalwidth, &normalheight, &normalchannels, STBI_rgb_alpha);
	if (!normalpixels) {
		std::cout << "Unable to load: " << "tex / normal.jpg" << std::endl;
	}
}

void vkImage::Texture::populate()
{
	///color texture
	//First create a CPU-visible buffer...
	BufferInputChunk input;
	input.logicalDevice = logicalDevice;
	input.physicalDevice = physicalDevice;
	input.memoryProperties = vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible;
	input.usage = vk::BufferUsageFlagBits::eTransferSrc;
	input.size = width * height * 4;

	Buffer stagingBuffer = vkUtil::createBuffer(input);

	//...then fill it,
	void* writeLocation = logicalDevice.mapMemory(stagingBuffer.bufferMemory, 0, input.size);
	memcpy(writeLocation, pixels, input.size);
	logicalDevice.unmapMemory(stagingBuffer.bufferMemory);

	//then transfer it to image memory
	ImageLayoutTransitionJob transitionJob;
	transitionJob.commandBuffer = commandBuffer;
	transitionJob.queue = queue;
	transitionJob.image = image;
	transitionJob.oldLayout = vk::ImageLayout::eUndefined;
	transitionJob.newLayout = vk::ImageLayout::eTransferDstOptimal;
	transition_image_layout(transitionJob);

	BufferImageCopyJob copyJob;
	copyJob.commandBuffer = commandBuffer;
	copyJob.queue = queue;
	copyJob.srcBuffer = stagingBuffer.buffer;
	copyJob.dstImage = image;
	copyJob.width = width;
	copyJob.height = height;
	copy_buffer_to_image(copyJob);

	transitionJob.oldLayout = vk::ImageLayout::eTransferDstOptimal;
	transitionJob.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
	transition_image_layout(transitionJob);

	//Now the staging buffer can be destroyed
	logicalDevice.freeMemory(stagingBuffer.bufferMemory);
	logicalDevice.destroyBuffer(stagingBuffer.buffer);


	//normal map
	BufferInputChunk normalInput;
	normalInput.logicalDevice = logicalDevice;
	normalInput.physicalDevice = physicalDevice;
	normalInput.memoryProperties = vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible;
	normalInput.usage = vk::BufferUsageFlagBits::eTransferSrc;
	normalInput.size = normalwidth * normalheight * 4;

	/*//...then fill it,
	void* writeLocation = logicalDevice.mapMemory(stagingBuffer.bufferMemory, 0, input.size);
	memcpy(writeLocation, pixels, input.size);
	logicalDevice.unmapMemory(stagingBuffer.bufferMemory);
	*/

	Buffer normalBuffer = vkUtil::createBuffer(normalInput);

	void* normalWriteLocation = logicalDevice.mapMemory(normalBuffer.bufferMemory, 0, normalInput.size);

	memcpy(normalWriteLocation, normalpixels, normalInput.size);

	logicalDevice.unmapMemory(normalBuffer.bufferMemory);

	//then transfer it to image memory

	ImageLayoutTransitionJob normalTransitionJob;
	normalTransitionJob.commandBuffer = commandBuffer;
	normalTransitionJob.queue = queue;
	normalTransitionJob.image = normalImage;
	normalTransitionJob.oldLayout = vk::ImageLayout::eUndefined;
	normalTransitionJob.newLayout = vk::ImageLayout::eTransferDstOptimal;
	transition_image_layout(normalTransitionJob);

	BufferImageCopyJob normalcopyJob;
	normalcopyJob.commandBuffer = commandBuffer;
	normalcopyJob.queue = queue;
	normalcopyJob.srcBuffer = normalBuffer.buffer;
	normalcopyJob.dstImage = normalImage;
	normalcopyJob.width = normalwidth;
	normalcopyJob.height = normalheight;
	copy_buffer_to_image(normalcopyJob);

	normalTransitionJob.oldLayout = vk::ImageLayout::eTransferDstOptimal;
	normalTransitionJob.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
	transition_image_layout(normalTransitionJob);

	//Now the staging buffer can be destroyed
	logicalDevice.freeMemory(normalBuffer.bufferMemory);
	logicalDevice.destroyBuffer(normalBuffer.buffer);
}

void vkImage::Texture::make_view()
{

	imageView = make_image_view(logicalDevice, image, vk::Format::eR8G8B8A8Unorm, vk::ImageAspectFlagBits::eColor);
	normalImageView = make_image_view(logicalDevice, normalImage, vk::Format::eR8G8B8A8Unorm, vk::ImageAspectFlagBits::eColor);
}

void vkImage::Texture::make_sampler()
{
	/*
	typedef struct VkSamplerCreateInfo {
		VkStructureType         sType;
		const void* pNext;
		VkSamplerCreateFlags    flags;
		VkFilter                magFilter;
		VkFilter                minFilter;
		VkSamplerMipmapMode     mipmapMode;
		VkSamplerAddressMode    addressModeU;
		VkSamplerAddressMode    addressModeV;
		VkSamplerAddressMode    addressModeW;
		float                   mipLodBias;
		VkBool32                anisotropyEnable;
		float                   maxAnisotropy;
		VkBool32                compareEnable;
		VkCompareOp             compareOp;
		float                   minLod;
		float                   maxLod;
		VkBorderColor           borderColor;
		VkBool32                unnormalizedCoordinates;
	} VkSamplerCreateInfo;
	*/
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
		sampler = logicalDevice.createSampler(samplerInfo);
		normalSampler = logicalDevice.createSampler(samplerInfo);
	}
	catch (vk::SystemError err) {
		std::cout << "Failed to make sampler." << std::endl;
	}
}

void vkImage::Texture::make_descriptor_set()
{

	descriptorSet = vkInit::allocate_descriptor_set(logicalDevice, descriptorPool, layout);

	vk::DescriptorImageInfo imageDescriptor;
	imageDescriptor.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
	imageDescriptor.imageView = imageView;
	imageDescriptor.sampler = sampler;

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
	normalimageDescriptor.imageView = normalImageView;
	normalimageDescriptor.sampler = normalSampler;

	vk::WriteDescriptorSet normaldescriptorWrite;
	normaldescriptorWrite.dstSet = descriptorSet;
	normaldescriptorWrite.dstBinding = 1;
	normaldescriptorWrite.dstArrayElement = 0;
	normaldescriptorWrite.descriptorType = vk::DescriptorType::eCombinedImageSampler;
	normaldescriptorWrite.descriptorCount = 1;
	normaldescriptorWrite.pImageInfo = &normalimageDescriptor;



	logicalDevice.updateDescriptorSets(normaldescriptorWrite, nullptr);
}

vkImage::Texture::Texture(TextureInputChunk info)
{

	logicalDevice = info.logicalDevice;
	physicalDevice = info.physicalDevice;
	filename = info.filenames[0];
	commandBuffer = info.commandBuffer;
	queue = info.queue;
	layout = info.layout;
	descriptorPool = info.descriptorPool;

	load();

	ImageInputChunk imageInput;
	imageInput.logicalDevice = logicalDevice;
	imageInput.physicalDevice = physicalDevice;
	imageInput.width = width;
	imageInput.height = height;
	imageInput.format = vk::Format::eR8G8B8A8Unorm;
	imageInput.tiling = vk::ImageTiling::eOptimal;
	imageInput.usage = vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled;
	imageInput.memoryProperties = vk::MemoryPropertyFlagBits::eDeviceLocal;
	image = make_image(imageInput);
	imageMemory = make_image_memory(imageInput, image);

	ImageInputChunk normalImageInput;
	normalImageInput.logicalDevice = logicalDevice;
	normalImageInput.physicalDevice = physicalDevice;
	normalImageInput.width = normalwidth;
	normalImageInput.height = normalheight;
	normalImageInput.format = vk::Format::eR8G8B8A8Unorm;
	normalImageInput.tiling = vk::ImageTiling::eOptimal;
	normalImageInput.usage = vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled;
	normalImageInput.memoryProperties = vk::MemoryPropertyFlagBits::eDeviceLocal;
	normalImage = make_image(normalImageInput);

	normalImageMemory = make_image_memory(normalImageInput, normalImage);


	populate();

	free(pixels);
	free(normalpixels);

	make_view();

	make_sampler();

	make_descriptor_set();
}


void vkImage::Texture::useTexture(vk::CommandBuffer commandBuffer, vk::PipelineLayout pipelineLayout)
{
	commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipelineLayout, 1, descriptorSet, nullptr);
}

vkImage::Texture::~Texture() {

	logicalDevice.freeMemory(imageMemory);
	logicalDevice.freeMemory(normalImageMemory);
	logicalDevice.destroyImage(image);
	logicalDevice.destroyImage(normalImage);
	logicalDevice.destroyImageView(imageView);
	logicalDevice.destroyImageView(normalImageView);
	logicalDevice.destroySampler(sampler);
	logicalDevice.destroySampler(normalSampler);

}