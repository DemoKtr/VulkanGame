#pragma once
#include "config.h"
#include "VertexMenagerie.h"
#include "Particle.h"
class ParticleMenagerie {
public:
	ParticleMenagerie();
	~ParticleMenagerie();

	vk::DescriptorBufferInfo particleBufferDescriptor;
	void* particleWriteLoacation;
	std::vector<vkParticle::Particle> particleData;
	vk::Device logicalDevice;
	bool benchmark = true;
	int burstParticleCount = 1024* 256;
	int numberOfEmiter = { 0 };
	Buffer particleBuffer;
	void finalization(FinalizationChunk finalizationChunk);
	void consume();
	void make_descriptors_resources();
	vk::DeviceSize getBufferSize();
	vk::DeviceSize size;
};

