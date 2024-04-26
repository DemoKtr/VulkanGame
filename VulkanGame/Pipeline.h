#pragma once
#include "config.h"
#include "Shaders.h"
#include "RenderStruct.h"
namespace vkInit {
	struct GraphicsPipelineInBundle {
		vk::Device device;
		std::string vertexFilePath;
		std::string fragmentFilePath;
        vk::Extent2D swapchainExtent;
        vk::Format swapchainImageFormat;
	};
	struct GraphicsPipelineOutBundle {
        vk::PipelineLayout layout;
        vk::RenderPass renderpass;
        vk::Pipeline graphicsPipeline;
	};
    vk::PipelineLayout create_pipeline_layout(vk::Device device, bool debugMode) {
        vk::PipelineLayoutCreateInfo layoutInfo;
        layoutInfo.flags = vk::PipelineLayoutCreateFlags();
        layoutInfo.setLayoutCount = 0;
        layoutInfo.pushConstantRangeCount =1;
        vk::PushConstantRange pushConstantInfo;
        pushConstantInfo.offset = 0;
        pushConstantInfo.size = sizeof(vkUtil::ObjectData);
        pushConstantInfo.stageFlags = vk::ShaderStageFlagBits::eVertex;
        layoutInfo.pPushConstantRanges = &pushConstantInfo;

        try {
            return device.createPipelineLayout(layoutInfo);
        }
        catch (vk::SystemError err) {
            if (debugMode) std::cout << "Failed Create pipeline Layout" << std::endl;
        }
    }
    
    vk::RenderPass create_renderpass(vk::Device device, vk::Format swapchainImageFormat, bool debugMode) {
        //Define a general attachment, with its load/store operations
        vk::AttachmentDescription colorAttachment = {};
        colorAttachment.flags = vk::AttachmentDescriptionFlags();
        colorAttachment.format = swapchainImageFormat;
        colorAttachment.samples = vk::SampleCountFlagBits::e1;
        colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
        colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
        colorAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
        colorAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
        colorAttachment.initialLayout = vk::ImageLayout::eUndefined;
        colorAttachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;

        //Declare that attachment to be color buffer 0 of the framebuffer
        vk::AttachmentReference colorAttachmentRef = {};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal;

        //Renderpasses are broken down into subpasses, there's always at least one.
        vk::SubpassDescription subpass = {};
        subpass.flags = vk::SubpassDescriptionFlags();
        subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;

        //Now create the renderpass
        vk::RenderPassCreateInfo renderpassInfo = {};
        renderpassInfo.flags = vk::RenderPassCreateFlags();
        renderpassInfo.attachmentCount = 1;
        renderpassInfo.pAttachments = &colorAttachment;
        renderpassInfo.subpassCount = 1;
        renderpassInfo.pSubpasses = &subpass;
        try {
            return device.createRenderPass(renderpassInfo);
        }
        catch (vk::SystemError err) {
            if (debugMode) {
                std::cout << "Failed to create renderpass!" << std::endl;
            }
        }
    }


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
        vertexShaderInfo.flags = vk::PipelineShaderStageCreateFlags();
        vertexShaderInfo.stage = vk::ShaderStageFlagBits::eVertex;
        vertexShaderInfo.module = vertexShader;
        vertexShaderInfo.pName = "main";
        shaderStages.push_back(vertexShaderInfo);


        //Viewport & Scissor
        vk::Viewport viewport = {};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = specyfication.swapchainExtent.width;
        viewport.height = specyfication.swapchainExtent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        vk::Rect2D scissor = {};
        scissor.offset.x = 0.0f;
        scissor.offset.y = 0.0f;
        scissor.extent = specyfication.swapchainExtent;
        vk::PipelineViewportStateCreateInfo viewportState = {};
        viewportState.flags = vk::PipelineViewportStateCreateFlags();
        viewportState.viewportCount = 1;
        viewportState.pViewports = &viewport;
        viewportState.scissorCount = 1;
        viewportState.pScissors = &scissor;
        pipelineInfo.pViewportState = &viewportState;

        //rasterizer
        vk::PipelineRasterizationStateCreateInfo rasterizer = {};
        rasterizer.flags = vk::PipelineRasterizationStateCreateFlags();
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = vk::PolygonMode::eFill;
        rasterizer.lineWidth = 1.0f;
        rasterizer.cullMode = vk::CullModeFlagBits::eBack;
        rasterizer.frontFace = vk::FrontFace::eClockwise;
        rasterizer.depthBiasEnable = VK_FALSE;
        pipelineInfo.pRasterizationState = &rasterizer;



        //Fragment Shader
        if (debugMode) {
            std::cout << "Create Fragment Shader " << std::endl;
        }
        vk::ShaderModule fragmentShader = vkUtil::createModule(specyfication.fragmentFilePath, specyfication.device, debugMode);
        vk::PipelineShaderStageCreateInfo fragmentShaderInfo = {};
        fragmentShaderInfo.flags = vk::PipelineShaderStageCreateFlags();
        fragmentShaderInfo.stage = vk::ShaderStageFlagBits::eFragment;
        fragmentShaderInfo.module = fragmentShader;
        fragmentShaderInfo.pName = "main";
        shaderStages.push_back(fragmentShaderInfo);
        //Now both shaders have been made, we can declare them to the pipeline info
        pipelineInfo.stageCount = shaderStages.size();
        pipelineInfo.pStages = shaderStages.data();

        //MultiSampling
        vk::PipelineMultisampleStateCreateInfo multisampling = {};
        multisampling.flags = vk::PipelineMultisampleStateCreateFlags();
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = vk::SampleCountFlagBits::e1;
        pipelineInfo.pMultisampleState = &multisampling;

        //color Blending
        vk::PipelineColorBlendAttachmentState colorBlendAttachment = {};
        colorBlendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
        colorBlendAttachment.blendEnable = VK_FALSE;
        vk::PipelineColorBlendStateCreateInfo colorBlending = {  };
        colorBlending.flags = vk::PipelineColorBlendStateCreateFlags();
        colorBlending.logicOp = vk::LogicOp::eCopy;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;
        colorBlending.blendConstants[0] = 0.0f;
        colorBlending.blendConstants[1] = 0.0f;
        colorBlending.blendConstants[2] = 0.0f;
        colorBlending.blendConstants[3] = 0.0f;
        pipelineInfo.pColorBlendState = &colorBlending;

        //Pipeline Layout
        if (debugMode) std::cout << "Creating PipelineLayout " << std::endl;
        vk::PipelineLayout pipelineLayout = create_pipeline_layout(specyfication.device, debugMode);
        pipelineInfo.layout = pipelineLayout;
        //Render Pass
        if (debugMode) std::cout << "Creating RenderPass " << std::endl;
        vk::RenderPass  renderpass = create_renderpass(specyfication.device, specyfication.swapchainImageFormat, debugMode);
        pipelineInfo.renderPass = renderpass;

        /// Create Graphics Pipeline
        pipelineInfo.basePipelineHandle = nullptr;
        if (debugMode) std::cout << "Creating Graphics Pipeline " << std::endl;
        vk::Pipeline graphicsPipeline;

        try {
            graphicsPipeline = (specyfication.device.createGraphicsPipeline(nullptr, pipelineInfo)).value;
        }
        catch (vk::SystemError err) {
            if (debugMode) std::cout << "Failed create Graphics Pipeline!" << std::endl;
        }

        GraphicsPipelineOutBundle output{};
        output.layout = pipelineLayout;
        output.renderpass = renderpass;
        output.graphicsPipeline = graphicsPipeline;


        specyfication.device.destroyShaderModule(vertexShader);
        specyfication.device.destroyShaderModule(fragmentShader);
        return output;
	}
}

