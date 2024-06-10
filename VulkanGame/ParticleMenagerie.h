#pragma once
#include "config.h"
#include "VertexMenagerie.h"

class ParticleMenagerie {
public:
	ParticleMenagerie();
	~ParticleMenagerie();

	vk::Device logicalDevice;
	bool benchmark = true;
	int burstParticleCount = 1024;
	int numberOfEmiter = { 0 };
	Buffer particleBuffer;
	void finalization(FinalizationChunk finalizationChunk);
	void consume();
};

