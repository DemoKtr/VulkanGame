#include "Particle.h"
#include <chrono>


void vkParticle::ParticleMenagerie::prepareStorageBuffer(FinalizationChunk finalizationChunk)
{
	std::default_random_engine randomizer(benchmark ? 0 : (unsigned)time(nullptr));
	std::uniform_real_distribution<float> rndDist(-1.0f, 1.0f);
	std::vector<Particle> particles(particleCount);
	for (auto& particle : particles) {
		particle.pos = glm::vec2(rndDist(randomizer), rndDist(randomizer));
		particle.vel = glm::vec2(0.0f);
		particle.gradientPos.x = particle.pos.x / 2.0f;
	}
	vk::DeviceSize storageBufferSize = particles.size() * sizeof(Particle);

	
	BufferInputChunk inputChunk;
	inputChunk.logicalDevice = logicalDevice;
		inputChunk.physicalDevice = physicalDevice;
	inputChunk.size = particles.size() * sizeof(Particle);
	inputChunk.usage = vk::BufferUsageFlagBits::eTransferSrc;
	inputChunk.memoryProperties = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent;
	Buffer stagingBuffer = vkUtil::createBuffer(inputChunk);

	//fill it with vertex data
	void* memoryLocation = logicalDevice.mapMemory(stagingBuffer.bufferMemory, 0, inputChunk.size);
	memcpy(memoryLocation, particles.data(), inputChunk.size);
	logicalDevice.unmapMemory(stagingBuffer.bufferMemory);

	//make the vertex buffer

	inputChunk.usage = vk::BufferUsageFlagBits::eTransferDst
		| vk::BufferUsageFlagBits::eVertexBuffer;
	inputChunk.memoryProperties = vk::MemoryPropertyFlagBits::eDeviceLocal;
	particleBuffer = vkUtil::createBuffer(inputChunk);

	//copy to it
	vkUtil::copyBuffer(
		stagingBuffer, particleBuffer, inputChunk.size,
		finalizationChunk.queue, finalizationChunk.commandBuffer
	);

	//destroy staging buffer
	logicalDevice.destroyBuffer(stagingBuffer.buffer);
	logicalDevice.freeMemory(stagingBuffer.bufferMemory);




}


vk::VertexInputBindingDescription vkParticle::getParticleVBO(){


		/* Provided by VK_VERSION_1_0
		typedef struct VkVertexInputBindingDescription {
			uint32_t             binding;
			uint32_t             stride; // Ile miejsca zaj¹æ pomiêdzy Vertices
			VkVertexInputRate    inputRate;
		} VkVertexInputBindingDescription;
		*/

		vk::VertexInputBindingDescription bindingDescription;
		bindingDescription.binding = 0;
		// xy rgb uv
		bindingDescription.stride = 6*sizeof(float);
		bindingDescription.inputRate = vk::VertexInputRate::eVertex; // einstance do instancjonowania
		return bindingDescription;
		}



std::vector<vk::VertexInputAttributeDescription> vkParticle::getParticleVAO()
{
	/* Provided by VK_VERSION_1_0
		typedef struct VkVertexInputAttributeDescription {
			uint32_t    location;
			uint32_t    binding;
			VkFormat    format;
			uint32_t    offset;
		} VkVertexInputAttributeDescription;
		*/
	std::vector<vk::VertexInputAttributeDescription> attributes;
	vk::VertexInputAttributeDescription dummy;
	attributes.push_back(dummy);
	attributes.push_back(dummy);


	//Pos
	attributes[0].binding = 0;
	attributes[0].location = 0;
	attributes[0].format = vk::Format::eR32G32Sfloat;
	attributes[0].offset = 0;

	//vertexColor
	attributes[1].binding = 0;
	attributes[1].location = 1;
	attributes[1].format = vk::Format::eR32G32B32A32Sfloat;
	attributes[1].offset = 4 * sizeof(float);

	return attributes;
}
