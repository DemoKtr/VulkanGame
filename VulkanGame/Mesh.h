#pragma once
#include "config.h"
#include "AnimatedMesh.h"
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
		bindingDescription.stride = 14 * sizeof(float);
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
		attributes.push_back(dummy);
		attributes.push_back(dummy);


		//Pos
		attributes[0].binding = 0;
		attributes[0].location = 0;
		attributes[0].format = vk::Format::eR32G32B32Sfloat;
		attributes[0].offset = 0;

		//vertexColor
		attributes[1].binding = 0;
		attributes[1].location = 1;
		attributes[1].format = vk::Format::eR32G32B32Sfloat;
		attributes[1].offset = 3 * sizeof(float);

		//TexCoord
		attributes[2].binding = 0;
		attributes[2].location = 2;
		attributes[2].format = vk::Format::eR32G32Sfloat;
		attributes[2].offset = 6 * sizeof(float);
		

		//Normal
		attributes[3].binding = 0;
		attributes[3].location = 3;
		attributes[3].format = vk::Format::eR32G32B32Sfloat;
		attributes[3].offset = 8 * sizeof(float);

		//tangent
		attributes[4].binding = 0;
		attributes[4].location = 4;
		attributes[4].format = vk::Format::eR32G32B32Sfloat;
		attributes[4].offset = 11 * sizeof(float);


		return attributes;
	}

	vk::VertexInputBindingDescription getAnimPosColBindingDescription() {

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
		bindingDescription.stride =  sizeof(vkMesh::Vertex);
		bindingDescription.inputRate = vk::VertexInputRate::eVertex; // einstance do instancjonowania

		return bindingDescription;
	}


	std::vector<vk::VertexInputAttributeDescription> getAnimPosColorAttributeDescriptions() {

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
		attributes.push_back(dummy);
		attributes.push_back(dummy);
		attributes.push_back(dummy);
		attributes.push_back(dummy);


		//Pos
		attributes[0].binding = 0;
		attributes[0].location = 0;
		attributes[0].format = vk::Format::eR32G32B32Sfloat;
		attributes[0].offset = 0;

		//normal
		attributes[1].binding = 0;
		attributes[1].location = 1;
		attributes[1].format = vk::Format::eR32G32B32Sfloat;
		attributes[1].offset = 3 * sizeof(float);

		//TexCoord
		attributes[2].binding = 0;
		attributes[2].location = 2;
		attributes[2].format = vk::Format::eR32G32Sfloat;
		attributes[2].offset = 6 * sizeof(float);


		//Tangent
		attributes[3].binding = 0;
		attributes[3].location = 3;
		attributes[3].format = vk::Format::eR32G32B32Sfloat;
		attributes[3].offset = 8 * sizeof(float);

		//BTangent
		attributes[4].binding = 0;
		attributes[4].location = 4;
		attributes[4].format = vk::Format::eR32G32B32Sfloat;
		attributes[4].offset = 11 * sizeof(float);

		//boneID
		attributes[5].binding = 0;
		attributes[5].location = 4;
		attributes[5].format = vk::Format::eR32G32B32A32Sint;
		attributes[5].offset = 14 * sizeof(float);

		//weights
		attributes[6].binding = 0;
		attributes[6].location = 4;
		attributes[6].format = vk::Format::eR32G32B32A32Sfloat;
		attributes[6].offset = 14 * sizeof(float) + 4 * sizeof(int);


		return attributes;
	}
}
