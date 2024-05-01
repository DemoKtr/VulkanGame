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
	int vertexCount = static_cast<int>(data.size() / 7);
	offsets.insert(std::make_pair(meshType, offset));
	sizes.insert(std::make_pair(meshType, vertexCount));
	offset += vertexCount;
}

void VertexMenagerie::finalize(FinalizationChunk finalizationChunk)
{

	this->logicalDevice = finalizationChunk.logicalDevice;



	BufferInputChunk inputChunk;
	inputChunk.logicalDevice = finalizationChunk.logicalDevice;
	inputChunk.physicalDevice = finalizationChunk.physicalDevice;
	inputChunk.size = sizeof(float) * lump.size();
	inputChunk.usage = vk::BufferUsageFlagBits::eTransferSrc;
	inputChunk.memoryProperties = vk::MemoryPropertyFlagBits ::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent;

	Buffer stagingBuffer= vkUtil::createBuffer(inputChunk);


	void* memoryLocation = logicalDevice.mapMemory(stagingBuffer.bufferMemory, 0, inputChunk.size);
	memcpy(memoryLocation, lump.data(), inputChunk.size);
	logicalDevice.unmapMemory(stagingBuffer.bufferMemory);



	inputChunk.usage = vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer;
	inputChunk.memoryProperties = vk::MemoryPropertyFlagBits::eDeviceLocal;

	vertexBuffer = vkUtil::createBuffer(inputChunk);
	vkUtil::copyBuffer(stagingBuffer, vertexBuffer, inputChunk.size, finalizationChunk.queue, finalizationChunk.commandBuffer);
	logicalDevice.destroyBuffer(stagingBuffer.buffer);
	logicalDevice.freeMemory(stagingBuffer.bufferMemory);
}
