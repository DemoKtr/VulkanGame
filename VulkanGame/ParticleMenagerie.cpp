#include "ParticleMenagerie.h"
#include <random>
#include "Particle.h"

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
	std::uniform_real_distribution<float> rndDist(-1.0f, 1.0f);
	std::vector<vkParticle::Particle> particles(numberOfEmiter * burstParticleCount);
	for (auto& particle : particles) {
		particle.pos = glm::vec3(rndDist(randomizer), rndDist(randomizer), rndDist(randomizer));
		particle.vel = glm::vec3(0.0f);
		particle.gradientPos.x = particle.pos.x / 2.0f;
	}
	vk::DeviceSize storageBufferSize = particles.size() * sizeof(vkParticle::Particle);


	BufferInputChunk inputChunk;
	inputChunk.logicalDevice = logicalDevice;
	inputChunk.physicalDevice = finalizationChunk.physicalDevice;
	inputChunk.size = particles.size() * sizeof(vkParticle::Particle);
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

void ParticleMenagerie::consume()
{
	numberOfEmiter += 1;
}