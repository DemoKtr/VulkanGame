#include "VertexMenagerie.h"



VertexMenagerie::VertexMenagerie()
{
	indexOffset = 0;

}

VertexMenagerie::~VertexMenagerie()
{

	logicalDevice.destroyBuffer(vertexBuffer.buffer);
	logicalDevice.destroyBuffer(indexBuffer.buffer);
	logicalDevice.freeMemory(vertexBuffer.bufferMemory);
	logicalDevice.freeMemory(indexBuffer.bufferMemory);
}

void VertexMenagerie::consume(meshTypes meshType, std::vector<vkMesh::Vert> data, std::vector<uint32_t> indicies)
{

			int indexCount = static_cast<int>(indicies.size());
			int vertexCount = static_cast<int>(data.size());
			int lastIndex = static_cast<int>(indexLump.size());

			firstIndices.insert(std::make_pair(meshType, lastIndex));
			indexCounts.insert(std::make_pair(meshType, indexCount));
			for (vkMesh::Vert attribute : data) {
				staticVertex.push_back(attribute);
			}
			for (uint32_t index : indicies) {
				indexLump.push_back(index + indexOffset);
			}

			indexOffset += vertexCount;

			
}

void VertexMenagerie::consume(animatedModelTypes meshType, std::vector<vkMesh::Vertex> data, std::vector<uint32_t> indicies)
{

	int indexCount = static_cast<int>(indicies.size());
	int vertexCount = static_cast<int>(data.size());
	int lastIndex = static_cast<int>(indexLump.size());

	AfirstIndices.insert(std::make_pair(meshType, lastIndex));
	AindexCounts.insert(std::make_pair(meshType, indexCount));

	for (vkMesh::Vertex attribute : data) {
		animatedVertex.push_back(attribute);
	}
	for (uint32_t index : indicies) {
		indexLump.push_back(index + indexOffset);
	}

	indexOffset += vertexCount;
	isAnimated = true;
}

void VertexMenagerie::finalize(FinalizationChunk finalizationChunk)
{


	logicalDevice = finalizationChunk.logicalDevice;
	for (vkMesh::Vertex vertex : animatedVertex) {
		vertexLump.push_back(vertex.Position.x);
		vertexLump.push_back(vertex.Position.y);
		vertexLump.push_back(vertex.Position.z);
		vertexLump.push_back(vertex.TexCoords.x);
		vertexLump.push_back(vertex.TexCoords.y);
		vertexLump.push_back(vertex.Normal.x);
		vertexLump.push_back(vertex.Normal.y);
		vertexLump.push_back(vertex.Normal.z);
		vertexLump.push_back(vertex.Tangent.x);
		vertexLump.push_back(vertex.Tangent.y);
		vertexLump.push_back(vertex.Tangent.z);
		vertexLump.push_back(vertex.Bitangent.x);
		vertexLump.push_back(vertex.Bitangent.y);
		vertexLump.push_back(vertex.Bitangent.z);
		vertexLump.push_back(vertex.m_Weights.x);
		vertexLump.push_back(vertex.m_Weights.y);
		vertexLump.push_back(vertex.m_Weights.z);
		vertexLump.push_back(vertex.m_Weights.w);
	}
	
	//make a staging buffer for vertices
	BufferInputChunk inputChunk;
	inputChunk.logicalDevice = finalizationChunk.logicalDevice;
	inputChunk.physicalDevice = finalizationChunk.physicalDevice;
	if (isAnimated)
	inputChunk.size = sizeof(vkMesh::Vertex) * animatedVertex.size();
	else
	inputChunk.size = sizeof(vkMesh::Vert) * staticVertex.size();
	inputChunk.usage = vk::BufferUsageFlagBits::eTransferSrc;
	inputChunk.memoryProperties = vk::MemoryPropertyFlagBits::eHostVisible
		| vk::MemoryPropertyFlagBits::eHostCoherent;
	Buffer stagingBuffer = vkUtil::createBuffer(inputChunk);
	
	//fill it with vertex data
	void* memoryLocation = logicalDevice.mapMemory(stagingBuffer.bufferMemory, 0, inputChunk.size);
	if (isAnimated)
		memcpy(memoryLocation, animatedVertex.data(), inputChunk.size);
	else
	memcpy(memoryLocation, staticVertex.data(), inputChunk.size);
	logicalDevice.unmapMemory(stagingBuffer.bufferMemory);

	//make the vertex buffer
	inputChunk.usage = vk::BufferUsageFlagBits::eTransferDst
		| vk::BufferUsageFlagBits::eVertexBuffer;
	inputChunk.memoryProperties = vk::MemoryPropertyFlagBits::eDeviceLocal;
	vertexBuffer = vkUtil::createBuffer(inputChunk);
	
	//copy to it
	vkUtil::copyBuffer(
		stagingBuffer, vertexBuffer, inputChunk.size,
		finalizationChunk.queue, finalizationChunk.commandBuffer
	);
	




	//destroy staging buffer
	logicalDevice.destroyBuffer(stagingBuffer.buffer);
	logicalDevice.freeMemory(stagingBuffer.bufferMemory);
	
	//make a staging buffer for indices
	inputChunk.size = sizeof(uint32_t) * indexLump.size();
	
	inputChunk.usage = vk::BufferUsageFlagBits::eTransferSrc;
	
	inputChunk.memoryProperties = vk::MemoryPropertyFlagBits::eHostVisible
		| vk::MemoryPropertyFlagBits::eHostCoherent;
	stagingBuffer = vkUtil::createBuffer(inputChunk);
	
	//fill it with index data
	memoryLocation = logicalDevice.mapMemory(stagingBuffer.bufferMemory, 0, inputChunk.size);
	memcpy(memoryLocation, indexLump.data(), inputChunk.size);
	logicalDevice.unmapMemory(stagingBuffer.bufferMemory);
	
	//make the vertex buffer
	inputChunk.usage = vk::BufferUsageFlagBits::eTransferDst
		| vk::BufferUsageFlagBits::eIndexBuffer;
	inputChunk.memoryProperties = vk::MemoryPropertyFlagBits::eDeviceLocal;
	indexBuffer = vkUtil::createBuffer(inputChunk);

	//copy to it
	vkUtil::copyBuffer(
		stagingBuffer, indexBuffer, inputChunk.size,
		finalizationChunk.queue, finalizationChunk.commandBuffer
	);
	
	//destroy staging buffer
	logicalDevice.destroyBuffer(stagingBuffer.buffer);
	logicalDevice.freeMemory(stagingBuffer.bufferMemory);
}


