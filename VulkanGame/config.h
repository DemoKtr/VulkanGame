#pragma once
#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <optional>
#include <fstream>
typedef glm::tvec2<int> ivec2;
struct Buffer {
	vk::Buffer buffer;
	vk::DeviceMemory bufferMemory;
};

struct BufferInputChunk {
	size_t size;
	vk::BufferUsageFlags usage;
	vk::Device logicalDevice;
	vk::PhysicalDevice physicalDevice;
	vk::MemoryPropertyFlags memoryProperties;
};




enum class meshTypes {
	KITTY,
	DOG
};

std::vector<std::string> split(std::string line, std::string delimiter);