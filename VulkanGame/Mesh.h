#pragma once
#include "config.h"

namespace vkMesh {
	vk::VertexInputBindingDescription getPosColBindingDescription() {

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
		bindingDescription.stride = 8 * sizeof(float);
		bindingDescription.inputRate = vk::VertexInputRate::eVertex; // einstance do instancjonowania

		return bindingDescription;
	}


	std::vector<vk::VertexInputAttributeDescription> getPosColorAttributeDescriptions() {

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
		attributes.push_back(dummy);

		//Pos
		attributes[0].binding = 0;
		attributes[0].location = 0;
		attributes[0].format = vk::Format::eR32G32B32Sfloat;
		attributes[0].offset = 0;

		//Color
		attributes[1].binding = 0;
		attributes[1].location = 1;
		attributes[1].format = vk::Format::eR32G32B32Sfloat;
		attributes[1].offset = 3 * sizeof(float);

		//TexCoord
		attributes[2].binding = 0;
		attributes[2].location = 2;
		attributes[2].format = vk::Format::eR32G32Sfloat;
		attributes[2].offset = 6 * sizeof(float);
		return attributes;
	}
}