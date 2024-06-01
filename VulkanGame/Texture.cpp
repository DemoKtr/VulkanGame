#include "Texture.h"
#include "Descrpitors.h"
#include "Memory.h"

void vkImage::Texture::load()
{
	pixels = stbi_load(diffusefilename, &width, &height, &channels, STBI_rgb_alpha);
	if (!pixels) {
		std::cout << "Unable to load: " << diffusefilename << std::endl;
	}
	normalpixels = stbi_load(normalfilename, &normalwidth, &normalheight, &normalchannels, STBI_rgb_alpha);
	if (!normalpixels) {
		std::cout << "Unable to load: " << normalfilename << std::endl;
	}
	armpixels = stbi_load(armfilename, &armwidth, &armheight, &armchannels, STBI_rgb_alpha);
	if (!armpixels) {
		std::cout << "Unable to load: " << armfilename << std::endl;
	}
	depthpixels = stbi_load(depthfilename, &depthwidth, &depthheight, &depthchannels, STBI_rgb_alpha);
	if (!depthpixels) {
		std::cout << "Unable to load: " << depthfilename<< std::endl;
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
	transitionJob.arrayCount = 1;
	transition_image_layout(transitionJob);

	BufferImageCopyJob copyJob;
	copyJob.commandBuffer = commandBuffer;
	copyJob.queue = queue;
	copyJob.srcBuffer = stagingBuffer.buffer;
	copyJob.dstImage = image;
	copyJob.width = width;
	copyJob.height = height;
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
	normalTransitionJob.arrayCount = 1;
	transition_image_layout(normalTransitionJob);

	BufferImageCopyJob normalcopyJob;
	normalcopyJob.commandBuffer = commandBuffer;
	normalcopyJob.queue = queue;
	normalcopyJob.srcBuffer = normalBuffer.buffer;
	normalcopyJob.dstImage = normalImage;
	normalcopyJob.width = normalwidth;
	normalcopyJob.height = normalheight;
	normalcopyJob.arrayCount = 1;
	copy_buffer_to_image(normalcopyJob);

	normalTransitionJob.oldLayout = vk::ImageLayout::eTransferDstOptimal;
	normalTransitionJob.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
	transition_image_layout(normalTransitionJob);
	logicalDevice.freeMemory(normalBuffer.bufferMemory);
	logicalDevice.destroyBuffer(normalBuffer.buffer);

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	//amr
	BufferInputChunk armInput;
	armInput.logicalDevice = logicalDevice;
	armInput.physicalDevice = physicalDevice;
	armInput.memoryProperties = vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible;
	armInput.usage = vk::BufferUsageFlagBits::eTransferSrc;
	armInput.size = armwidth * armheight * 4;

	/*//...then fill it,
	void* writeLocation = logicalDevice.mapMemory(stagingBuffer.bufferMemory, 0, input.size);
	memcpy(writeLocation, pixels, input.size);
	logicalDevice.unmapMemory(stagingBuffer.bufferMemory);
	*/

	Buffer armBuffer = vkUtil::createBuffer(armInput);

	void* armWriteLocation = logicalDevice.mapMemory(armBuffer.bufferMemory, 0, armInput.size);

	memcpy(armWriteLocation, armpixels, armInput.size);

	logicalDevice.unmapMemory(armBuffer.bufferMemory);

	//then transfer it to image memory

	ImageLayoutTransitionJob armTransitionJob;
	armTransitionJob.commandBuffer = commandBuffer;
	armTransitionJob.queue = queue;
	armTransitionJob.image = armImage;
	armTransitionJob.oldLayout = vk::ImageLayout::eUndefined;
	armTransitionJob.newLayout = vk::ImageLayout::eTransferDstOptimal;
	armTransitionJob.arrayCount = 1;
	transition_image_layout(armTransitionJob);

	BufferImageCopyJob armcopyJob;
	armcopyJob.commandBuffer = commandBuffer;
	armcopyJob.queue = queue;
	armcopyJob.srcBuffer = armBuffer.buffer;
	armcopyJob.dstImage = armImage;
	armcopyJob.width = armwidth;
	armcopyJob.height = armheight;
	armcopyJob.arrayCount = 1;
	copy_buffer_to_image(armcopyJob);

	armTransitionJob.oldLayout = vk::ImageLayout::eTransferDstOptimal;
	armTransitionJob.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
	transition_image_layout(armTransitionJob);

	logicalDevice.freeMemory(armBuffer.bufferMemory);
	logicalDevice.destroyBuffer(armBuffer.buffer);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//depth
	BufferInputChunk depthInput;
	depthInput.logicalDevice = logicalDevice;
	depthInput.physicalDevice = physicalDevice;
	depthInput.memoryProperties = vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible;
	depthInput.usage = vk::BufferUsageFlagBits::eTransferSrc;
	depthInput.size = depthwidth * depthheight * 4;

	/*//...then fill it,
	void* writeLocation = logicalDevice.mapMemory(stagingBuffer.bufferMemory, 0, input.size);
	memcpy(writeLocation, pixels, input.size);
	logicalDevice.unmapMemory(stagingBuffer.bufferMemory);
	*/

	Buffer depthBuffer = vkUtil::createBuffer(depthInput);

	void* depthWriteLocation = logicalDevice.mapMemory(depthBuffer.bufferMemory, 0, depthInput.size);

	memcpy(depthWriteLocation, depthpixels, depthInput.size);

	logicalDevice.unmapMemory(depthBuffer.bufferMemory);

	//then transfer it to image memory

	ImageLayoutTransitionJob depthTransitionJob;
	depthTransitionJob.commandBuffer = commandBuffer;
	depthTransitionJob.queue = queue;
	depthTransitionJob.image = depthImage;
	depthTransitionJob.oldLayout = vk::ImageLayout::eUndefined;
	depthTransitionJob.newLayout = vk::ImageLayout::eTransferDstOptimal;
	depthTransitionJob.arrayCount = 1;
	transition_image_layout(depthTransitionJob);

	BufferImageCopyJob depthcopyJob;
	depthcopyJob.commandBuffer = commandBuffer;
	depthcopyJob.queue = queue;
	depthcopyJob.srcBuffer = depthBuffer.buffer;
	depthcopyJob.dstImage = depthImage;
	depthcopyJob.width = depthwidth;
	depthcopyJob.height = depthheight;
	depthcopyJob.arrayCount = 1;
	copy_buffer_to_image(depthcopyJob);

	depthTransitionJob.oldLayout = vk::ImageLayout::eTransferDstOptimal;
	depthTransitionJob.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
	transition_image_layout(depthTransitionJob);


	//Now the staging buffer can be destroyed
	logicalDevice.freeMemory(depthBuffer.bufferMemory);
	logicalDevice.destroyBuffer(depthBuffer.buffer);
}

void vkImage::Texture::make_view()
{

	imageView = make_image_view(logicalDevice, image, vk::Format::eR8G8B8A8Unorm, vk::ImageAspectFlagBits::eColor, vk::ImageViewType::e2D, 1);
	normalImageView = make_image_view(logicalDevice, normalImage, vk::Format::eR8G8B8A8Unorm, vk::ImageAspectFlagBits::eColor,vk::ImageViewType::e2D,1);
	armImageView = make_image_view(logicalDevice, armImage, vk::Format::eR8G8B8A8Unorm, vk::ImageAspectFlagBits::eColor,vk::ImageViewType::e2D,1);
	depthImageView = make_image_view(logicalDevice, depthImage, vk::Format::eR8G8B8A8Unorm, vk::ImageAspectFlagBits::eColor,vk::ImageViewType::e2D,1);
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
		armSampler = logicalDevice.createSampler(samplerInfo);
		depthSampler = logicalDevice.createSampler(samplerInfo);
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

	vk::DescriptorImageInfo armimageDescriptor;
	armimageDescriptor.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
	armimageDescriptor.imageView = armImageView;
	armimageDescriptor.sampler = armSampler;

	vk::WriteDescriptorSet armdescriptorWrite;
	armdescriptorWrite.dstSet = descriptorSet;
	armdescriptorWrite.dstBinding = 1;
	armdescriptorWrite.dstArrayElement = 0;
	armdescriptorWrite.descriptorType = vk::DescriptorType::eCombinedImageSampler;
	armdescriptorWrite.descriptorCount = 1;
	armdescriptorWrite.pImageInfo = &armimageDescriptor;
	logicalDevice.updateDescriptorSets(armdescriptorWrite, nullptr);

	vk::DescriptorImageInfo depthimageDescriptor;
	depthimageDescriptor.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
	depthimageDescriptor.imageView = depthImageView;
	depthimageDescriptor.sampler = depthSampler;
	vk::WriteDescriptorSet depthdescriptorWrite;
	depthdescriptorWrite.dstSet = descriptorSet;
	depthdescriptorWrite.dstBinding = 1;
	depthdescriptorWrite.dstArrayElement = 0;
	depthdescriptorWrite.descriptorType = vk::DescriptorType::eCombinedImageSampler;
	depthdescriptorWrite.descriptorCount = 1;
	depthdescriptorWrite.pImageInfo = &depthimageDescriptor;
	logicalDevice.updateDescriptorSets(depthdescriptorWrite, nullptr);
}

vkImage::Texture::Texture(TextureInputChunk info)
{

	logicalDevice = info.logicalDevice;
	physicalDevice = info.physicalDevice;
	diffusefilename = info.diffusefilenames;
	normalfilename = info.normalfilenames;
	armfilename = info.armfilenames;
	depthfilename = info.depthfilenames;
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
	imageInput.arrayCount = 1;
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
	normalImageInput.arrayCount = 1;
	normalImageInput.format = vk::Format::eR8G8B8A8Unorm;
	normalImageInput.tiling = vk::ImageTiling::eOptimal;
	normalImageInput.usage = vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled;
	normalImageInput.memoryProperties = vk::MemoryPropertyFlagBits::eDeviceLocal;
	normalImage = make_image(normalImageInput);
	normalImageMemory = make_image_memory(normalImageInput, normalImage);

	ImageInputChunk armImageInput;
	armImageInput.logicalDevice = logicalDevice;
	armImageInput.physicalDevice = physicalDevice;
	armImageInput.width = armwidth;
	armImageInput.height = armheight;
	armImageInput.arrayCount = 1;
	armImageInput.format = vk::Format::eR8G8B8A8Unorm;
	armImageInput.tiling = vk::ImageTiling::eOptimal;
	armImageInput.usage = vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled;
	armImageInput.memoryProperties = vk::MemoryPropertyFlagBits::eDeviceLocal;
	armImage = make_image(armImageInput);
	armImageMemory = make_image_memory(armImageInput, armImage);

	ImageInputChunk depthImageInput;
	depthImageInput.logicalDevice = logicalDevice;
	depthImageInput.physicalDevice = physicalDevice;
	depthImageInput.width = armwidth;
	depthImageInput.height = armheight;
	depthImageInput.arrayCount = 1;
	depthImageInput.format = vk::Format::eR8G8B8A8Unorm;
	depthImageInput.tiling = vk::ImageTiling::eOptimal;
	depthImageInput.usage = vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled;
	depthImageInput.memoryProperties = vk::MemoryPropertyFlagBits::eDeviceLocal;
	depthImage = make_image(depthImageInput);
	depthImageMemory = make_image_memory(depthImageInput, depthImage);


	populate();

	free(pixels);
	free(normalpixels);
	free(armpixels);
	free(depthpixels);

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
	logicalDevice.freeMemory(armImageMemory);
	logicalDevice.freeMemory(depthImageMemory);
	logicalDevice.destroyImage(image);
	logicalDevice.destroyImage(normalImage);
	logicalDevice.destroyImage(armImage);
	logicalDevice.destroyImage(depthImage);
	logicalDevice.destroyImageView(imageView);
	logicalDevice.destroyImageView(normalImageView);
	logicalDevice.destroyImageView(armImageView);
	logicalDevice.destroyImageView(depthImageView);
	logicalDevice.destroySampler(sampler);
	logicalDevice.destroySampler(normalSampler);
	logicalDevice.destroySampler(armSampler);
	logicalDevice.destroySampler(depthSampler);
	

}