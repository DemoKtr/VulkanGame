#pragma once
#include "config.h"
#include "Image.h"



namespace vkImage{

	struct ParticleTextureInputChunk {
		vk::Device logicalDevice;
		vk::PhysicalDevice physicalDevice;
		const char* colorfilenames;
		const char* gradientfilenames;
		vk::CommandBuffer commandBuffer;
		vk::Queue queue;
		vk::DescriptorSetLayout layout;
		vk::DescriptorPool descriptorPool;

	};

class ParticleTexture
{
	int colorwidth, colorheight, colorchannels, gradientchannels, gradientwidth, gradientheight;
	vk::PhysicalDevice physicalDevice;
	vk::Device logicalDevice;

	stbi_uc* colorpixels;
	stbi_uc* gradientpixels;


	//res
	vk::Image colorimage;
	vk::DeviceMemory colorimageMemory;
	vk::ImageView colorimageView;
	vk::Sampler colorsampler;


	//res normalMap
	vk::Image gradientImage;
	vk::DeviceMemory gradientImageMemory;
	vk::ImageView gradientImageView;
	vk::Sampler gradientSampler;

	

	//Resource Descriptors
	vk::DescriptorSetLayout layout;
	vk::DescriptorSet descriptorSet;
	vk::DescriptorPool descriptorPool;

	vk::CommandBuffer commandBuffer;
	vk::Queue queue;



	void load();
	void populate();
	void make_view();
	void make_sampler();
	void make_descriptor_set();



public:
	const char* colorfilename;
	const char* gradientfilename;

	ParticleTexture(ParticleTextureInputChunk info);

	void useTexture(vk::CommandBuffer commandBuffer, vk::PipelineLayout pipelineLayout);
	~ParticleTexture();

};

}