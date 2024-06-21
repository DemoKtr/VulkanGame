#pragma once
#include "config.h"

namespace vkMesh {

    struct  FinalizationChunk {
        vk::Device logicalDevice;
        vk::PhysicalDevice physicalDevice;
        vk::Queue queue;
        vk::CommandBuffer commandBuffer;

    };

	vk::VertexInputBindingDescription getMeshVBO();
	std::vector<vk::VertexInputAttributeDescription> getMeshVAO();
   
}

class CubeMapMesh
{
	vk::Device logicalDevice;
    
public:
	CubeMapMesh();
	~CubeMapMesh();
	void finalize(vkMesh::FinalizationChunk finalizationChink);
	Buffer vertexBuffer;


};