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

	struct ssaoUBO {
		glm::mat4 projection;
		glm::vec3 samples[64];
		glm::vec2 screenSize;
		int kernelSize;//= 64;
		float radius;// = 0.5;
		float bias;// = 0.025;
	};

	struct downSacalingUBO {
		glm::vec2 srcResolution;
		int mipLevel;
	};

	struct upSacalingUBO {
		float filterRadius;
	};


	struct animatedUBO {
		glm::mat4 view;
		glm::mat4 projection;	
	};
	struct animatedSBO {
		glm::mat4 model;
		glm::mat4 finalBoneMatrices[100];
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
		vk::Framebuffer animationFramebuffer;
		vk::Framebuffer shadowFramebuffer;
		vk::Framebuffer particleFramebuffer;
		vk::Framebuffer postProcessFramebuffer;
		vk::Framebuffer skyBoxFramebuffer;
		
		vk::Framebuffer finalFramebuffer;


		std::vector<vk::Framebuffer> downscaleFramebuffer;
		std::vector<vk::Framebuffer> upscaleFramebuffer;

		//zBuffer
		vk::Image depthBuffer;
		vk::DeviceMemory depthBufferMemory;
		vk::ImageView depthBufferView;
		vk::Format depthFormat;
		int width, height;


		vk::Image skyboxdepthBuffer;
		vk::DeviceMemory skyboxdepthBufferMemory;
		vk::ImageView skyboxdepthBufferView;


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
		FrameBufferAttachment sampledAttachment;
		FrameBufferAttachment multiSampledattachment;
		FrameBufferAttachment animationattachment;

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
		vk::DescriptorSet animationDescriptorSet;
		vk::DescriptorSet deferedDescriptorSet;
		vk::DescriptorSet particleDescriptorSet;
		vk::DescriptorSet particleCameraDescriptorSet;

		vk::DescriptorSet shadowDescriptorSet;

		vk::DescriptorSet postProcessDescriptorSet;
		
		vk::DescriptorSet skyBoxDescriptorSet;

		vk::DescriptorSet ssaoDescriptorSet;

		vk::DescriptorSet blurDescriptorSet;
		vk::DescriptorSet finalDescriptorSet;

		SkyBoxUBO	skyboxData;
		Buffer skyboxUBOBuffer;
		void* skyboxUBOWriteLoacation;
		vk::DescriptorBufferInfo skyboxUBOBufferDescriptor;



		std::vector<vk::DescriptorSet> downScaleDescriptorsSet;
		std::vector<vk::DescriptorSet> upScaleDescriptorsSet;
		downSacalingUBO	downScaleUBOData;
		Buffer downScaleUBOBuffer;
		void* downScaleUBOWriteLoacation;
		vk::DescriptorBufferInfo downsScaleBufferDescriptor;
		upSacalingUBO	upScaleUBOData;
		Buffer upScaleUBOBuffer;
		void* upScaleUBOWriteLoacation;
		vk::DescriptorBufferInfo upScaleUBOBufferDescriptor;

		ssaoUBO	ssaoUBOData;
		Buffer ssaoUBOBuffer;
		void* ssaoUBOWriteLoacation;
		vk::DescriptorBufferInfo ssaoUBOBufferDescriptor;

		animatedUBO animationsUBOData;
		Buffer animationsUBOBuffer;
		void* animationsUBOWriteLoacation;
		vk::DescriptorBufferInfo animationsUBOBufferDescriptor;

		std::vector<animatedSBO> animationsSBOData;
		Buffer animationsSBOBuffer;
		void* animationsSBOWriteLoacation;
		vk::DescriptorBufferInfo animationsSBOBufferDescriptor;

		vk::Sampler upscaleSampler;

		void shadowDescripotrsWrite();
		
		void make_descriptor_resources();
		void make_animated_descriptor_resources(uint32_t animatorCounter);
		void wirte_samplingdescriptor_set(std::vector<vk::ImageView> mipImagesView, vk::Sampler sampler);
		void wirte_samplingubo_set(glm::vec2 screenSize, std::vector<glm::vec2> mipSize);
		void make_depth_resources();
		void write_descriptor_set();
		void write_animated_descriptor_set();
		void writeGbufferDescriptor(vk::DescriptorSet descriptorSet, vk::Device logicalDevice);
		void writeParticleDescriptor(vk::DescriptorBufferInfo &particleBufferDescriptor);
		void write_skybox_descriptor(std::vector<vk::ImageView> &mipImagesView,vk::Sampler sampler);
		void destroy();

	};
}
