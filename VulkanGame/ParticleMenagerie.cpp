#include "ParticleMenagerie.h"
#include <random>


ParticleMenagerie::ParticleMenagerie()
{
}

ParticleMenagerie::~ParticleMenagerie()
{
	logicalDevice.freeMemory(particleBuffer.bufferMemory);
	logicalDevice.destroyBuffer(particleBuffer.buffer);
}


void ParticleMenagerie::finalization(FinalizationChunk finalizationChunk)
{
	logicalDevice = finalizationChunk.logicalDevice;
	std::default_random_engine randomizer(benchmark ? 0 : (unsigned)time(nullptr));
	std::uniform_real_distribution<float> rndDistX(-1.75f, 1.65f);
	std::uniform_real_distribution<float> rndDistZ(-0.05f, 0.05f);
	std::uniform_real_distribution<float> rndDistY(-0.10f, 0.0f);
	std::uniform_real_distribution<float> rndVelDistX(-0.01f, 0.01f);
	
	//std::uniform_real_distribution<float> rndVelDistX(-0.25f, 0.25f);
	std::uniform_real_distribution<float> rndVelDistZ(-0.05f, 0.05f);
	std::uniform_real_distribution<float> rndVelDistY(0.01f,0.05f);
	std::uniform_real_distribution<float> rndLifetimeDist(0.2f, 2.5f);
	std::vector<vkParticle::Particle> particles(numberOfEmiter * burstParticleCount);
	for (auto& particle : particles) {
		particle.pos = glm::vec4(rndDistX(randomizer), rndDistY(randomizer), rndVelDistX(randomizer), rndVelDistY(randomizer));
		particle.gradientPos.x = 1.0f;// particle.pos.x / 2.0f;
		particle.gradientPos.y = 1.0f;// particle.pos.x / 2.0f;
		particle.gradientPos.z = rndDistZ(randomizer);// particle.pos.x / 2.0f;
		particle.gradientPos.w = particle.gradientPos.z;// particle.pos.x / 2.0f;
		particle.initialPos = particle.pos;
		particle.initialPos.z = rndLifetimeDist(randomizer);
		particle.initialPos.w = particle.initialPos.z;
		particle.acc = glm::vec4(1.0f);
	}
	particleVertexData.reserve(particles.size() * 16);
	for (auto& particle : particles) {
		particleVertexData.push_back(particle.pos.x);
		particleVertexData.push_back(particle.pos.y);
		particleVertexData.push_back(particle.pos.z);
		particleVertexData.push_back(particle.pos.w);

		particleVertexData.push_back(particle.gradientPos.x);
		particleVertexData.push_back(particle.gradientPos.y);
		particleVertexData.push_back(particle.gradientPos.z);
		particleVertexData.push_back(particle.gradientPos.w);

		particleVertexData.push_back(particle.initialPos.x);
		particleVertexData.push_back(particle.initialPos.y);
		particleVertexData.push_back(particle.initialPos.z);
		particleVertexData.push_back(particle.initialPos.w);

		particleVertexData.push_back(particle.acc.x);
		particleVertexData.push_back(particle.acc.y);
		particleVertexData.push_back(particle.acc.z);
		particleVertexData.push_back(particle.acc.w);
	}


	vk::DeviceSize storageBufferSize = particleVertexData.size() * sizeof(float);
	size = storageBufferSize;

	BufferInputChunk inputChunk;
	inputChunk.logicalDevice = logicalDevice;
	inputChunk.physicalDevice = finalizationChunk.physicalDevice;
	inputChunk.size = storageBufferSize;
	inputChunk.usage = vk::BufferUsageFlagBits::eTransferSrc;
	inputChunk.memoryProperties = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent;
	Buffer stagingBuffer = vkUtil::createBuffer(inputChunk);

	//fill it with vertex data
	particleWriteLoacation = logicalDevice.mapMemory(stagingBuffer.bufferMemory, 0, storageBufferSize);
	memcpy(particleWriteLoacation, particleVertexData.data(), storageBufferSize);

	logicalDevice.unmapMemory(stagingBuffer.bufferMemory);

	//make the vertex buffer

	inputChunk.usage = vk::BufferUsageFlagBits::eTransferDst
		| vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eStorageBuffer;
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

void ParticleMenagerie::consume()
{
	numberOfEmiter += 1;
}

void ParticleMenagerie::make_descriptors_resources()
{
	

	particleData.reserve(numberOfEmiter * burstParticleCount);


	for (int i = 0; i < numberOfEmiter * burstParticleCount; ++i)
	{
		particleData.push_back(vkParticle::Particle());
	}


	/*
	typedef struct VkDescriptorBufferInfo {
		VkBuffer        buffer;
		VkDeviceSize    offset;
		VkDeviceSize    range;
	} VkDescriptorBufferInfo; =
	*/
	particleBufferDescriptor.buffer = particleBuffer.buffer;
	particleBufferDescriptor.offset = 0;
	particleBufferDescriptor.range = numberOfEmiter*burstParticleCount*sizeof(vkParticle::Particle);


}

vk::DeviceSize ParticleMenagerie::getBufferSize()
{
	return size;
}
