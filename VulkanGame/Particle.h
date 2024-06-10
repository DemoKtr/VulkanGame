#pragma once
#include "config.h"
#include <random>
#include "Memory.h"
#include "VertexMenagerie.h"
namespace vkParticle {
	struct Particle
	{
		glm::vec2 pos;								// Particle position
		glm::vec2 vel;								// Particle velocity
		glm::vec4 gradientPos;

	};
	vk::VertexInputBindingDescription getParticleVBO();
	std::vector<vk::VertexInputAttributeDescription> getParticleVAO();

	class ParticleMenagerie {
		vk::Device logicalDevice;
		vk::PhysicalDevice physicalDevice;

		bool benchmark = true;
		int particleCount = 256 * 1024;
		Buffer particleBuffer;
		vk::DescriptorSet particleDescriptorSet;
		void prepareStorageBuffer(FinalizationChunk finalizationChunk);
	};
}

