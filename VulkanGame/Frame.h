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
		float heightScale;
		glm::vec3 camPos;
	};
	struct PointLight {
		glm::vec3 position[2];
		glm::vec3 diffuse[2];
		glm::vec3 camPos;

	};
	struct ShadowUBO {
		glm::mat4 mvp[2][6]; //tyle ile swiatel na scenie
		glm::vec4 modelPos[2];  // tyle elemenetow ile jest modeli na scenie
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
		vk::Framebuffer shadowFramebuffer;

		//zBuffer
		vk::Image depthBuffer;
		vk::DeviceMemory depthBufferMemory;
		vk::ImageView depthBufferView;
		vk::Format depthFormat;
		int width, height;

		//gBuffer
		vkUtil::Gbuffer gbuffer;
		vkUtil::shadowMapBuffer shadowMapBuffer;


		vk::CommandBuffer commandBuffer;
		vk::CommandBuffer shadowCommandBuffer;
		vk::CommandBuffer geometryCommandBuffer;

		//Sync objects
		vk::Semaphore imageAvailable, renderFinished;
		vk::Fence inFlight;

		//Resources
		UBO cameraData;
		PointLight lightData;
		ShadowUBO shadowData;
		Buffer cameraDataBuffer;
		Buffer lightDataBuffer;
		Buffer shadowDataBuffer;
		void* cameraDataWriteLocation;
		void* lightDataWriteLocation;
		void* shadowDataWriteLocation;


		std::vector<glm::mat4> modelTransforms;
		std::vector<glm::mat4> LightTransforms;
		Buffer modelBuffer;
		void* modelBufferWriteLocation;

		//Resource Descriptors
		vk::DescriptorBufferInfo uniformBufferDescriptor;
		vk::DescriptorBufferInfo uniformlightBufferDescriptor;
		vk::DescriptorBufferInfo uniformShadowBufferDescriptor;
		vk::DescriptorBufferInfo modelBufferDescriptor;

		vk::DescriptorSet descriptorSet;
		vk::DescriptorSet deferedDescriptorSet;

		vk::DescriptorSet shadowDescriptorSet;

		void shadowDescripotrsWrite();
		void make_descriptor_resources();
		void make_depth_resources();
		void write_descriptor_set();
		void writeGbufferDescriptor(vk::DescriptorSet descriptorSet, vk::Device logicalDevice);
		void destroy();

	};
}
