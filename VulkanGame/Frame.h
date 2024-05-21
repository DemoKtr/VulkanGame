#pragma once
#include "config.h"
#include "Memory.h"
#include "RenderStruct.h"
namespace vkUtil {

	/**
		Describes the data to send to the shader for each frame.
	*/
	struct UBO {
		glm::mat4 view;
		glm::mat4 projection;
		glm::mat4 viewProjection;
	};
	struct PointLight {
		glm::vec3 position;

		float constant;
		float linear;
		float quadratic;

		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
	};

	/**
		Holds the data structures associated with a "Frame"
	*/
	class SwapChainFrame {

	public:
		vk::Device logicalDevice;
		vk::PhysicalDevice physicalDevice;

		//Swapchain-type stuff
		vk::Image image;
		vk::ImageView imageView;
		vk::Framebuffer framebuffer;

		//zBuffer
		vk::Image depthBuffer;
		vk::DeviceMemory depthBufferMemory;
		vk::ImageView depthBufferView;
		vk::Format depthFormat;
		int width, height;

		//gBuffer
		vkUtil::Gbuffer gbuffer;


		vk::CommandBuffer commandBuffer;

		//Sync objects
		vk::Semaphore imageAvailable, renderFinished;
		vk::Fence inFlight;

		//Resources
		UBO cameraData;
		PointLight lightData;
		Buffer cameraDataBuffer;
		Buffer lightDataBuffer;
		void* cameraDataWriteLocation;
		void* lightDataWriteLocation;


		std::vector<glm::mat4> modelTransforms;
		Buffer modelBuffer;
		void* modelBufferWriteLocation;

		//Resource Descriptors
		vk::DescriptorBufferInfo uniformBufferDescriptor;
		vk::DescriptorBufferInfo uniformlightBufferDescriptor;
		vk::DescriptorBufferInfo modelBufferDescriptor;

		vk::DescriptorSet descriptorSet;
		vk::DescriptorSet deferedDescriptorSet;

		void make_descriptor_resources();
		void make_depth_resources();
		void write_descriptor_set();

		void destroy();

	};
}
