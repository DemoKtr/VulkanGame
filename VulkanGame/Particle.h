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
		glm::vec2 initialPos;
		float lifeTime;
		float currentLifeTime;

	};
	vk::VertexInputBindingDescription getParticleVBO();
	std::vector<vk::VertexInputAttributeDescription> getParticleVAO();

	
}

