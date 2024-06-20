#include "ParticleEmiter.h"

ParticleEmiter::ParticleEmiter(glm::vec3 position)
{
	this->transform.setLocalPosition(position);
	this->transform.computeModelMatrix();
}
