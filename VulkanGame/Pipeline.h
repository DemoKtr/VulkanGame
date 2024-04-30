#pragma once
#include "config.h"
#include "Shaders.h"
#include "RenderStruct.h"
#include "Mesh.h"
namespace vkInit {
	struct GraphicsPipelineInBundle {
		vk::Device device;
		std::string vertexFilePath;
		std::string fragmentFilePath;
        vk::Extent2D swapchainExtent;
        vk::Format swapchainImageFormat;
        vk::DescriptorSetLayout descriptorSetLayout;
	};
	struct GraphicsPipelineOutBundle {
        vk::PipelineLayout layout;
        vk::RenderPass renderpass;
        vk::Pipeline graphicsPipeline;
	};
    vk::PipelineLayout create_pipeline_layout(vk::Device device, vk::DescriptorSetLayout descriptorSetLayout ,bool debugMode);
    vk::RenderPass create_renderpass(vk::Device device, vk::Format swapchainImageFormat, bool debugMode);
    GraphicsPipelineOutBundle create_graphic_pipeline(GraphicsPipelineInBundle specyfication, bool debugMode);
    vk::PipelineVertexInputStateCreateInfo make_vertex_input_info(
        const vk::VertexInputBindingDescription& bindingDescription,
        const std::array<vk::VertexInputAttributeDescription, 2>& attributeDescriptions);
    vk::PipelineInputAssemblyStateCreateInfo make_input_assembly_info();
    vk::PipelineShaderStageCreateInfo make_shader_info(
        const vk::ShaderModule& shaderModule, const vk::ShaderStageFlagBits& stage);

    vk::PipelineVertexInputStateCreateInfo make_vertex_input_info(
        const vk::VertexInputBindingDescription& bindingDescription,
        const std::array<vk::VertexInputAttributeDescription, 2>& attributeDescriptions);

    /**
        \returns the input assembly stage creation info
    */
    vk::PipelineInputAssemblyStateCreateInfo make_input_assembly_info();

    /**
        Configure a programmable shader stage.

        \param shaderModule the compiled shader module
        \param stage the shader stage which the module is for
        \returns the shader stage creation info
    */
    vk::PipelineShaderStageCreateInfo make_shader_info(
        const vk::ShaderModule& shaderModule, const vk::ShaderStageFlagBits& stage);

    /**
        Create a viewport.

        \param specification holds relevant data fields
        \returns the created viewport
    */
    vk::Viewport make_viewport(const GraphicsPipelineInBundle& specification);

    /**
        Create a scissor rectangle.

        \param specification holds relevant data fields
        \returns the created rectangle
    */
    vk::Rect2D make_scissor(const GraphicsPipelineInBundle& specification);

    /**
        Configure the pipeline's viewport stage.

        \param viewport the viewport specification
        \param scissor the scissor rectangle to apply
        \returns the viewport state creation info
    */
    vk::PipelineViewportStateCreateInfo make_viewport_state(const vk::Viewport& viewport, const vk::Rect2D& scissor);

    /**
        \returns the creation info for the configured rasterizer stage
    */
    vk::PipelineRasterizationStateCreateInfo make_rasterizer_info();

    /**
        \returns the creation info for the configured multisampling stage
    */
    vk::PipelineMultisampleStateCreateInfo make_multisampling_info();

    /**
        \returns the created color blend state
    */
    vk::PipelineColorBlendAttachmentState make_color_blend_attachment_state();

    /**
        \returns the creation info for the configured color blend stage
    */
    vk::PipelineColorBlendStateCreateInfo make_color_blend_attachment_stage(
        const vk::PipelineColorBlendAttachmentState& colorBlendAttachment);

    /**
        Make a pipeline layout, this consists mostly of describing the
        push constants and descriptor set layouts which will be used.

        \param device the logical device
        \returns the created pipeline layout
    */


    /**
        \returns the created push constant range
    */
    vk::PushConstantRange make_push_constant_info();
    /**
        Make a color attachment description

        \param swapchainImageFormat the image format used by the swapchain
        \returns a description of the corresponding color attachment
    */
    vk::AttachmentDescription make_color_attachment(const vk::Format& swapchainImageFormat);

    /**
        \returns Make a color attachment refernce
    */
    vk::AttachmentReference make_color_attachment_reference();

    /**
        Make a simple subpass.

        \param colorAttachmentRef a reference to a color attachment for the color buffer
        \returns a description of the subpass
    */
    vk::SubpassDescription make_subpass(const vk::AttachmentReference& colorAttachmentRef);

    /**
        Make a simple renderpass.

        \param colorAttachment the color attachment for the color buffer
        \param subpass a description of the subpass
        \returns creation info for the renderpass
    */
    vk::RenderPassCreateInfo make_renderpass_info(const vk::AttachmentDescription& colorAttachment, const vk::SubpassDescription& subpass);

    



    vk::PipelineLayout create_pipeline_layout(vk::Device device, vk::DescriptorSetLayout descriptorSetLayout, bool debugMode) {
        /*
        typedef struct VkPipelineLayoutCreateInfo {
            VkStructureType                 sType;
            const void*                     pNext;
            VkPipelineLayoutCreateFlags     flags;
            uint32_t                        setLayoutCount;
            const VkDescriptorSetLayout*    pSetLayouts;
            uint32_t                        pushConstantRangeCount;
            const VkPushConstantRange*      pPushConstantRanges;
        } VkPipelineLayoutCreateInfo;
        */
        
       vk::PipelineLayoutCreateInfo layoutInfo;
		layoutInfo.flags = vk::PipelineLayoutCreateFlags();

		layoutInfo.setLayoutCount = 1;
		layoutInfo.pSetLayouts = &descriptorSetLayout;

		layoutInfo.pushConstantRangeCount = 1;
		vk::PushConstantRange pushConstantInfo = make_push_constant_info();
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
        vk::AttachmentDescription colorAttachment = make_color_attachment(swapchainImageFormat);

        //Declare that attachment to be color buffer 0 of the framebuffer
        vk::AttachmentReference colorAttachmentRef = make_color_attachment_reference();

        //Renderpasses are broken down into subpasses, there's always at least one.
        vk::SubpassDescription subpass = make_subpass(colorAttachmentRef);

        //Now create the renderpass
        vk::RenderPassCreateInfo renderpassInfo = make_renderpass_info(colorAttachment, subpass);
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
        vk::VertexInputBindingDescription bindingDescription = vkMesh::getPosColBindingDescription();
        std::array < vk::VertexInputAttributeDescription, 2> attributeDescriptions = vkMesh::getPosColorAttributeDescriptions();
        vk::PipelineVertexInputStateCreateInfo vertexInputInfo = make_vertex_input_info(bindingDescription, attributeDescriptions);
        pipelineInfo.pVertexInputState = &vertexInputInfo;




        //Input Assembly
        vk::PipelineInputAssemblyStateCreateInfo inputAssemblyInfo = make_input_assembly_info();
        pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;

        //Vertex Shader
        if (debugMode) {
            std::cout << "Create Vertex Shader " << std::endl;
        }
        vk::ShaderModule vertexShader = vkUtil::createModule(specyfication.vertexFilePath, specyfication.device, debugMode);
        vk::PipelineShaderStageCreateInfo vertexShaderInfo = make_shader_info(vertexShader, vk::ShaderStageFlagBits::eVertex);
        shaderStages.push_back(vertexShaderInfo);


        //Viewport and Scissor
        vk::Viewport viewport = make_viewport(specyfication);
        vk::Rect2D scissor = make_scissor(specyfication);
        vk::PipelineViewportStateCreateInfo viewportState = make_viewport_state(viewport, scissor);
        pipelineInfo.pViewportState = &viewportState;

        //Rasterizer
        vk::PipelineRasterizationStateCreateInfo rasterizer = make_rasterizer_info();
        pipelineInfo.pRasterizationState = &rasterizer;


        //Fragment Shader
        if (debugMode) {
            std::cout << "Create Fragment Shader " << std::endl;
        }
        vk::ShaderModule fragmentShader = vkUtil::createModule(specyfication.fragmentFilePath, specyfication.device, debugMode);
        vk::PipelineShaderStageCreateInfo fragmentShaderInfo = make_shader_info(fragmentShader, vk::ShaderStageFlagBits::eFragment);
        shaderStages.push_back(fragmentShaderInfo);

        //Now both shaders have been made, we can declare them to the pipeline info
        pipelineInfo.stageCount = shaderStages.size();
        pipelineInfo.pStages = shaderStages.data();

        //Multisampling
        vk::PipelineMultisampleStateCreateInfo multisampling = make_multisampling_info();
        pipelineInfo.pMultisampleState = &multisampling;

        //Color Blend
        vk::PipelineColorBlendAttachmentState colorBlendAttachment = make_color_blend_attachment_state();
        vk::PipelineColorBlendStateCreateInfo colorBlending = make_color_blend_attachment_stage(colorBlendAttachment);
        pipelineInfo.pColorBlendState = &colorBlending;

        //Pipeline Layout
        if (debugMode) std::cout << "Creating PipelineLayout " << std::endl;
        vk::PipelineLayout pipelineLayout = create_pipeline_layout(specyfication.device, specyfication.descriptorSetLayout,debugMode);
        pipelineInfo.layout = pipelineLayout;
        //Render Pass
        if (debugMode) std::cout << "Creating RenderPass " << std::endl;
        vk::RenderPass renderpass = create_renderpass(specyfication.device, specyfication.swapchainImageFormat,debugMode);
        pipelineInfo.renderPass = renderpass;
        pipelineInfo.subpass = 0;


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

        GraphicsPipelineOutBundle output;
        output.layout = pipelineLayout;
        output.renderpass = renderpass;
        output.graphicsPipeline = graphicsPipeline;

        //Finally clean up by destroying shader modules
        specyfication.device.destroyShaderModule(vertexShader);
        specyfication.device.destroyShaderModule(fragmentShader);
        return output;
    }

    vk::PipelineVertexInputStateCreateInfo make_vertex_input_info(
        const vk::VertexInputBindingDescription& bindingDescription,
        const std::array<vk::VertexInputAttributeDescription, 2>& attributeDescriptions) {

        vk::PipelineVertexInputStateCreateInfo vertexInputInfo = {};
        vertexInputInfo.flags = vk::PipelineVertexInputStateCreateFlags();
        vertexInputInfo.vertexBindingDescriptionCount = 1;
        vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
        vertexInputInfo.vertexAttributeDescriptionCount = 2;
        vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
        return vertexInputInfo;
    }

    vk::PipelineInputAssemblyStateCreateInfo make_input_assembly_info() {

        vk::PipelineInputAssemblyStateCreateInfo inputAssemblyInfo = {};
        inputAssemblyInfo.flags = vk::PipelineInputAssemblyStateCreateFlags();
        inputAssemblyInfo.topology = vk::PrimitiveTopology::eTriangleList;

        return inputAssemblyInfo;
    }
    vk::PipelineShaderStageCreateInfo make_shader_info(const vk::ShaderModule& shaderModule, const vk::ShaderStageFlagBits& stage) {

        vk::PipelineShaderStageCreateInfo shaderInfo = {};
        shaderInfo.flags = vk::PipelineShaderStageCreateFlags();
        shaderInfo.stage = stage;
        shaderInfo.module = shaderModule;
        shaderInfo.pName = "main";
        return shaderInfo;
    }

    vk::Viewport make_viewport(const GraphicsPipelineInBundle& specification) {

        vk::Viewport viewport = {};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)specification.swapchainExtent.width;
        viewport.height = (float)specification.swapchainExtent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        return viewport;
    }

    vk::Rect2D make_scissor(const GraphicsPipelineInBundle& specification) {

        vk::Rect2D scissor = {};
        scissor.offset.x = 0.0f;
        scissor.offset.y = 0.0f;
        scissor.extent = specification.swapchainExtent;

        return scissor;
    }

    vk::PipelineViewportStateCreateInfo make_viewport_state(const vk::Viewport& viewport, const vk::Rect2D& scissor) {

        vk::PipelineViewportStateCreateInfo viewportState = {};
        viewportState.flags = vk::PipelineViewportStateCreateFlags();
        viewportState.viewportCount = 1;
        viewportState.pViewports = &viewport;
        viewportState.scissorCount = 1;
        viewportState.pScissors = &scissor;

        return viewportState;
    }

    vk::PipelineRasterizationStateCreateInfo make_rasterizer_info() {

        vk::PipelineRasterizationStateCreateInfo rasterizer = {};
        rasterizer.flags = vk::PipelineRasterizationStateCreateFlags();
        rasterizer.depthClampEnable = VK_FALSE; //discard out of bounds fragments, don't clamp them
        rasterizer.rasterizerDiscardEnable = VK_FALSE; //This flag would disable fragment output
        rasterizer.polygonMode = vk::PolygonMode::eFill;
        rasterizer.lineWidth = 1.0f;
        rasterizer.cullMode = vk::CullModeFlagBits::eBack;
        rasterizer.frontFace = vk::FrontFace::eClockwise;
        rasterizer.depthBiasEnable = VK_FALSE; //Depth bias can be useful in shadow maps.

        return rasterizer;
    }
    vk::PipelineMultisampleStateCreateInfo make_multisampling_info() {

        vk::PipelineMultisampleStateCreateInfo multisampling = {};
        multisampling.flags = vk::PipelineMultisampleStateCreateFlags();
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = vk::SampleCountFlagBits::e1;

        return multisampling;
    }

    vk::PipelineColorBlendAttachmentState make_color_blend_attachment_state() {

        vk::PipelineColorBlendAttachmentState colorBlendAttachment = {};
        colorBlendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
        colorBlendAttachment.blendEnable = VK_FALSE;

        return colorBlendAttachment;
    }

    vk::PipelineColorBlendStateCreateInfo make_color_blend_attachment_stage(const vk::PipelineColorBlendAttachmentState& colorBlendAttachment) {

        vk::PipelineColorBlendStateCreateInfo colorBlending = {};
        colorBlending.flags = vk::PipelineColorBlendStateCreateFlags();
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = vk::LogicOp::eCopy;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;
        colorBlending.blendConstants[0] = 0.0f;
        colorBlending.blendConstants[1] = 0.0f;
        colorBlending.blendConstants[2] = 0.0f;
        colorBlending.blendConstants[3] = 0.0f;

        return colorBlending;
    }
    vk::PushConstantRange make_push_constant_info() {

        vk::PushConstantRange pushConstantInfo;
        pushConstantInfo.offset = 0;
        pushConstantInfo.size = sizeof(vkUtil::ObjectData);
        pushConstantInfo.stageFlags = vk::ShaderStageFlagBits::eVertex;

        return pushConstantInfo;
    }

    vk::AttachmentDescription make_color_attachment(const vk::Format& swapchainImageFormat) {

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

        return colorAttachment;
    }

    vk::AttachmentReference make_color_attachment_reference() {

        vk::AttachmentReference colorAttachmentRef = {};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal;

        return colorAttachmentRef;
    }

    vk::SubpassDescription make_subpass(const vk::AttachmentReference& colorAttachmentRef) {

        vk::SubpassDescription subpass = {};
        subpass.flags = vk::SubpassDescriptionFlags();
        subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;

        return subpass;
    }

    vk::RenderPassCreateInfo make_renderpass_info(const vk::AttachmentDescription& colorAttachment, const vk::SubpassDescription& subpass) {

        vk::RenderPassCreateInfo renderpassInfo = {};
        renderpassInfo.flags = vk::RenderPassCreateFlags();
        renderpassInfo.attachmentCount = 1;
        renderpassInfo.pAttachments = &colorAttachment;
        renderpassInfo.subpassCount = 1;
        renderpassInfo.pSubpasses = &subpass;

        return renderpassInfo;
    }

}

