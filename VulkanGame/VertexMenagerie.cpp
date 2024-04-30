#include "VertexMenagerie.h"

VertexMenagerie::VertexMenagerie()
{
	offset = 0;
}

VertexMenagerie::~VertexMenagerie()
{

	logicalDevice.destroyBuffer(vertexBuffer.buffer);
	logicalDevice.freeMemory(vertexBuffer.bufferMemory);
}

void VertexMenagerie::consume(meshTypes meshType, std::vector<float> data)
{
	for (float attribute : data) {
		lump.push_back(attribute);
	}
	int vertexCount = static_cast<int>(data.size() / 5);
	offsets.insert(std::make_pair(meshType, offset));
	sizes.insert(std::make_pair(meshType, vertexCount));
	offset += vertexCount;
}

void VertexMenagerie::finalize(vk::Device logicalDevice, vk::PhysicalDevice  physicalDevice)
{

	this->logicalDevice = logicalDevice;



	BufferInputChunk inputChunk;
	inputChunk.logicalDevice = logicalDevice;
	inputChunk.physicalDevice = physicalDevice;
	inputChunk.size = sizeof(float) * lump.size();
	inputChunk.usage = vk::BufferUsageFlagBits::eVertexBuffer;

	vertexBuffer = vkUtil::createBuffer(inputChunk);

	void* memoryLocation = logicalDevice.mapMemory(vertexBuffer.bufferMemory, 0, inputChunk.size);
	memcpy(memoryLocation, lump.data(), inputChunk.size);
	logicalDevice.unmapMemory(vertexBuffer.bufferMemory);
}
