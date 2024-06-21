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
		glm::vec4 heightScale;
		glm::vec4 camPos;
	};
	struct PointLight {
		glm::vec4 position;
		glm::vec4 diffuse;
		glm::mat4 mvp[6];

	};

	struct particleUBO {
		float deltaT = {0};							//		Frame delta time
		int32_t particleCount{0};
	};

	struct SkyBoxUBO {
		glm::mat4 view;
		glm::mat4 projection;
	};

	struct UBOCameraParticle {
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;
		
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
		vk::Framebuffer particleFramebuffer;
		vk::Framebuffer postProcessFramebuffer;
		vk::Framebuffer skyBoxFramebuffer;

		//zBuffer
		vk::Image depthBuffer;
		vk::DeviceMemory depthBufferMemory;
		vk::ImageView depthBufferView;
		vk::Format depthFormat;
		int width, height;
		//particleZ
		vk::Image particledepthBuffer;
		vk::DeviceMemory particledepthBufferMemory;
		vk::ImageView particledepthBufferView;

		//gBuffer
		vkUtil::Gbuffer gbuffer;
		vkUtil::shadowMapBuffer shadowMapBuffer;


		vk::CommandBuffer commandBuffer;
		vk::CommandBuffer computeCommandBuffer;
		vk::CommandBuffer particleSeccondaryCommandBuffer;
		vk::CommandBuffer skyBoxSeccondaryCommandBuffer;


		//Sync objects
		vk::Semaphore imageAvailable, renderFinished, computeFinished;
		vk::Fence inFlight;

		//Resources
		UBO cameraData;
		UBOCameraParticle particleCameraUBOData;
		PointLight lightData;
		Buffer cameraDataBuffer;
		Buffer lightDataBuffer;
		void* cameraDataWriteLocation;
		void* lightDataWriteLocation;

	
		FrameBufferAttachment particleAttachment;
		FrameBufferAttachment postProcessInputAttachment;
		FrameBufferAttachment skyBoxAttachment;

		std::vector<glm::mat4> modelTransforms;
		std::vector<PointLight> LightTransforms;
		Buffer modelBuffer;
		void* modelBufferWriteLocation;
		glm::vec4 camPos = glm::vec4(0.0f);
		Buffer camPosBuffer;
		void* camPosWriteLoacation;

		//Resource Descriptors
		vk::DescriptorBufferInfo uniformBufferDescriptor;
		vk::DescriptorBufferInfo uniformlightBufferDescriptor;
		vk::DescriptorBufferInfo modelBufferDescriptor;
		vk::DescriptorBufferInfo camPosBufferDescriptor;
		vk::DescriptorBufferInfo camParticlePosBufferDescriptor;
		
		
		particleUBO particleUBOData;
		Buffer particleUBOBuffer;
		void* particleUBOWriteLoacation;
		vk::DescriptorBufferInfo particleUBOBufferDescriptor;

		Buffer particleCameraUBOBuffer;
		void* particleCameraUBOWriteLoacation;
		vk::DescriptorBufferInfo particleCameraUBOBufferDescriptor;


		vk::DescriptorSet descriptorSet;
		vk::DescriptorSet deferedDescriptorSet;
		vk::DescriptorSet particleDescriptorSet;
		vk::DescriptorSet particleCameraDescriptorSet;

		vk::DescriptorSet shadowDescriptorSet;

		vk::DescriptorSet postProcessDescriptorSet;
		
		vk::DescriptorSet skyBoxDescriptorSet;

		SkyBoxUBO	skyboxData;
		Buffer skyboxUBOBuffer;
		void* skyboxUBOWriteLoacation;
		vk::DescriptorBufferInfo skyboxUBOBufferDescriptor;

		void shadowDescripotrsWrite();
		
		void make_descriptor_resources();
	
		void make_depth_resources();
		void write_descriptor_set();
		void writeGbufferDescriptor(vk::DescriptorSet descriptorSet, vk::Device logicalDevice);
		void writeParticleDescriptor(vk::DescriptorBufferInfo &particleBufferDescriptor);
		void write_skybox_descriptor();
		void destroy();

	};
}
