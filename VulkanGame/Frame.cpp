#include "Frame.h"
#include "Memory.h"
#include "Image.h"
#include "Shadows.h"
void vkUtil::SwapChainFrame::shadowDescripotrsWrite()
{
	//Geometry shader UBO uniform
	vk::WriteDescriptorSet shadowWriteInfo;
	shadowWriteInfo.dstSet = shadowDescriptorSet;
	shadowWriteInfo.dstBinding = 0;
	shadowWriteInfo.dstArrayElement = 0; //byte offset within binding for inline uniform blocks
	shadowWriteInfo.descriptorCount = 1;
	shadowWriteInfo.descriptorType = vk::DescriptorType::eStorageBuffer;
	shadowWriteInfo.pBufferInfo = &modelBufferDescriptor;

	logicalDevice.updateDescriptorSets(shadowWriteInfo, nullptr);

	vk::WriteDescriptorSet shadowWriteInfo2;
	shadowWriteInfo2.dstSet = shadowDescriptorSet;
	shadowWriteInfo2.dstBinding = 1;
	shadowWriteInfo2.dstArrayElement = 0; //byte offset within binding for inline uniform blocks
	shadowWriteInfo2.descriptorCount = 1;
	shadowWriteInfo2.descriptorType = vk::DescriptorType::eStorageBuffer;
	shadowWriteInfo2.pBufferInfo = &uniformlightBufferDescriptor;
	logicalDevice.updateDescriptorSets(shadowWriteInfo2, nullptr);
}

void vkUtil::SwapChainFrame::make_descriptor_resources() {

			BufferInputChunk input;
			input.logicalDevice = logicalDevice;
			input.memoryProperties = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent;
			input.physicalDevice = physicalDevice;
			input.size = sizeof(UBO);
			input.usage = vk::BufferUsageFlagBits::eUniformBuffer;
			cameraDataBuffer = createBuffer(input);
			particleCameraUBOBuffer = createBuffer(input);
			input.size = sizeof(glm::vec4);
			camPosBuffer  = createBuffer(input);

			input.size = sizeof(SkyBoxUBO);
			skyboxUBOBuffer = createBuffer(input);

			
			cameraDataWriteLocation = logicalDevice.mapMemory(cameraDataBuffer.bufferMemory, 0, sizeof(UBO));
			
			particleCameraUBOWriteLoacation = logicalDevice.mapMemory(particleCameraUBOBuffer.bufferMemory, 0, sizeof(UBO));
			camPosWriteLoacation = logicalDevice.mapMemory(camPosBuffer.bufferMemory, 0, sizeof(glm::vec4));
			skyboxUBOWriteLoacation = logicalDevice.mapMemory(skyboxUBOBuffer.bufferMemory,0,sizeof(SkyBoxUBO));
			////////////

			input.size = sizeof(particleUBO);
			input.usage = vk::BufferUsageFlagBits::eUniformBuffer;
			particleUBOBuffer = createBuffer(input);
			particleUBOWriteLoacation = logicalDevice.mapMemory(particleUBOBuffer.bufferMemory,0, sizeof(particleUBO));


			input.size = 1024 * sizeof(glm::mat4);
			input.usage = vk::BufferUsageFlagBits::eStorageBuffer;
			modelBuffer = createBuffer(input);
			input.usage = vk::BufferUsageFlagBits::eStorageBuffer;
			input.size = 1024 * sizeof(PointLight);
			lightDataBuffer = createBuffer(input);
			lightDataWriteLocation = logicalDevice.mapMemory(lightDataBuffer.bufferMemory, 0, 1024 * sizeof(PointLight));
			modelBufferWriteLocation = logicalDevice.mapMemory(modelBuffer.bufferMemory, 0, 1024 * sizeof(glm::mat4));

			modelTransforms.reserve(1024);
			LightTransforms.reserve(1024);
			
			for (int i = 0; i < 1024; ++i)
			{
				modelTransforms.push_back(glm::mat4(1.0f));
				LightTransforms.push_back(PointLight());
			}
			
			
			
			
			
			/*
			typedef struct VkDescriptorBufferInfo {
				VkBuffer        buffer;
				VkDeviceSize    offset;
				VkDeviceSize    range;
			} VkDescriptorBufferInfo; =
			*/
			camPosBufferDescriptor.buffer = camPosBuffer.buffer;
			camPosBufferDescriptor.offset = 0;
			camPosBufferDescriptor.range = sizeof(glm::vec4);


			skyboxUBOBufferDescriptor.buffer = skyboxUBOBuffer.buffer;
			skyboxUBOBufferDescriptor.offset = 0;
			skyboxUBOBufferDescriptor.range = sizeof(SkyBoxUBO);

			particleCameraUBOBufferDescriptor.buffer = particleCameraUBOBuffer.buffer;
			particleCameraUBOBufferDescriptor.offset = 0;
			particleCameraUBOBufferDescriptor.range = sizeof(UBO);


			uniformBufferDescriptor.buffer = cameraDataBuffer.buffer;
			uniformBufferDescriptor.offset = 0;
			uniformBufferDescriptor.range = sizeof(UBO);


			particleUBOBufferDescriptor.buffer = particleUBOBuffer.buffer;
			particleUBOBufferDescriptor.offset = 0;
			particleUBOBufferDescriptor.range = sizeof(particleUBO);

			uniformlightBufferDescriptor.buffer = lightDataBuffer.buffer;
			uniformlightBufferDescriptor.offset = 0;
			uniformlightBufferDescriptor.range = 1024 * sizeof(PointLight);


			modelBufferDescriptor.buffer = modelBuffer.buffer;
			modelBufferDescriptor.offset = 0;
			modelBufferDescriptor.range = 1024 * sizeof(glm::mat4);

		}



void vkUtil::SwapChainFrame::write_descriptor_set() {

			vk::WriteDescriptorSet writeInfo;
			/*
			typedef struct VkWriteDescriptorSet {
				VkStructureType                  sType;
				const void* pNext;
				VkDescriptorSet                  dstSet;
				uint32_t                         dstBinding;
				uint32_t                         dstArrayElement;
				uint32_t                         descriptorCount;
				VkDescriptorType                 descriptorType;
				const VkDescriptorImageInfo* pImageInfo;
				const VkDescriptorBufferInfo* pBufferInfo;
				const VkBufferView* pTexelBufferView;
			} VkWriteDescriptorSet;
			*/

			writeInfo.dstSet = descriptorSet;
			writeInfo.dstBinding = 0;
			writeInfo.dstArrayElement = 0; //byte offset within binding for inline uniform blocks
			writeInfo.descriptorCount = 1;
			writeInfo.descriptorType = vk::DescriptorType::eUniformBuffer;
			writeInfo.pBufferInfo = &uniformBufferDescriptor;

			logicalDevice.updateDescriptorSets(writeInfo, nullptr);

			vk::WriteDescriptorSet writeInfo2;
			writeInfo2.dstSet = descriptorSet;
			writeInfo2.dstBinding = 1;
			writeInfo2.dstArrayElement = 0; //byte offset within binding for inline uniform blocks
			writeInfo2.descriptorCount = 1;
			writeInfo2.descriptorType = vk::DescriptorType::eStorageBuffer;
			writeInfo2.pBufferInfo = &modelBufferDescriptor;

			logicalDevice.updateDescriptorSets(writeInfo2, nullptr);

			
			
			
			
		}
	
void vkUtil::SwapChainFrame::make_depth_resources()
		{
			depthFormat = vkImage::find_supported_format(physicalDevice, { vk::Format::eD32Sfloat,vk::Format::eD24UnormS8Uint }, vk::ImageTiling::eOptimal, vk::FormatFeatureFlagBits::eDepthStencilAttachment);

			vkImage::ImageInputChunk imageInfo;
			imageInfo.logicalDevice = logicalDevice;
			imageInfo.physicalDevice = physicalDevice;
			imageInfo.tiling = vk::ImageTiling::eOptimal;
			imageInfo.usage = vk::ImageUsageFlagBits::eDepthStencilAttachment;
			imageInfo.memoryProperties = vk::MemoryPropertyFlagBits::eDeviceLocal;
			imageInfo.width = width;
			imageInfo.height = height;
			imageInfo.format = depthFormat;
			imageInfo.arrayCount = 1;
			depthBuffer = vkImage::make_image(imageInfo);
			particledepthBuffer = vkImage::make_image(imageInfo);
			
			depthBufferMemory = vkImage::make_image_memory(imageInfo, depthBuffer);
			particledepthBufferMemory = vkImage::make_image_memory(imageInfo, particledepthBuffer);
			depthBufferView = vkImage::make_image_view(
				logicalDevice, particledepthBuffer, depthFormat, vk::ImageAspectFlagBits::eDepth, vk::ImageViewType::e2D, 1
			);
			particledepthBufferView = vkImage::make_image_view(
				logicalDevice, depthBuffer, depthFormat, vk::ImageAspectFlagBits::eDepth, vk::ImageViewType::e2D, 1
			);
			shadowMapBuffer.height = 1024;
			shadowMapBuffer.width = 1024;
			vkShadows::createShadowsAttachment(physicalDevice, logicalDevice, &shadowMapBuffer, depthFormat);
			vk::SamplerCreateInfo samplerInfo = {};
			samplerInfo.magFilter = vk::Filter::eLinear;
			samplerInfo.minFilter = vk::Filter::eLinear;
			samplerInfo.mipmapMode = vk::SamplerMipmapMode::eLinear;
			samplerInfo.addressModeU = vk::SamplerAddressMode::eClampToEdge;
			samplerInfo.addressModeV = vk::SamplerAddressMode::eClampToEdge;
			samplerInfo.addressModeW = vk::SamplerAddressMode::eClampToEdge;
			samplerInfo.mipLodBias = 0.0f;
			samplerInfo.maxAnisotropy = 1.0f;
			samplerInfo.minLod = 0.0f;
			samplerInfo.maxLod = 1.0f;
			samplerInfo.borderColor = vk::BorderColor::eFloatOpaqueWhite;
			shadowMapBuffer.sampler = logicalDevice.createSampler(samplerInfo);
			
			
		}


void vkUtil::SwapChainFrame::writeGbufferDescriptor(vk::DescriptorSet descriptorSet, vk::Device logicalDevice)
		{
			vk::DescriptorImageInfo imageDescriptorPos;
			imageDescriptorPos.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
			imageDescriptorPos.imageView = gbuffer.position.view;
			imageDescriptorPos.sampler = VK_NULL_HANDLE;

			vk::WriteDescriptorSet descriptorWritePos;
			descriptorWritePos.dstSet = deferedDescriptorSet;
			descriptorWritePos.dstBinding = 0;
			descriptorWritePos.dstArrayElement = 0;
			descriptorWritePos.descriptorType = vk::DescriptorType::eInputAttachment;
			descriptorWritePos.descriptorCount = 1;
			descriptorWritePos.pImageInfo = &imageDescriptorPos;

			//logicalDevice.updateDescriptorSets(descriptorWritePos, nullptr);

			vk::DescriptorImageInfo imageDescriptorNormal;
			imageDescriptorNormal.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
			imageDescriptorNormal.imageView = gbuffer.normal.view;
			imageDescriptorNormal.sampler = VK_NULL_HANDLE;

			vk::WriteDescriptorSet descriptorWriteNormal;
			descriptorWriteNormal.dstSet = deferedDescriptorSet;
			descriptorWriteNormal.dstBinding = 1;
			descriptorWriteNormal.dstArrayElement = 0;
			descriptorWriteNormal.descriptorType = vk::DescriptorType::eInputAttachment;
			descriptorWriteNormal.descriptorCount = 1;
			descriptorWriteNormal.pImageInfo = &imageDescriptorNormal;

			//logicalDevice.updateDescriptorSets(descriptorWriteNormal, nullptr);

			vk::DescriptorImageInfo imageDescriptoralbedo;
			imageDescriptoralbedo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
			imageDescriptoralbedo.imageView = gbuffer.albedo.view;
			imageDescriptoralbedo.sampler = VK_NULL_HANDLE;

			vk::WriteDescriptorSet descriptorWriteAlbedo;
			descriptorWriteAlbedo.dstSet = deferedDescriptorSet;
			descriptorWriteAlbedo.dstBinding = 2;
			descriptorWriteAlbedo.dstArrayElement = 0;
			descriptorWriteAlbedo.descriptorType = vk::DescriptorType::eInputAttachment;
			descriptorWriteAlbedo.descriptorCount = 1;
			descriptorWriteAlbedo.pImageInfo = &imageDescriptoralbedo;

			

			vk::DescriptorImageInfo imageDescriptorarm;
			imageDescriptorarm.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
			imageDescriptorarm.imageView = gbuffer.arm.view;
			imageDescriptorarm.sampler = VK_NULL_HANDLE;

			vk::WriteDescriptorSet descriptorWritearm;
			descriptorWritearm.dstSet = deferedDescriptorSet;
			descriptorWritearm.dstBinding = 3;
			descriptorWritearm.dstArrayElement = 0;
			descriptorWritearm.descriptorType = vk::DescriptorType::eInputAttachment;
			descriptorWritearm.descriptorCount = 1;
			descriptorWritearm.pImageInfo = &imageDescriptorarm;

			vk::DescriptorImageInfo imageDescriptorT;
			imageDescriptorT.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
			imageDescriptorT.imageView = gbuffer.T.view;
			imageDescriptorT.sampler = VK_NULL_HANDLE;

			vk::WriteDescriptorSet descriptorWriteT;
			descriptorWriteT.dstSet = deferedDescriptorSet;
			descriptorWriteT.dstBinding = 4;
			descriptorWriteT.dstArrayElement = 0;
			descriptorWriteT.descriptorType = vk::DescriptorType::eInputAttachment;
			descriptorWriteT.descriptorCount = 1;
			descriptorWriteT.pImageInfo = &imageDescriptorT;
			
			
			vk::WriteDescriptorSet writeInfo;
			writeInfo.dstSet = deferedDescriptorSet;
			writeInfo.dstBinding = 5;
			writeInfo.dstArrayElement = 0; //byte offset within binding for inline uniform blocks
			writeInfo.descriptorCount = 1;
			writeInfo.descriptorType = vk::DescriptorType::eStorageBuffer;
			writeInfo.pBufferInfo = &uniformlightBufferDescriptor;

			vk::WriteDescriptorSet camPosWriteInfo;
			camPosWriteInfo.dstSet = deferedDescriptorSet;
			camPosWriteInfo.dstBinding = 6;
			camPosWriteInfo.dstArrayElement = 0; //byte offset within binding for inline uniform blocks
			camPosWriteInfo.descriptorCount = 1;
			camPosWriteInfo.descriptorType = vk::DescriptorType::eUniformBuffer;
			camPosWriteInfo.pBufferInfo = &camPosBufferDescriptor;



			vk::DescriptorImageInfo imageDescriptorShadow;
			imageDescriptorShadow.imageLayout = vk::ImageLayout::eDepthStencilReadOnlyOptimal;
			imageDescriptorShadow.imageView = shadowMapBuffer.shadowBufferDepthAttachment.view;
			imageDescriptorShadow.sampler = shadowMapBuffer.sampler;
			vk::WriteDescriptorSet imageShadow;
			imageShadow.dstSet = deferedDescriptorSet;
			imageShadow.dstBinding = 7;
			imageShadow.dstArrayElement = 0; //byte offset within binding for inline uniform blocks
			imageShadow.descriptorCount = 1;
			imageShadow.descriptorType = vk::DescriptorType::eCombinedImageSampler;
			imageShadow.pImageInfo = &imageDescriptorShadow;


			vk::DescriptorImageInfo imageDescriptorworldPos;
			imageDescriptorworldPos.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
			imageDescriptorworldPos.imageView = gbuffer.worldPos.view; 
			imageDescriptorworldPos.sampler = VK_NULL_HANDLE;

			vk::WriteDescriptorSet descriptorWriteworldPos;
			descriptorWriteworldPos.dstSet = deferedDescriptorSet;
			descriptorWriteworldPos.dstBinding = 8;
			descriptorWriteworldPos.dstArrayElement = 0;
			descriptorWriteworldPos.descriptorType = vk::DescriptorType::eInputAttachment;
			descriptorWriteworldPos.descriptorCount = 1;
			descriptorWriteworldPos.pImageInfo = &imageDescriptorworldPos;



			std::vector<vk::WriteDescriptorSet> writeDescriptorSets = {
				descriptorWritePos,descriptorWriteNormal,descriptorWriteAlbedo,descriptorWritearm,descriptorWriteT,writeInfo,camPosWriteInfo,imageShadow,descriptorWriteworldPos,
			};

			logicalDevice.updateDescriptorSets(writeDescriptorSets, nullptr);

		}

void vkUtil::SwapChainFrame::writeParticleDescriptor(vk::DescriptorBufferInfo &particleBufferDescriptor)
		{

	/*
	typedef struct VkWriteDescriptorSet {
		VkStructureType                  sType;
		const void* pNext;
		VkDescriptorSet                  dstSet;
		uint32_t                         dstBinding;
		uint32_t                         dstArrayElement;
		uint32_t                         descriptorCount;
		VkDescriptorType                 descriptorType;
		const VkDescriptorImageInfo* pImageInfo;
		const VkDescriptorBufferInfo* pBufferInfo;
		const VkBufferView* pTexelBufferView;
	} VkWriteDescriptorSet;
	*/

	vk::WriteDescriptorSet particleCameraWriteInfo;
	particleCameraWriteInfo.dstSet = particleCameraDescriptorSet;
	particleCameraWriteInfo.dstBinding = 0;
	particleCameraWriteInfo.dstArrayElement = 0; //byte offset within binding for inline uniform blocks
	particleCameraWriteInfo.descriptorCount = 1;
	particleCameraWriteInfo.descriptorType = vk::DescriptorType::eUniformBuffer;
	particleCameraWriteInfo.pBufferInfo = &particleCameraUBOBufferDescriptor;

	logicalDevice.updateDescriptorSets(particleCameraWriteInfo, nullptr);

	vk::WriteDescriptorSet writeInfo;
	

	writeInfo.dstSet = particleDescriptorSet;
	writeInfo.dstBinding = 0;
	writeInfo.dstArrayElement = 0; //byte offset within binding for inline uniform blocks
	writeInfo.descriptorCount = 1;
	writeInfo.descriptorType = vk::DescriptorType::eUniformBuffer;
	writeInfo.pBufferInfo = &particleUBOBufferDescriptor;

	logicalDevice.updateDescriptorSets(writeInfo, nullptr);

	vk::WriteDescriptorSet writeInfo2;
	writeInfo2.dstSet = particleDescriptorSet;
	writeInfo2.dstBinding = 1;
	writeInfo2.dstArrayElement = 0; //byte offset within binding for inline uniform blocks
	writeInfo2.descriptorCount = 1;
	writeInfo2.descriptorType = vk::DescriptorType::eStorageBuffer;
	writeInfo2.pBufferInfo = &particleBufferDescriptor;

	logicalDevice.updateDescriptorSets(writeInfo2, nullptr);

	
	


		}

void vkUtil::SwapChainFrame::write_skybox_descriptor()
{
	vk::WriteDescriptorSet skyboxCameraDescriptor;
	skyboxCameraDescriptor.dstSet = skyBoxDescriptorSet;
	skyboxCameraDescriptor.dstBinding = 0;
	skyboxCameraDescriptor.dstArrayElement = 0; //byte offset within binding for inline uniform blocks
	skyboxCameraDescriptor.descriptorCount = 1;
	skyboxCameraDescriptor.descriptorType = vk::DescriptorType::eUniformBuffer;
	skyboxCameraDescriptor.pBufferInfo = &skyboxUBOBufferDescriptor;



	vk::DescriptorImageInfo postProcessInputAttachmentImage;
	postProcessInputAttachmentImage.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
	postProcessInputAttachmentImage.imageView = postProcessInputAttachment.view;
	postProcessInputAttachmentImage.sampler = shadowMapBuffer.sampler;
	vk::WriteDescriptorSet postProcessInputDescriptor;
	postProcessInputDescriptor.dstSet = skyBoxDescriptorSet;
	postProcessInputDescriptor.dstBinding = 1;
	postProcessInputDescriptor.dstArrayElement = 0; //byte offset within binding for inline uniform blocks
	postProcessInputDescriptor.descriptorCount = 1;
	postProcessInputDescriptor.descriptorType = vk::DescriptorType::eCombinedImageSampler;
	postProcessInputDescriptor.pImageInfo = &postProcessInputAttachmentImage;

	vk::DescriptorImageInfo particleImage;
	particleImage.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
	particleImage.imageView = particleAttachment.view;
	particleImage.sampler = shadowMapBuffer.sampler;
	vk::WriteDescriptorSet particleDescriptor;
	particleDescriptor.dstSet = skyBoxDescriptorSet;
	particleDescriptor.dstBinding = 2;
	particleDescriptor.dstArrayElement = 0; //byte offset within binding for inline uniform blocks
	particleDescriptor.descriptorCount = 1;
	particleDescriptor.descriptorType = vk::DescriptorType::eCombinedImageSampler;
	particleDescriptor.pImageInfo = &particleImage;


	std::vector<vk::WriteDescriptorSet> writeDescriptorSets = {
				skyboxCameraDescriptor, postProcessInputDescriptor, particleDescriptor
	};

	logicalDevice.updateDescriptorSets(writeDescriptorSets, nullptr);
}




void vkUtil::SwapChainFrame::destroy()
		{
			logicalDevice.destroyImageView(imageView);
			logicalDevice.destroyFramebuffer(framebuffer);
			logicalDevice.destroyFramebuffer(shadowFramebuffer);
			logicalDevice.destroyFramebuffer(particleFramebuffer);
			

			try {
				logicalDevice.destroySemaphore(imageAvailable);
				logicalDevice.destroySemaphore(computeFinished);


				logicalDevice.destroySemaphore(renderFinished);
			}
			catch (vk::SystemError err) {
				 std::cout << "Failed delete semaphores!" << std::endl;
			}
			
			logicalDevice.destroyFence(inFlight);

			logicalDevice.unmapMemory(camPosBuffer.bufferMemory);
			logicalDevice.freeMemory(camPosBuffer.bufferMemory);
			logicalDevice.destroyBuffer(camPosBuffer.buffer);


			logicalDevice.unmapMemory(skyboxUBOBuffer.bufferMemory);
			logicalDevice.freeMemory(skyboxUBOBuffer.bufferMemory);
			logicalDevice.destroyBuffer(skyboxUBOBuffer.buffer);

			logicalDevice.unmapMemory(particleCameraUBOBuffer.bufferMemory);
			logicalDevice.freeMemory(particleCameraUBOBuffer.bufferMemory);
			logicalDevice.destroyBuffer(particleCameraUBOBuffer.buffer);

			logicalDevice.unmapMemory(particleUBOBuffer.bufferMemory);
			logicalDevice.freeMemory(particleUBOBuffer.bufferMemory);
			logicalDevice.destroyBuffer(particleUBOBuffer.buffer);

			logicalDevice.unmapMemory(cameraDataBuffer.bufferMemory);
			logicalDevice.freeMemory(cameraDataBuffer.bufferMemory);
			logicalDevice.destroyBuffer(cameraDataBuffer.buffer);

			logicalDevice.unmapMemory(modelBuffer.bufferMemory);
			logicalDevice.freeMemory(modelBuffer.bufferMemory);
			logicalDevice.destroyBuffer(modelBuffer.buffer);

			logicalDevice.unmapMemory(lightDataBuffer.bufferMemory);
			logicalDevice.freeMemory(lightDataBuffer.bufferMemory);
			logicalDevice.destroyBuffer(lightDataBuffer.buffer);



			logicalDevice.destroyImage(depthBuffer);
			logicalDevice.destroyImage(particledepthBuffer);
			logicalDevice.destroyImage(gbuffer.position.image);
			logicalDevice.destroyImage(gbuffer.normal.image);
			logicalDevice.destroyImage(gbuffer.albedo.image);
			logicalDevice.destroyImage(gbuffer.arm.image);
			logicalDevice.destroyImage(gbuffer.T.image);
			logicalDevice.destroyImage(gbuffer.worldPos.image);
			logicalDevice.destroyImage(particleAttachment.image);
			logicalDevice.destroyImage(shadowMapBuffer.shadowBufferDepthAttachment.image);
			logicalDevice.freeMemory(depthBufferMemory);
			logicalDevice.freeMemory(particledepthBufferMemory);
			logicalDevice.freeMemory(gbuffer.position.mem);
			logicalDevice.freeMemory(gbuffer.normal.mem);
			logicalDevice.freeMemory(gbuffer.albedo.mem);
			logicalDevice.freeMemory(gbuffer.arm.mem);
			logicalDevice.freeMemory(gbuffer.T.mem);
			logicalDevice.freeMemory(gbuffer.worldPos.mem);
			logicalDevice.freeMemory(particleAttachment.mem);
			logicalDevice.freeMemory(shadowMapBuffer.shadowBufferDepthAttachment.mem);
			logicalDevice.destroyImageView(depthBufferView);
			logicalDevice.destroyImageView(particledepthBufferView);
			logicalDevice.destroyImageView(gbuffer.position.view);
			logicalDevice.destroyImageView(gbuffer.normal.view);
			logicalDevice.destroyImageView(gbuffer.albedo.view);
			logicalDevice.destroyImageView(gbuffer.arm.view);
			logicalDevice.destroyImageView(gbuffer.T.view);
			logicalDevice.destroyImageView(gbuffer.worldPos.view);
			logicalDevice.destroyImageView(particleAttachment.view);
			logicalDevice.destroyImageView(shadowMapBuffer.shadowBufferDepthAttachment.view);
			logicalDevice.destroySampler(shadowMapBuffer.sampler);
			
		}




