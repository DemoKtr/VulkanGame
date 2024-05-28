#pragma once
#include "config.h"
#include "Queues.h"

namespace vkInit {

	struct commandBufferInputChunk {
		vk::Device device;
		vk::CommandPool commandPool;
		std::vector<vkUtil::SwapChainFrame>& frames;
	};


	vk::CommandPool make_command_pool(vk::PhysicalDevice physicalDevice, vk::Device device, vk::SurfaceKHR surface, bool debugMode) {
		vkUtil::QueueFamilyIndices queueFamilyIndices = vkUtil::findQueueFamilies(physicalDevice, surface, debugMode);
		vk::CommandPoolCreateInfo poolInfo = {};
		poolInfo.flags = vk::CommandPoolCreateFlags() | vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
		poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

		try {
			return device.createCommandPool(poolInfo);
		}
		catch(vk::SystemError err){
			if(debugMode)
			std::cout<<"Failed Create commandPool" << std::endl;
			return nullptr;
		}
	}

	vk::CommandBuffer make_command_buffer(commandBufferInputChunk inputChunk, bool debugMode) {
		vk::CommandBufferAllocateInfo allocInfo = {};
		allocInfo.commandPool = inputChunk.commandPool;
		allocInfo.level = vk::CommandBufferLevel::ePrimary;
		allocInfo.commandBufferCount = 1;

		try {
			vk::CommandBuffer commandBuffer = inputChunk.device.allocateCommandBuffers(allocInfo)[0];

			if (debugMode) std::cout << "Allocated main command buffer for frame" <<std::endl;
			return commandBuffer;


		}
		catch (vk::SystemError err) {
			std::cout << "FAILED!!! Allocated  main command buffer for frame" << std::endl;
			return nullptr;
		}
	}
	void make_frame_command_buffers(commandBufferInputChunk inputChunk, bool debugMode) {
		vk::CommandBufferAllocateInfo allocInfo = {};
		allocInfo.commandPool = inputChunk.commandPool;
		allocInfo.level = vk::CommandBufferLevel::ePrimary;
		allocInfo.commandBufferCount = 1;

		vk::CommandBufferAllocateInfo secallocInfo = {};
		secallocInfo.commandPool = inputChunk.commandPool;
		secallocInfo.level = vk::CommandBufferLevel::eSecondary;
		secallocInfo.commandBufferCount = 2;

		for (int i = 0; i < inputChunk.frames.size(); ++i) {
			try {
				inputChunk.frames[i].commandBuffer = inputChunk.device.allocateCommandBuffers(allocInfo)[0];
				if (debugMode) std::cout << "Allocated Primary command buffer for frame" << i << std::endl;
				inputChunk.frames[i].shadowCommandBuffer = inputChunk.device.allocateCommandBuffers(secallocInfo)[0];
				if (debugMode) std::cout << "Allocated Shadow command buffer for frame" << i << std::endl;
				inputChunk.frames[i].geometryCommandBuffer = inputChunk.device.allocateCommandBuffers(secallocInfo)[1];
				if (debugMode) std::cout << "Allocated Geometry command buffer for frame" << i << std::endl;
				
			}
			catch (vk::SystemError err) {
				std::cout << "FAILED!!! Allocated command buffer for frame" << i << std::endl;
			}
		}


	}
}
