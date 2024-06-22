#include "NoiseTexture.h"
#include <random>
#include "Memory.h"
void vkImage::NoiseTexture::generate()
{
    std::uniform_real_distribution<float> randomFloats(0.0, 1.0); // generates random floats between 0.0 and 1.0
    std::default_random_engine generator;

	for (unsigned int i = 0; i < 16; i++)
	{
		glm::vec4 noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f,0.0f); // rotate around z-axis (in tangent space)
		ssaoNoise.push_back(noise);
	}
    
}

void vkImage::NoiseTexture::populate()
{
	BufferInputChunk input;
	input.logicalDevice = logicalDevice;
	input.physicalDevice = physicalDevice;
	input.memoryProperties = vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible;
	input.usage = vk::BufferUsageFlagBits::eTransferSrc;

	// Popraw rozmiar bufora staging na 192 bajty (16 * 3 * sizeof(float))
	input.size = ssaoNoise.size() * sizeof(glm::vec4);

	Buffer stagingBuffer = vkUtil::createBuffer(input);

	// ...then fill it,
	void* writeLocation = logicalDevice.mapMemory(stagingBuffer.bufferMemory, 0, input.size);
	memcpy(writeLocation, ssaoNoise.data(), input.size);
	logicalDevice.unmapMemory(stagingBuffer.bufferMemory);

	// then transfer it to image memory
	ImageLayoutTransitionJob transitionJob;
	transitionJob.commandBuffer = commandBuffer;
	transitionJob.queue = queue;
	transitionJob.image = noiseImage;
	transitionJob.oldLayout = vk::ImageLayout::eUndefined;
	transitionJob.newLayout = vk::ImageLayout::eTransferDstOptimal;
	transitionJob.arrayCount = 1;
	transition_image_layout(transitionJob);

	BufferImageCopyJob copyJob;
	copyJob.commandBuffer = commandBuffer;
	copyJob.queue = queue;
	copyJob.srcBuffer = stagingBuffer.buffer;
	copyJob.dstImage = noiseImage;
	copyJob.width = noisewidth;  // np. 4
	copyJob.height = noiseheight; // np. 4
	copyJob.arrayCount = 1;
	copy_buffer_to_image(copyJob);

	transitionJob.oldLayout = vk::ImageLayout::eTransferDstOptimal;
	transitionJob.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
	transition_image_layout(transitionJob);

	// Now the staging buffer can be destroyed
	logicalDevice.freeMemory(stagingBuffer.bufferMemory);
	logicalDevice.destroyBuffer(stagingBuffer.buffer);

}

void vkImage::NoiseTexture::make_view()
{
	noiseimageView = make_image_view(logicalDevice, noiseImage, vk::Format::eR32G32B32A32Sfloat, vk::ImageAspectFlagBits::eColor, vk::ImageViewType::e2D, 1);
}

void vkImage::NoiseTexture::make_sampler()
{
	vk::SamplerCreateInfo samplerInfo;
	samplerInfo.flags = vk::SamplerCreateFlags();
	samplerInfo.minFilter = vk::Filter::eNearest;
	samplerInfo.magFilter = vk::Filter::eNearest;
	samplerInfo.addressModeU = vk::SamplerAddressMode::eRepeat;
	samplerInfo.addressModeV = vk::SamplerAddressMode::eRepeat;
	samplerInfo.addressModeW = vk::SamplerAddressMode::eRepeat;

	samplerInfo.anisotropyEnable = false;
	samplerInfo.maxAnisotropy = 1.0f;

	samplerInfo.borderColor = vk::BorderColor::eIntOpaqueBlack;
	samplerInfo.unnormalizedCoordinates = false;
	samplerInfo.compareEnable = false;
	samplerInfo.compareOp = vk::CompareOp::eAlways;

	samplerInfo.mipmapMode = vk::SamplerMipmapMode::eNearest;
	samplerInfo.mipLodBias = 0.0f;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = 0.0f;

	try {
		noisesampler = logicalDevice.createSampler(samplerInfo);
	}
	catch (vk::SystemError err) {
		std::cout << "Failed to make sampler." << std::endl;
	}

}

vkImage::NoiseTexture::NoiseTexture(NoiseTextureInputChunk info)
{
	logicalDevice = info.logicalDevice;
	physicalDevice = info.physicalDevice;;
	commandBuffer = info.commandBuffer;
	queue = info.queue;


	generate();

	ImageInputChunk imageInput;
	imageInput.logicalDevice = logicalDevice;
	imageInput.physicalDevice = physicalDevice;
	imageInput.width = noisewidth;
	imageInput.height = noiseheight;
	imageInput.format = vk::Format::eR32G32B32A32Sfloat;
	imageInput.arrayCount = 1;
	imageInput.tiling = vk::ImageTiling::eOptimal;
	imageInput.usage = vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled;
	imageInput.memoryProperties = vk::MemoryPropertyFlagBits::eDeviceLocal;
	noiseImage = make_image(imageInput);
	noiseimageMemory = make_image_memory(imageInput, noiseImage);


	populate();

	make_view();

	make_sampler();


}

vkImage::NoiseTexture::~NoiseTexture()
{
	logicalDevice.freeMemory(noiseimageMemory);
	logicalDevice.destroyImage(noiseImage);
	logicalDevice.destroyImageView(noiseimageView);
	logicalDevice.destroySampler(noisesampler);

}
