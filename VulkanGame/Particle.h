#pragma once
#include "config.h"
#include <random>
#include "Memory.h"
#include "VertexMenagerie.h"
namespace vkParticle {
	struct Particle
	{
		glm::vec4 pos;								// Particle position								// Particle velocity
		glm::vec4 gradientPos;
		glm::vec4 initialPos;
		glm::vec4 acc;

	};
	vk::VertexInputBindingDescription getParticleVBO();
	std::vector<vk::VertexInputAttributeDescription> getParticleVAO();

	
}

