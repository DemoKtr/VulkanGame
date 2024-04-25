#pragma once
#include "config.h"
#include "Shaders.h"
namespace vkInit {
	struct GraphicsPipelineInBundle {
		vk::Device device;
		std::string vertexFilePath;
	};
	struct GraphicsPipelineOutBundle {

	};

    GraphicsPipelineOutBundle create_graphic_pipeline(GraphicsPipelineInBundle specyfication, bool debugMode) {
        vk::GraphicsPipelineCreateInfo pipelineInfo = {  };
        pipelineInfo.flags = vk::PipelineCreateFlags();

        std::vector <vk::PipelineShaderStageCreateInfo> shaderStages;

        //Vertex Input
        vk::PipelineVertexInputStateCreateInfo vertexInputInfo = {};
        vertexInputInfo.flags = vk::PipelineVertexInputStateCreateFlags();
        vertexInputInfo.vertexBindingDescriptionCount = 0;
        vertexInputInfo.vertexAttributeDescriptionCount = 0;
        pipelineInfo.pVertexInputState = &vertexInputInfo;

        //Input Assembly
        vk::PipelineInputAssemblyStateCreateInfo inputAssemblyInfo = {};
        inputAssemblyInfo.flags = vk::PipelineInputAssemblyStateCreateFlags();
        inputAssemblyInfo.topology = vk::PrimitiveTopology::eTriangleList; //duzo opcji
        pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;

        //Vertex Shader
        if (debugMode) {
            std::cout << "Create Vertex Shader " << std::endl;
        }
        vk::ShaderModule vertexShader = vkUtil::createModule(specyfication.vertexFilePath, specyfication.device, debugMode);
        vk::PipelineShaderStageCreateInfo vertexShaderInfo = {  };
        vertexShaderInfo.module = vertexShader;
        vertexShaderInfo.pName = "main";
        shaderStages.push_back(vertexShaderInfo);


        GraphicsPipelineOutBundle output{};
        return output;
	}
}

