#include "Particle.h"
#include <chrono>





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
		bindingDescription.stride = sizeof(Particle);
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
	attributes[0].format = vk::Format::eR32G32B32A32Sfloat;
	attributes[0].offset = 0;


	//gradient
	attributes[1].binding = 0;
	attributes[1].location = 1;
	attributes[1].format = vk::Format::eR32G32B32A32Sfloat;
	attributes[1].offset = sizeof(glm::vec4);




	return attributes;
}
