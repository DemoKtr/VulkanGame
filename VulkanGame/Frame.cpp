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
			input.size = sizeof(UBOCameraParticle);
			particleCameraUBOBuffer = createBuffer(input);
			input.size = sizeof(glm::vec4);
			camPosBuffer  = createBuffer(input);

			input.size = sizeof(SkyBoxUBO);
			skyboxUBOBuffer = createBuffer(input);




			input.size = sizeof(downSacalingUBO);
			input.usage = vk::BufferUsageFlagBits::eUniformBuffer;
			downScaleUBOBuffer = vkUtil::createBuffer(input);
			input.size = sizeof(upSacalingUBO);
			upScaleUBOBuffer = vkUtil::createBuffer(input);


			downScaleUBOWriteLoacation = logicalDevice.mapMemory(downScaleUBOBuffer.bufferMemory, 0, sizeof(downSacalingUBO));

			upScaleUBOWriteLoacation = logicalDevice.mapMemory(upScaleUBOBuffer.bufferMemory, 0, sizeof(upSacalingUBO));


			downsScaleBufferDescriptor.buffer = downScaleUBOBuffer.buffer;
			downsScaleBufferDescriptor.offset = 0;
			downsScaleBufferDescriptor.range = sizeof(downSacalingUBO);


			upScaleUBOBufferDescriptor.buffer = upScaleUBOBuffer.buffer;
			upScaleUBOBufferDescriptor.offset = 0;
			upScaleUBOBufferDescriptor.range = sizeof(upSacalingUBO);

			input.size = sizeof(ssaoUBO);
			ssaoUBOBuffer = createBuffer(input);
			
			cameraDataWriteLocation = logicalDevice.mapMemory(cameraDataBuffer.bufferMemory, 0, sizeof(UBO));
			
			particleCameraUBOWriteLoacation = logicalDevice.mapMemory(particleCameraUBOBuffer.bufferMemory, 0, sizeof(UBOCameraParticle));
			camPosWriteLoacation = logicalDevice.mapMemory(camPosBuffer.bufferMemory, 0, sizeof(glm::vec4));
			skyboxUBOWriteLoacation = logicalDevice.mapMemory(skyboxUBOBuffer.bufferMemory,0,sizeof(SkyBoxUBO));
			ssaoUBOWriteLoacation = logicalDevice.mapMemory(ssaoUBOBuffer.bufferMemory,0,sizeof(ssaoUBO));
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

			ssaoUBOBufferDescriptor.buffer = ssaoUBOBuffer.buffer;
			ssaoUBOBufferDescriptor.offset = 0;
			ssaoUBOBufferDescriptor.range = sizeof(ssaoUBO);

			particleCameraUBOBufferDescriptor.buffer = particleCameraUBOBuffer.buffer;
			particleCameraUBOBufferDescriptor.offset = 0;
			particleCameraUBOBufferDescriptor.range = sizeof(UBOCameraParticle);


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


			vk::SamplerCreateInfo samplerInfo;
			samplerInfo.flags = vk::SamplerCreateFlags();
			samplerInfo.minFilter = vk::Filter::eLinear;
			samplerInfo.magFilter = vk::Filter::eLinear;
			samplerInfo.addressModeU = vk::SamplerAddressMode::eClampToEdge;
			samplerInfo.addressModeV = vk::SamplerAddressMode::eClampToEdge;
			samplerInfo.addressModeW = vk::SamplerAddressMode::eClampToEdge;

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
				upscaleSampler = logicalDevice.createSampler(samplerInfo);
			}
			catch (vk::SystemError err) {
				std::cout << "Failed to make sampler." << std::endl;
			}

		}

void vkUtil::SwapChainFrame::make_animated_descriptor_resources(uint32_t animatorCounter) {
	BufferInputChunk input;
	input.logicalDevice = logicalDevice;
	input.memoryProperties = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent;
	input.physicalDevice = physicalDevice;
	input.size = sizeof(animatedUBO);
	input.usage = vk::BufferUsageFlagBits::eUniformBuffer;
	animationsUBOBuffer = createBuffer(input);
	input.size = sizeof(animatedSBO)*animatorCounter;
	input.usage = vk::BufferUsageFlagBits::eStorageBuffer;
	animationsSBOBuffer = createBuffer(input);

	animationsUBOWriteLoacation = logicalDevice.mapMemory(animationsUBOBuffer.bufferMemory, 0, sizeof(animatedUBO));
	animationsSBOWriteLoacation = logicalDevice.mapMemory(animationsSBOBuffer.bufferMemory, 0, sizeof(animatedSBO)*animatorCounter);


	animationsSBOData.reserve(animatorCounter);


	for (int i = 0; i < animatorCounter; ++i)
	{
		animationsSBOData[i].model = glm::mat4(1.0f);
		for (uint32_t j = 0; j < 100; ++j) {
			animationsSBOData[i].finalBoneMatrices[j] = glm::mat4(1.0f);
		}
	}

	animationsUBOBufferDescriptor.buffer = animationsUBOBuffer.buffer;
	animationsUBOBufferDescriptor.offset = 0;
	animationsUBOBufferDescriptor.range = sizeof(animatedUBO);

	animationsSBOBufferDescriptor.buffer = animationsSBOBuffer.buffer;
	animationsSBOBufferDescriptor.offset = 0;
	animationsSBOBufferDescriptor.range = sizeof(animatedSBO) * animatorCounter;

}

void vkUtil::SwapChainFrame::wirte_samplingdescriptor_set(std::vector<vk::ImageView> mipImagesView,vk::Sampler sampler)
		{
			std::vector<vk::WriteDescriptorSet> writeDescriptorSets = {};

			vk::DescriptorImageInfo firstdownScaleDescriptorSet;
			firstdownScaleDescriptorSet.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
			firstdownScaleDescriptorSet.imageView = gbuffer.normal.view;
			firstdownScaleDescriptorSet.sampler = sampler;
			vk::WriteDescriptorSet firstDownScaleDescriptor;
			firstDownScaleDescriptor.dstSet = downScaleDescriptorsSet[0];
			firstDownScaleDescriptor.dstBinding = 0;
			firstDownScaleDescriptor.dstArrayElement = 0; //byte offset within binding for inline uniform blocks
			firstDownScaleDescriptor.descriptorCount = 1;
			firstDownScaleDescriptor.descriptorType = vk::DescriptorType::eCombinedImageSampler;
			firstDownScaleDescriptor.pImageInfo = &firstdownScaleDescriptorSet;
			writeDescriptorSets.push_back(firstDownScaleDescriptor);
			
			vk::DescriptorImageInfo seccondDownScaleDescriptorSet;
			seccondDownScaleDescriptorSet.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
			seccondDownScaleDescriptorSet.imageView = mipImagesView[0];
			seccondDownScaleDescriptorSet.sampler = sampler;
			vk::WriteDescriptorSet seccondDownScaleDescriptor;
			seccondDownScaleDescriptor.dstSet = downScaleDescriptorsSet[1];
			seccondDownScaleDescriptor.dstBinding = 0;
			seccondDownScaleDescriptor.dstArrayElement = 0; //byte offset within binding for inline uniform blocks
			seccondDownScaleDescriptor.descriptorCount = 1;
			seccondDownScaleDescriptor.descriptorType = vk::DescriptorType::eCombinedImageSampler;
			seccondDownScaleDescriptor.pImageInfo = &seccondDownScaleDescriptorSet;
			writeDescriptorSets.push_back(seccondDownScaleDescriptor);

			vk::DescriptorImageInfo thirdDownScaleDescriptorSet;
			thirdDownScaleDescriptorSet.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
			thirdDownScaleDescriptorSet.imageView = mipImagesView[1];
			thirdDownScaleDescriptorSet.sampler = sampler;
			vk::WriteDescriptorSet thirdDownScaleDescriptor;
			thirdDownScaleDescriptor.dstSet = downScaleDescriptorsSet[2];
			thirdDownScaleDescriptor.dstBinding = 0;
			thirdDownScaleDescriptor.dstArrayElement = 0; //byte offset within binding for inline uniform blocks
			thirdDownScaleDescriptor.descriptorCount = 1;
			thirdDownScaleDescriptor.descriptorType = vk::DescriptorType::eCombinedImageSampler;
			thirdDownScaleDescriptor.pImageInfo = &thirdDownScaleDescriptorSet;
			writeDescriptorSets.push_back(thirdDownScaleDescriptor);

			vk::DescriptorImageInfo forthDownScaleDescriptorSet;
			forthDownScaleDescriptorSet.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
			forthDownScaleDescriptorSet.imageView = mipImagesView[2];
			forthDownScaleDescriptorSet.sampler = sampler;
			vk::WriteDescriptorSet forthDownScaleDescriptor;
			forthDownScaleDescriptor.dstSet = downScaleDescriptorsSet[3];
			forthDownScaleDescriptor.dstBinding = 0;
			forthDownScaleDescriptor.dstArrayElement = 0; //byte offset within binding for inline uniform blocks
			forthDownScaleDescriptor.descriptorCount = 1;
			forthDownScaleDescriptor.descriptorType = vk::DescriptorType::eCombinedImageSampler;
			forthDownScaleDescriptor.pImageInfo = &forthDownScaleDescriptorSet;
			writeDescriptorSets.push_back(forthDownScaleDescriptor);

			vk::DescriptorImageInfo fifthDownScaleDescriptorSet;
			fifthDownScaleDescriptorSet.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
			fifthDownScaleDescriptorSet.imageView = mipImagesView[3];
			fifthDownScaleDescriptorSet.sampler = sampler;
			vk::WriteDescriptorSet fifthDownScaleDescriptor;
			fifthDownScaleDescriptor.dstSet = downScaleDescriptorsSet[4];
			fifthDownScaleDescriptor.dstBinding = 0;
			fifthDownScaleDescriptor.dstArrayElement = 0; //byte offset within binding for inline uniform blocks
			fifthDownScaleDescriptor.descriptorCount = 1;
			fifthDownScaleDescriptor.descriptorType = vk::DescriptorType::eCombinedImageSampler;
			fifthDownScaleDescriptor.pImageInfo = &fifthDownScaleDescriptorSet;
			writeDescriptorSets.push_back(fifthDownScaleDescriptor);

			vk::DescriptorImageInfo sixDownScaleDescriptorSet;
			sixDownScaleDescriptorSet.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
			sixDownScaleDescriptorSet.imageView = mipImagesView[4];
			sixDownScaleDescriptorSet.sampler = sampler;
			vk::WriteDescriptorSet sixDownScaleDescriptor;
			sixDownScaleDescriptor.dstSet = downScaleDescriptorsSet[5];
			sixDownScaleDescriptor.dstBinding = 0;
			sixDownScaleDescriptor.dstArrayElement = 0; //byte offset within binding for inline uniform blocks
			sixDownScaleDescriptor.descriptorCount = 1;
			sixDownScaleDescriptor.descriptorType = vk::DescriptorType::eCombinedImageSampler;
			sixDownScaleDescriptor.pImageInfo = &sixDownScaleDescriptorSet;
			writeDescriptorSets.push_back(sixDownScaleDescriptor);

			vk::DescriptorImageInfo firstUpScaleDescriptorSet;
			firstUpScaleDescriptorSet.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
			firstUpScaleDescriptorSet.imageView = mipImagesView[5];
			firstUpScaleDescriptorSet.sampler = upscaleSampler;
			vk::WriteDescriptorSet firstUpScaleDescriptor;
			firstUpScaleDescriptor.dstSet = upScaleDescriptorsSet[0];
			firstUpScaleDescriptor.dstBinding = 0;
			firstUpScaleDescriptor.dstArrayElement = 0; //byte offset within binding for inline uniform blocks
			firstUpScaleDescriptor.descriptorCount = 1;
			firstUpScaleDescriptor.descriptorType = vk::DescriptorType::eCombinedImageSampler;
			firstUpScaleDescriptor.pImageInfo = &firstUpScaleDescriptorSet;
			writeDescriptorSets.push_back(firstUpScaleDescriptor);

			vk::DescriptorImageInfo seccondUpScaleDescriptorSet;
			seccondUpScaleDescriptorSet.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
			seccondUpScaleDescriptorSet.imageView = mipImagesView[4];
			seccondUpScaleDescriptorSet.sampler = upscaleSampler;
			vk::WriteDescriptorSet seccondUpScaleDescriptor;
			seccondUpScaleDescriptor.dstSet = upScaleDescriptorsSet[1];
			seccondUpScaleDescriptor.dstBinding = 0;
			seccondUpScaleDescriptor.dstArrayElement = 0; //byte offset within binding for inline uniform blocks
			seccondUpScaleDescriptor.descriptorCount = 1;
			seccondUpScaleDescriptor.descriptorType = vk::DescriptorType::eCombinedImageSampler;
			seccondUpScaleDescriptor.pImageInfo = &seccondUpScaleDescriptorSet;
			writeDescriptorSets.push_back(seccondUpScaleDescriptor);

			vk::DescriptorImageInfo thirdUpScaleDescriptorSet;
			thirdUpScaleDescriptorSet.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
			thirdUpScaleDescriptorSet.imageView = mipImagesView[3];
			thirdUpScaleDescriptorSet.sampler = upscaleSampler;
			vk::WriteDescriptorSet thirdUpScaleDescriptor;
			thirdUpScaleDescriptor.dstSet = upScaleDescriptorsSet[2];
			thirdUpScaleDescriptor.dstBinding = 0;
			thirdUpScaleDescriptor.dstArrayElement = 0; //byte offset within binding for inline uniform blocks
			thirdUpScaleDescriptor.descriptorCount = 1;
			thirdUpScaleDescriptor.descriptorType = vk::DescriptorType::eCombinedImageSampler;
			thirdUpScaleDescriptor.pImageInfo = &thirdUpScaleDescriptorSet;
			writeDescriptorSets.push_back(thirdUpScaleDescriptor);

			vk::DescriptorImageInfo forthUpScaleDescriptorSet;
			forthUpScaleDescriptorSet.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
			forthUpScaleDescriptorSet.imageView = mipImagesView[2];
			forthUpScaleDescriptorSet.sampler = upscaleSampler;
			vk::WriteDescriptorSet forthUpScaleDescriptor;
			forthUpScaleDescriptor.dstSet = upScaleDescriptorsSet[3];
			forthUpScaleDescriptor.dstBinding = 0;
			forthUpScaleDescriptor.dstArrayElement = 0; //byte offset within binding for inline uniform blocks
			forthUpScaleDescriptor.descriptorCount = 1;
			forthUpScaleDescriptor.descriptorType = vk::DescriptorType::eCombinedImageSampler;
			forthUpScaleDescriptor.pImageInfo = &forthUpScaleDescriptorSet;
			writeDescriptorSets.push_back(forthUpScaleDescriptor);

			vk::DescriptorImageInfo fifthUpScaleDescriptorSet;
			fifthUpScaleDescriptorSet.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
			fifthUpScaleDescriptorSet.imageView = mipImagesView[1];
			fifthUpScaleDescriptorSet.sampler = upscaleSampler;
			vk::WriteDescriptorSet fifthUpScaleDescriptor;
			fifthUpScaleDescriptor.dstSet = upScaleDescriptorsSet[4];
			fifthUpScaleDescriptor.dstBinding = 0;
			fifthUpScaleDescriptor.dstArrayElement = 0; //byte offset within binding for inline uniform blocks
			fifthUpScaleDescriptor.descriptorCount = 1;
			fifthUpScaleDescriptor.descriptorType = vk::DescriptorType::eCombinedImageSampler;
			fifthUpScaleDescriptor.pImageInfo = &fifthUpScaleDescriptorSet;
			writeDescriptorSets.push_back(fifthUpScaleDescriptor);

			vk::DescriptorImageInfo sixUpScaleDescriptorSet;
			sixUpScaleDescriptorSet.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
			sixUpScaleDescriptorSet.imageView = mipImagesView[0];
			sixUpScaleDescriptorSet.sampler = upscaleSampler;
			vk::WriteDescriptorSet sixUpScaleDescriptor;
			sixUpScaleDescriptor.dstSet = upScaleDescriptorsSet[5];
			sixUpScaleDescriptor.dstBinding = 0;
			sixUpScaleDescriptor.dstArrayElement = 0; //byte offset within binding for inline uniform blocks
			sixUpScaleDescriptor.descriptorCount = 1;
			sixUpScaleDescriptor.descriptorType = vk::DescriptorType::eCombinedImageSampler;
			sixUpScaleDescriptor.pImageInfo = &sixUpScaleDescriptorSet;
			writeDescriptorSets.push_back(sixUpScaleDescriptor);
			
			logicalDevice.updateDescriptorSets(writeDescriptorSets, nullptr);
		}

void vkUtil::SwapChainFrame::wirte_samplingubo_set(glm::vec2 screenSize, std::vector<glm::vec2> mipSize)
		{
			downScaleUBOData.mipLevel = 1;
			downScaleUBOData.srcResolution = screenSize;
			memcpy(downScaleUBOWriteLoacation, &(downScaleUBOData), sizeof(downSacalingUBO));

			vk::WriteDescriptorSet writeInfo;

			writeInfo.dstSet = downScaleDescriptorsSet[0];
			writeInfo.dstBinding = 1;
			writeInfo.dstArrayElement = 0; //byte offset within binding for inline uniform blocks
			writeInfo.descriptorCount = 1;
			writeInfo.descriptorType = vk::DescriptorType::eUniformBuffer;
			writeInfo.pBufferInfo = &downsScaleBufferDescriptor;
			logicalDevice.updateDescriptorSets(writeInfo, nullptr);
			downScaleUBOData.srcResolution = mipSize[0];
			memcpy(downScaleUBOWriteLoacation, &(downScaleUBOData), sizeof(downSacalingUBO));
			writeInfo.dstSet = downScaleDescriptorsSet[1];
			logicalDevice.updateDescriptorSets(writeInfo, nullptr);
			downScaleUBOData.srcResolution = mipSize[1];
			memcpy(downScaleUBOWriteLoacation, &(downScaleUBOData), sizeof(downSacalingUBO));
			writeInfo.dstSet = downScaleDescriptorsSet[2];
			logicalDevice.updateDescriptorSets(writeInfo, nullptr);
			downScaleUBOData.srcResolution = mipSize[2];
			memcpy(downScaleUBOWriteLoacation, &(downScaleUBOData), sizeof(downSacalingUBO));
			writeInfo.dstSet = downScaleDescriptorsSet[3];
			logicalDevice.updateDescriptorSets(writeInfo, nullptr);
			downScaleUBOData.srcResolution = mipSize[3];
			memcpy(downScaleUBOWriteLoacation, &(downScaleUBOData), sizeof(downSacalingUBO));
			writeInfo.dstSet = downScaleDescriptorsSet[4];
			logicalDevice.updateDescriptorSets(writeInfo, nullptr);
			downScaleUBOData.srcResolution = mipSize[4];
			memcpy(downScaleUBOWriteLoacation, &(downScaleUBOData), sizeof(downSacalingUBO));
			writeInfo.dstSet = downScaleDescriptorsSet[5];
			logicalDevice.updateDescriptorSets(writeInfo, nullptr);


			upScaleUBOData.filterRadius = 0.005f;
			memcpy(upScaleUBOWriteLoacation, &(upScaleUBOData), sizeof(upSacalingUBO));
			writeInfo.dstSet = upScaleDescriptorsSet[0];
			writeInfo.pBufferInfo = &upScaleUBOBufferDescriptor;
			logicalDevice.updateDescriptorSets(writeInfo, nullptr);
			writeInfo.dstSet = upScaleDescriptorsSet[1];
			logicalDevice.updateDescriptorSets(writeInfo, nullptr);
			writeInfo.dstSet = upScaleDescriptorsSet[2];
			logicalDevice.updateDescriptorSets(writeInfo, nullptr);
			writeInfo.dstSet = upScaleDescriptorsSet[3];
			logicalDevice.updateDescriptorSets(writeInfo, nullptr);
			writeInfo.dstSet = upScaleDescriptorsSet[4];
			logicalDevice.updateDescriptorSets(writeInfo, nullptr);
			writeInfo.dstSet = upScaleDescriptorsSet[5];
			logicalDevice.updateDescriptorSets(writeInfo, nullptr);
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

void vkUtil::SwapChainFrame::write_animated_descriptor_set(){
	vk::WriteDescriptorSet writeInfo;
	writeInfo.dstSet = animationDescriptorSet;
	writeInfo.dstBinding = 0;
	writeInfo.dstArrayElement = 0; //byte offset within binding for inline uniform blocks
	writeInfo.descriptorCount = 1;
	writeInfo.descriptorType = vk::DescriptorType::eUniformBuffer;
	writeInfo.pBufferInfo = &animationsUBOBufferDescriptor;

	logicalDevice.updateDescriptorSets(writeInfo, nullptr);

	vk::WriteDescriptorSet writeInfo2;
	writeInfo2.dstSet = animationDescriptorSet;
	writeInfo2.dstBinding = 1;
	writeInfo2.dstArrayElement = 0; //byte offset within binding for inline uniform blocks
	writeInfo2.descriptorCount = 1;
	writeInfo2.descriptorType = vk::DescriptorType::eStorageBuffer;
	writeInfo2.pBufferInfo = &animationsSBOBufferDescriptor;

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

			skyboxdepthBuffer = vkImage::make_image(imageInfo);

			particledepthBuffer = vkImage::make_image(imageInfo);
			
			depthBufferMemory = vkImage::make_image_memory(imageInfo, depthBuffer);
			skyboxdepthBufferMemory = vkImage::make_image_memory(imageInfo, skyboxdepthBuffer);
			particledepthBufferMemory = vkImage::make_image_memory(imageInfo, particledepthBuffer);


			depthBufferView = vkImage::make_image_view(
				logicalDevice, depthBuffer, depthFormat, vk::ImageAspectFlagBits::eDepth, vk::ImageViewType::e2D, 1
			);
			skyboxdepthBufferView = vkImage::make_image_view(
				logicalDevice, skyboxdepthBuffer, depthFormat, vk::ImageAspectFlagBits::eDepth, vk::ImageViewType::e2D, 1
			);
			particledepthBufferView = vkImage::make_image_view(
				logicalDevice, particledepthBuffer, depthFormat, vk::ImageAspectFlagBits::eDepth, vk::ImageViewType::e2D, 1
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

void vkUtil::SwapChainFrame::write_skybox_descriptor(std::vector<vk::ImageView>& mipImagesView, vk::Sampler sampler)
{



	vk::WriteDescriptorSet skyBoxWriteInfo;
	skyBoxWriteInfo.dstSet = skyBoxDescriptorSet;
	skyBoxWriteInfo.dstBinding = 0;
	skyBoxWriteInfo.dstArrayElement = 0; //byte offset within binding for inline uniform blocks
	skyBoxWriteInfo.descriptorCount = 1;
	skyBoxWriteInfo.descriptorType = vk::DescriptorType::eUniformBuffer;
	skyBoxWriteInfo.pBufferInfo = &skyboxUBOBufferDescriptor;


	vk::DescriptorImageInfo skyboxInputAttachmentImage;
	skyboxInputAttachmentImage.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
	skyboxInputAttachmentImage.imageView = skyBoxAttachment.view;
	skyboxInputAttachmentImage.sampler = shadowMapBuffer.sampler;
	vk::WriteDescriptorSet skyboxInputDescriptor;
	skyboxInputDescriptor.dstSet = postProcessDescriptorSet;
	skyboxInputDescriptor.dstBinding = 0;
	skyboxInputDescriptor.dstArrayElement = 0; //byte offset within binding for inline uniform blocks
	skyboxInputDescriptor.descriptorCount = 1;
	skyboxInputDescriptor.descriptorType = vk::DescriptorType::eCombinedImageSampler;
	skyboxInputDescriptor.pImageInfo = &skyboxInputAttachmentImage;

	vk::DescriptorImageInfo particleImage;
	particleImage.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
	particleImage.imageView = particleAttachment.view;
	particleImage.sampler = shadowMapBuffer.sampler;
	vk::WriteDescriptorSet particleDescriptor;
	particleDescriptor.dstSet = postProcessDescriptorSet;
	particleDescriptor.dstBinding = 1;
	particleDescriptor.dstArrayElement = 0; //byte offset within binding for inline uniform blocks
	particleDescriptor.descriptorCount = 1;
	particleDescriptor.descriptorType = vk::DescriptorType::eCombinedImageSampler;
	particleDescriptor.pImageInfo = &particleImage;


	vk::DescriptorImageInfo postProcessInputAttachmentImage;
	postProcessInputAttachmentImage.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
	postProcessInputAttachmentImage.imageView = postProcessInputAttachment.view;
	postProcessInputAttachmentImage.sampler = shadowMapBuffer.sampler;
	vk::WriteDescriptorSet postProcessInputDescriptor;
	postProcessInputDescriptor.dstSet = postProcessDescriptorSet;
	postProcessInputDescriptor.dstBinding = 2;
	postProcessInputDescriptor.dstArrayElement = 0; //byte offset within binding for inline uniform blocks
	postProcessInputDescriptor.descriptorCount = 1;
	postProcessInputDescriptor.descriptorType = vk::DescriptorType::eCombinedImageSampler;
	postProcessInputDescriptor.pImageInfo = &postProcessInputAttachmentImage;


	vk::DescriptorImageInfo finalInputAttachmentImage;
	finalInputAttachmentImage.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
	finalInputAttachmentImage.imageView =  gbuffer.normal.view;// sampledAttachment.view;
	finalInputAttachmentImage.sampler = upscaleSampler;
	vk::WriteDescriptorSet finalInputDescriptor;
	finalInputDescriptor.dstSet = finalDescriptorSet;
	finalInputDescriptor.dstBinding = 0;
	finalInputDescriptor.dstArrayElement = 0; //byte offset within binding for inline uniform blocks
	finalInputDescriptor.descriptorCount = 1;
	finalInputDescriptor.descriptorType = vk::DescriptorType::eCombinedImageSampler;
	finalInputDescriptor.pImageInfo = &finalInputAttachmentImage;

	vk::DescriptorImageInfo finalbloomInputAttachmentImage;
	finalbloomInputAttachmentImage.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
	finalbloomInputAttachmentImage.imageView = sampledAttachment.view;// sampledAttachment.view;
	finalbloomInputAttachmentImage.sampler = upscaleSampler;
	vk::WriteDescriptorSet finalbloomInputDescriptor;
	finalbloomInputDescriptor.dstSet = finalDescriptorSet;
	finalbloomInputDescriptor.dstBinding = 1;
	finalbloomInputDescriptor.dstArrayElement = 0; //byte offset within binding for inline uniform blocks
	finalbloomInputDescriptor.descriptorCount = 1;
	finalbloomInputDescriptor.descriptorType = vk::DescriptorType::eCombinedImageSampler;
	finalbloomInputDescriptor.pImageInfo = &finalInputAttachmentImage;

	std::vector<vk::WriteDescriptorSet> writeDescriptorSets = {
				skyBoxWriteInfo,skyboxInputDescriptor ,particleDescriptor, postProcessInputDescriptor, finalInputDescriptor,finalbloomInputDescriptor
	};

	logicalDevice.updateDescriptorSets(writeDescriptorSets, nullptr);
}




void vkUtil::SwapChainFrame::destroy()
		{
			logicalDevice.destroyImageView(imageView);
			logicalDevice.destroyFramebuffer(framebuffer);
			logicalDevice.destroyFramebuffer(animationFramebuffer);
			logicalDevice.destroyFramebuffer(shadowFramebuffer);
			logicalDevice.destroyFramebuffer(particleFramebuffer);
			logicalDevice.destroyFramebuffer(postProcessFramebuffer);
			logicalDevice.destroyFramebuffer(skyBoxFramebuffer);
			logicalDevice.destroyFramebuffer(finalFramebuffer);
			logicalDevice.destroySemaphore(imageAvailable);
			logicalDevice.destroySemaphore(computeFinished);
			logicalDevice.destroySemaphore(renderFinished);
			
			
			for (uint32_t i = 0; i < downscaleFramebuffer.size(); ++i) { logicalDevice.destroyFramebuffer(downscaleFramebuffer[i]); }
			for (uint32_t i = 0; i < upscaleFramebuffer.size(); ++i) { logicalDevice.destroyFramebuffer(upscaleFramebuffer[i]); }

			
			logicalDevice.destroyFence(inFlight);


			logicalDevice.unmapMemory(downScaleUBOBuffer.bufferMemory);
			logicalDevice.freeMemory(downScaleUBOBuffer.bufferMemory);
			logicalDevice.destroyBuffer(downScaleUBOBuffer.buffer);


			logicalDevice.unmapMemory(upScaleUBOBuffer.bufferMemory);
			logicalDevice.freeMemory(upScaleUBOBuffer.bufferMemory);
			logicalDevice.destroyBuffer(upScaleUBOBuffer.buffer);

			logicalDevice.unmapMemory(camPosBuffer.bufferMemory);
			logicalDevice.freeMemory(camPosBuffer.bufferMemory);
			logicalDevice.destroyBuffer(camPosBuffer.buffer);


			logicalDevice.unmapMemory(skyboxUBOBuffer.bufferMemory);
			logicalDevice.freeMemory(skyboxUBOBuffer.bufferMemory);
			logicalDevice.destroyBuffer(skyboxUBOBuffer.buffer);

			logicalDevice.unmapMemory(ssaoUBOBuffer.bufferMemory);
			logicalDevice.freeMemory(ssaoUBOBuffer.bufferMemory);
			logicalDevice.destroyBuffer(ssaoUBOBuffer.buffer);

			logicalDevice.unmapMemory(particleCameraUBOBuffer.bufferMemory);
			logicalDevice.freeMemory(particleCameraUBOBuffer.bufferMemory);
			logicalDevice.destroyBuffer(particleCameraUBOBuffer.buffer);

			logicalDevice.unmapMemory(particleUBOBuffer.bufferMemory);
			logicalDevice.freeMemory(particleUBOBuffer.bufferMemory);
			logicalDevice.destroyBuffer(particleUBOBuffer.buffer);

			logicalDevice.unmapMemory(cameraDataBuffer.bufferMemory);
			logicalDevice.freeMemory(cameraDataBuffer.bufferMemory);
			logicalDevice.destroyBuffer(cameraDataBuffer.buffer);

			logicalDevice.unmapMemory(animationsSBOBuffer.bufferMemory);
			logicalDevice.freeMemory(animationsSBOBuffer.bufferMemory);
			logicalDevice.destroyBuffer(animationsSBOBuffer.buffer);

			logicalDevice.unmapMemory(animationsUBOBuffer.bufferMemory);
			logicalDevice.freeMemory(animationsUBOBuffer.bufferMemory);
			logicalDevice.destroyBuffer(animationsUBOBuffer.buffer);

			logicalDevice.unmapMemory(modelBuffer.bufferMemory);
			logicalDevice.freeMemory(modelBuffer.bufferMemory);
			logicalDevice.destroyBuffer(modelBuffer.buffer);

			logicalDevice.unmapMemory(lightDataBuffer.bufferMemory);
			logicalDevice.freeMemory(lightDataBuffer.bufferMemory);
			logicalDevice.destroyBuffer(lightDataBuffer.buffer);



			logicalDevice.destroyImage(depthBuffer);
			logicalDevice.destroyImage(skyboxdepthBuffer);
			logicalDevice.destroyImage(postProcessInputAttachment.image);
			logicalDevice.destroyImage(particledepthBuffer);
			logicalDevice.destroyImage(gbuffer.position.image);
			logicalDevice.destroyImage(multiSampledattachment.image);
			logicalDevice.destroyImage(gbuffer.normal.image);
			logicalDevice.destroyImage(gbuffer.albedo.image);
			logicalDevice.destroyImage(gbuffer.arm.image);
			logicalDevice.destroyImage(gbuffer.T.image);
			logicalDevice.destroyImage(gbuffer.worldPos.image);
			logicalDevice.destroyImage(particleAttachment.image);
			logicalDevice.destroyImage(skyBoxAttachment.image);
			logicalDevice.destroyImage(sampledAttachment.image);
			logicalDevice.destroyImage(shadowMapBuffer.shadowBufferDepthAttachment.image);
			logicalDevice.freeMemory(depthBufferMemory);
			logicalDevice.freeMemory(skyboxdepthBufferMemory);
			logicalDevice.freeMemory(postProcessInputAttachment.mem);
			logicalDevice.freeMemory(particledepthBufferMemory);
			logicalDevice.freeMemory(gbuffer.position.mem);
			logicalDevice.freeMemory(gbuffer.normal.mem);
			logicalDevice.freeMemory(gbuffer.albedo.mem);
			logicalDevice.freeMemory(gbuffer.arm.mem);
			logicalDevice.freeMemory(gbuffer.T.mem);
			logicalDevice.freeMemory(multiSampledattachment.mem);
			logicalDevice.freeMemory(gbuffer.worldPos.mem);
			logicalDevice.freeMemory(sampledAttachment.mem);
			logicalDevice.freeMemory(particleAttachment.mem);
			logicalDevice.freeMemory(skyBoxAttachment.mem);
			logicalDevice.freeMemory(shadowMapBuffer.shadowBufferDepthAttachment.mem);
			logicalDevice.destroyImageView(depthBufferView);
			logicalDevice.destroyImageView(skyboxdepthBufferView);
			logicalDevice.destroyImageView(postProcessInputAttachment.view);
			logicalDevice.destroyImageView(particledepthBufferView);
			logicalDevice.destroyImageView(gbuffer.position.view);
			logicalDevice.destroyImageView(gbuffer.normal.view);
			logicalDevice.destroyImageView(gbuffer.albedo.view);
			logicalDevice.destroyImageView(multiSampledattachment.view);
			logicalDevice.destroyImageView(gbuffer.arm.view);
			logicalDevice.destroyImageView(gbuffer.T.view);
			logicalDevice.destroyImageView(gbuffer.worldPos.view);
			logicalDevice.destroyImageView(particleAttachment.view);
			logicalDevice.destroyImageView(skyBoxAttachment.view);
			logicalDevice.destroyImageView(sampledAttachment.view);
			logicalDevice.destroyImageView(shadowMapBuffer.shadowBufferDepthAttachment.view);
			logicalDevice.destroySampler(shadowMapBuffer.sampler);
			logicalDevice.destroySampler(upscaleSampler);
			
		}




