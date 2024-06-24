#pragma once
#include "config.h"
#include "Shaders.h"
#include "RenderStruct.h"
#include "Mesh.h"
#include "RenderStruct.h"
#include "RenderPass.h"
#include "Particle.h"
namespace vkInit {
	struct GraphicsPipelineInBundle {
		vk::Device device;
		std::string vertexFilePath;
		std::string fragmentFilePath;
		std::string deferedFragmentFilePath;
		std::string deferedVertexFilePath;
        vk::Extent2D swapchainExtent;
        vk::Format swapchainImageFormat, depthFormat;
        std::vector<vk::DescriptorSetLayout> geometryDescriptorSetLayouts;
        std::vector<vk::DescriptorSetLayout> deferedDescriptorSetLayouts;
        vkUtil::Gbuffer gbuffer;
        vkUtil::FrameBufferAttachment postProcessImageInput;
	};

    struct ShadowGraphicsPipelineInBundle {
        vk::Device device;
        std::string vertexFilePath;
        std::string geometryFilePath;
        vk::Extent2D swapchainExtent;
        vk::Format  depthFormat;
        std::vector<vk::DescriptorSetLayout> shadowDescriptorSetLayout;
        vkUtil::shadowMapBuffer shadowAttachmentBuffer;
    };
    struct ParticleGraphicsPipelineInBundle {
        vk::Device device;
        std::string vertexFilePath;
        std::string fragmentFilePath;
        std::string computeFilePath;
        vk::Extent2D swapchainExtent;
        vk::Format  depthFormat;
        vkUtil::FrameBufferAttachment particleAttachment;
        std::vector<vk::DescriptorSetLayout> particleComputeDescriptorSetLayout;
        std::vector<vk::DescriptorSetLayout> particleGraphicDescriptorSetLayout;
    };

    struct PostProcessPipelineInBundle {
        vk::Device device;
        std::string vertexFilePath;
        std::string fragmentFilePath;
        vk::Extent2D swapchainExtent;
        //vk::Format  depthFormat;
        vk::Format swapchainImageFormat;
        std::vector<vk::DescriptorSetLayout> postProcessSetLayout;
 
    };
    struct skyBoxPipelineInBundle {
        vk::Device device;
        std::string vertexFilePath;
        std::string fragmentFilePath;
        vk::Extent2D swapchainExtent;
        vk::Format  depthFormat;
        vk::Format swapchainImageFormat;
        std::vector<vk::DescriptorSetLayout> skyBoxSetLayout;
        vkUtil::FrameBufferAttachment Attachment;

    };

    struct updownGraphicsPipelineInBundle {
        vk::Device device;
        std::string downScalevertexFilePath;
        std::string upScalevertexFilePath;
        std::string downScalefragmentFilePath;
        std::string upScalefragmentFilePath;
        std::vector<vk::DescriptorSetLayout> downSampleDescriptorSetLayout;
        std::vector<vk::DescriptorSetLayout> upSampleDescriptorSetLayout;
        std::vector<glm::ivec2> screenSize;
    };

	struct GraphicsPipelineOutBundle {
        vk::PipelineLayout layout;
        vk::PipelineLayout deferedLayout;
        vk::RenderPass renderpass;
        vk::Pipeline graphicsPipeline;
        vk::Pipeline deferedGraphicsPipeline;
	};

    struct ShadowGraphicsPipelineOutBundle {
        vk::PipelineLayout shadowPipelineLayout;
        vk::RenderPass shadowRenderPass;
        vk::Pipeline shadowPipeline;
    };
    struct ParticleGraphicsPipelineOutBundle {
        vk::PipelineLayout particlePipelineLayout;
        vk::PipelineLayout particleComputePipelineLayout;
        vk::RenderPass particleRenderPass;
        vk::Pipeline particleComputePipeline;
        vk::Pipeline particleGrphicPipeline;
    };

    struct PostProcessGraphicsPipelineOutBundle {
        vk::PipelineLayout postProcessPipelineLayout;
        vk::RenderPass renderpass;
        vk::Pipeline postProcessgraphicsPipeline;
    };

    struct skyBoxGraphicsPipelineOutBundle {
        vk::PipelineLayout skyBoxPipelineLayout;
        vk::RenderPass renderpass;
        vk::Pipeline skyBoxgraphicsPipeline;
    };


    struct updownGraphicsPipelineOutBundle {
        vk::PipelineLayout downscalePipelineLayout;
        vk::PipelineLayout upscalePipelineLayout;
        vk::RenderPass renderpass;
        std::vector<vk::Pipeline> downscalePipeline;
        std::vector<vk::Pipeline> upscaleGrphicPipeline;
    };


    ShadowGraphicsPipelineOutBundle createShadowsPipeline(ShadowGraphicsPipelineInBundle specyfication, bool debugMode);
    vk::PipelineLayout create_pipeline_layout(vk::Device device, std::vector<vk::DescriptorSetLayout> descriptorSetLayouts ,bool debugMode);
    PostProcessGraphicsPipelineOutBundle create_postprocess_pipelines(PostProcessPipelineInBundle specyfication, bool debugMode);
    vk::RenderPass create_renderpass(vk::Device device, vk::Format swapchainImageFormat, vk::Format depthFormat, bool debugMode);
    GraphicsPipelineOutBundle create_graphic_pipeline(GraphicsPipelineInBundle specyfication, bool debugMode);
    
    vk::PipelineInputAssemblyStateCreateInfo make_input_assembly_info();
    vk::PipelineShaderStageCreateInfo make_shader_info(
        const vk::ShaderModule& shaderModule, const vk::ShaderStageFlagBits& stage);
    vk::PipelineVertexInputStateCreateInfo make_vertex_input_info(
        const vk::VertexInputBindingDescription& bindingDescription,
        const std::vector<vk::VertexInputAttributeDescription>& attributeDescriptions);
    skyBoxGraphicsPipelineOutBundle create_skybox_pipeline(skyBoxPipelineInBundle specyfication, bool debugMode);
    updownGraphicsPipelineOutBundle create_updownsampling_pipeline(updownGraphicsPipelineInBundle specyfication, bool debugMode);
    /**
        \returns the input assembly stage creation info
    */
    vk::PipelineInputAssemblyStateCreateInfo make_input_assembly_info();
    ParticleGraphicsPipelineOutBundle createParticlePipeline(ParticleGraphicsPipelineInBundle specification, bool debugMode);
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

    std::array<vk::PipelineColorBlendAttachmentState, 4> makeDeferedAttachmentState();

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


    vk::AttachmentDescription make_depth_attachment(const vk::Format& depthFormat);


    vk::AttachmentReference make_depth_attachment_reference();


    vk::Viewport make_viewport(const vk::Extent2D& swapchainExtent);

    vk::Rect2D make_scissor(const vk::Extent2D& swapchainExtent);

    /**
        Make a simple subpass.

        \param colorAttachmentRef a reference to a color attachment for the color buffer
        \returns a description of the subpass
    */
    vk::SubpassDescription make_subpass(const std::vector<vk::AttachmentReference>& attachments);

    /**
        Make a simple renderpass.

        \param colorAttachment the color attachment for the color buffer
        \param subpass a description of the subpass
        \returns creation info for the renderpass
    */
    vk::RenderPassCreateInfo make_renderpass_info(const std::vector<vk::AttachmentDescription>& attachemnts, const vk::SubpassDescription& subpass);

    GraphicsPipelineOutBundle create_defered_pipelines();



    vk::PipelineLayout create_pipeline_layout(vk::Device device,std::vector<vk::DescriptorSetLayout> descriptorSetLayouts, bool debugMode) {
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
        
        layoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
		layoutInfo.pSetLayouts = descriptorSetLayouts.data();

		layoutInfo.pushConstantRangeCount = 0;

        try {
            return device.createPipelineLayout(layoutInfo);
        }
        catch (vk::SystemError err) {
            if (debugMode) std::cout << "Failed Create pipeline Layout" << std::endl;
        }
    }
    vk::RenderPass create_renderpass(vk::Device device, vk::Format swapchainImageFormat,vk::Format depthFormat ,bool debugMode) {
        
        std::vector<vk::AttachmentDescription> attachments;
        std::vector<vk::AttachmentReference> attachmentRefertences;
        
        //Define a general attachment, with its load/store operations
        attachments.push_back(make_color_attachment(swapchainImageFormat));

        //Declare that attachment to be color buffer 0 of the framebuffer
         attachmentRefertences.push_back(make_color_attachment_reference());


         attachments.push_back(make_depth_attachment(depthFormat));

         //Declare that attachment to be color buffer 0 of the framebuffer
         attachmentRefertences.push_back(make_depth_attachment_reference());


        //Renderpasses are broken down into subpasses, there's always at least one.
        vk::SubpassDescription subpass = make_subpass(attachmentRefertences);

        //Now create the renderpass
        vk::RenderPassCreateInfo renderpassInfo = make_renderpass_info(attachments, subpass);
        //vk::RenderPassCreateInfo deferedrenderpassInfo = makeDeferedRenderpassInfo(swapchainImageFormat);
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
        std::vector <vk::VertexInputAttributeDescription> attributeDescriptions = vkMesh::getPosColorAttributeDescriptions();
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

        //Depth Stencil Stage
        vk::PipelineDepthStencilStateCreateInfo depthStageInfo;
        depthStageInfo.flags = vk::PipelineDepthStencilStateCreateFlags();
        depthStageInfo.depthTestEnable = true;
        depthStageInfo.depthWriteEnable = true;
        depthStageInfo.depthCompareOp = vk::CompareOp::eLess;
        depthStageInfo.depthBoundsTestEnable = false;
        depthStageInfo.stencilTestEnable = false;
        pipelineInfo.pDepthStencilState = &depthStageInfo;

        //Multisampling
        vk::PipelineMultisampleStateCreateInfo multisampling = make_multisampling_info();
        pipelineInfo.pMultisampleState = &multisampling;

        //Color Blend
        vk::PipelineColorBlendAttachmentState colorBlendAttachment = make_color_blend_attachment_state();
        vk::PipelineColorBlendStateCreateInfo colorBlending = make_color_blend_attachment_stage(colorBlendAttachment);
        pipelineInfo.pColorBlendState = &colorBlending;

        //Pipeline Layout
        if (debugMode) std::cout << "Creating PipelineLayout " << std::endl;
        vk::PipelineLayout pipelineLayout = create_pipeline_layout(specyfication.device, specyfication.geometryDescriptorSetLayouts,debugMode);
        pipelineInfo.layout = pipelineLayout;
        //Render Pass
        if (debugMode) std::cout << "Creating RenderPass " << std::endl;
        vk::RenderPass renderpass = create_renderpass(specyfication.device, specyfication.swapchainImageFormat,specyfication.depthFormat,debugMode);
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

    vk::PipelineVertexInputStateCreateInfo make_vertex_input_info(const vk::VertexInputBindingDescription& bindingDescription, const std::vector<vk::VertexInputAttributeDescription>& attributeDescriptions)
    {
        vk::PipelineVertexInputStateCreateInfo vertexInputInfo = {};
        vertexInputInfo.flags = vk::PipelineVertexInputStateCreateFlags();
        vertexInputInfo.vertexBindingDescriptionCount = 1;
        vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
        vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
        return vertexInputInfo;
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



    vk::Viewport make_viewport(const vk::Extent2D &swapchainExtent) {

        vk::Viewport viewport = {};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)swapchainExtent.width;
        viewport.height = (float)swapchainExtent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        return viewport;
    }

    vk::Rect2D make_scissor(const vk::Extent2D &swapchainExtent) {

        vk::Rect2D scissor = {};
        scissor.offset.x = 0.0f;
        scissor.offset.y = 0.0f;
        scissor.extent = swapchainExtent;

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


    vk::AttachmentDescription make_depth_attachment(const vk::Format& depth) {

        vk::AttachmentDescription colorAttachment = {};
        colorAttachment.flags = vk::AttachmentDescriptionFlags();
        colorAttachment.format = depth;
        colorAttachment.samples = vk::SampleCountFlagBits::e1;
        colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
        colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
        colorAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
        colorAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
        colorAttachment.initialLayout = vk::ImageLayout::eUndefined;
        colorAttachment.finalLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

        return colorAttachment;
    }

    vk::AttachmentReference make_depth_attachment_reference() {

        vk::AttachmentReference colorAttachmentRef = {};
        colorAttachmentRef.attachment = 1;
        colorAttachmentRef.layout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

        return colorAttachmentRef;
    }

    vk::SubpassDescription make_subpass(const std::vector<vk::AttachmentReference>& attachments) {

        vk::SubpassDescription subpass = {};
        subpass.flags = vk::SubpassDescriptionFlags();
        subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &attachments[0];
        subpass.pDepthStencilAttachment = &attachments[1];

        return subpass;
    }

    vk::RenderPassCreateInfo make_renderpass_info(const  std::vector<vk::AttachmentDescription>& attachments, const vk::SubpassDescription& subpass) {

        vk::RenderPassCreateInfo renderpassInfo = {};
        renderpassInfo.flags = vk::RenderPassCreateFlags();
        renderpassInfo.attachmentCount = attachments.size();
        renderpassInfo.pAttachments = attachments.data();
        renderpassInfo.subpassCount = 1;
        renderpassInfo.pSubpasses = &subpass;

        return renderpassInfo;
    }

    GraphicsPipelineOutBundle create_defered_pipelines(GraphicsPipelineInBundle specyfication,bool debugMode)
    {
        GraphicsPipelineOutBundle output;

        vk::PipelineLayout geometryPipelineLayout = create_pipeline_layout(specyfication.device, specyfication.geometryDescriptorSetLayouts, debugMode);
        vk::PipelineLayout deferedPipelineLayout = create_pipeline_layout(specyfication.device, specyfication.deferedDescriptorSetLayouts, debugMode);
        vk::PipelineInputAssemblyStateCreateInfo inputAssemblyState = make_input_assembly_info();
        vk::PipelineRasterizationStateCreateInfo rasterizationState = make_rasterizer_info();
        vk::PipelineColorBlendAttachmentState blendAttachmentState = make_color_blend_attachment_state();
        vk::PipelineColorBlendStateCreateInfo colorBlendState = make_color_blend_attachment_stage(blendAttachmentState);
        vk::PipelineDepthStencilStateCreateInfo depthStageInfo;
        depthStageInfo.flags = vk::PipelineDepthStencilStateCreateFlags();
        depthStageInfo.depthTestEnable = true;
        depthStageInfo.depthWriteEnable = true;
        depthStageInfo.depthCompareOp = vk::CompareOp::eLess;
        depthStageInfo.depthBoundsTestEnable = false;
        depthStageInfo.stencilTestEnable = false;
        //Viewport and Scissor
        vk::Viewport viewport = make_viewport(specyfication);
        vk::Rect2D scissor = make_scissor(specyfication);
        vk::PipelineViewportStateCreateInfo viewportState = make_viewport_state(viewport, scissor);
        vk::PipelineMultisampleStateCreateInfo multisampleState = make_multisampling_info();
        std::array<vk::PipelineShaderStageCreateInfo, 2> shaderStages;

        vk::RenderPass renderpass = vkInit::create_defered_renderpass(specyfication.device, specyfication.gbuffer, specyfication.swapchainImageFormat, specyfication.depthFormat,specyfication.postProcessImageInput);

        vk::GraphicsPipelineCreateInfo pipelineInfo = {};
        pipelineInfo.pInputAssemblyState = &inputAssemblyState;
        pipelineInfo.renderPass = renderpass;
        pipelineInfo.pRasterizationState = &rasterizationState;
        pipelineInfo.pColorBlendState = &colorBlendState;
        pipelineInfo.pMultisampleState = &multisampleState;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pDepthStencilState = &depthStageInfo;
        
        /////////////////////////////////////////////////////////////
        //offscreen
        pipelineInfo.layout = geometryPipelineLayout;
        vk::VertexInputBindingDescription bindingDescription = vkMesh::getPosColBindingDescription();
        std::vector <vk::VertexInputAttributeDescription> attributeDescriptions = vkMesh::getPosColorAttributeDescriptions();
        vk::PipelineVertexInputStateCreateInfo vertexInputInfo = make_vertex_input_info(bindingDescription, attributeDescriptions);
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        rasterizationState.cullMode = vk::CullModeFlagBits::eFront;

        // Offscreen pipeline
        vk::ShaderModule vertexShader = vkUtil::createModule(specyfication.vertexFilePath, specyfication.device, debugMode);
        vk::PipelineShaderStageCreateInfo vertexShaderInfo = make_shader_info(vertexShader, vk::ShaderStageFlagBits::eVertex);
        shaderStages[0] =(vertexShaderInfo);
        vk::ShaderModule fragmentShader = vkUtil::createModule(specyfication.fragmentFilePath, specyfication.device, debugMode);
        vk::PipelineShaderStageCreateInfo fragmentShaderInfo = make_shader_info(fragmentShader, vk::ShaderStageFlagBits::eFragment);
        shaderStages[1] = (fragmentShaderInfo);

        // Blend attachment states required for all color attachments
        // This is important, as color write mask will otherwise be 0x0 and you
        // won't see anything rendered to the attachment
        std::array<vk::PipelineColorBlendAttachmentState, 7> blendAttachmentStates = {
   vk::PipelineColorBlendAttachmentState(
        VK_FALSE,
        vk::BlendFactor::eOne,
        vk::BlendFactor::eZero,
        vk::BlendOp::eAdd,
        vk::BlendFactor::eOne,
        vk::BlendFactor::eZero,
        vk::BlendOp::eAdd,
        vk::ColorComponentFlags(VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT)
    ),
    vk::PipelineColorBlendAttachmentState(
        VK_FALSE,
        vk::BlendFactor::eOne,
        vk::BlendFactor::eZero,
        vk::BlendOp::eAdd,
        vk::BlendFactor::eOne,
        vk::BlendFactor::eZero,
        vk::BlendOp::eAdd,
        vk::ColorComponentFlags(VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT)
    ),
    vk::PipelineColorBlendAttachmentState(
        VK_FALSE,
        vk::BlendFactor::eOne,
        vk::BlendFactor::eZero,
        vk::BlendOp::eAdd,
        vk::BlendFactor::eOne,
        vk::BlendFactor::eZero,
        vk::BlendOp::eAdd,
        vk::ColorComponentFlags(VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT)
    ),
    vk::PipelineColorBlendAttachmentState(
        VK_FALSE,
        vk::BlendFactor::eOne,
        vk::BlendFactor::eZero,
        vk::BlendOp::eAdd,
        vk::BlendFactor::eOne,
        vk::BlendFactor::eZero,
        vk::BlendOp::eAdd,
        vk::ColorComponentFlags(VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT)
    ),
    vk::PipelineColorBlendAttachmentState(
        VK_FALSE,
        vk::BlendFactor::eOne,
        vk::BlendFactor::eZero,
        vk::BlendOp::eAdd,
        vk::BlendFactor::eOne,
        vk::BlendFactor::eZero,
        vk::BlendOp::eAdd,
        vk::ColorComponentFlags(VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT)
    ),
    vk::PipelineColorBlendAttachmentState(
        VK_FALSE,
        vk::BlendFactor::eOne,
        vk::BlendFactor::eZero,
        vk::BlendOp::eAdd,
        vk::BlendFactor::eOne,
        vk::BlendFactor::eZero,
        vk::BlendOp::eAdd,
        vk::ColorComponentFlags(VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT)
    ),
    vk::PipelineColorBlendAttachmentState(
        VK_FALSE,
        vk::BlendFactor::eOne,
        vk::BlendFactor::eZero,
        vk::BlendOp::eAdd,
        vk::BlendFactor::eOne,
        vk::BlendFactor::eZero,
        vk::BlendOp::eAdd,
        vk::ColorComponentFlags(VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT)
    ),
        };

        colorBlendState.attachmentCount = static_cast<uint32_t>(blendAttachmentStates.size());
        colorBlendState.pAttachments = blendAttachmentStates.data();
        pipelineInfo.stageCount = shaderStages.size();
        pipelineInfo.pStages = shaderStages.data();
        pipelineInfo.subpass = 0;
        vk::Pipeline geometryPipeline;

        pipelineInfo.basePipelineHandle = nullptr;
        if (debugMode) std::cout << "Creating geometry Graphics Pipeline " << std::endl;
        try {
            geometryPipeline = (specyfication.device.createGraphicsPipeline(nullptr, pipelineInfo)).value;
        }
        catch (vk::SystemError err) {
            if (debugMode) std::cout << "Failed create geometry Graphics Pipeline!" << std::endl;
        }

        //////////////
        //Composition
        vk::PipelineVertexInputStateCreateInfo emptyInputState = {};
        pipelineInfo.pVertexInputState = &emptyInputState;

        rasterizationState.cullMode = vk::CullModeFlagBits::eBack;
        vk::ShaderModule dvertexShader = vkUtil::createModule(specyfication.deferedVertexFilePath, specyfication.device, debugMode);
        vk::PipelineShaderStageCreateInfo dvertexShaderInfo = make_shader_info(dvertexShader, vk::ShaderStageFlagBits::eVertex);
        shaderStages[0] = (dvertexShaderInfo);
        vk::ShaderModule dfragmentShader = vkUtil::createModule(specyfication.deferedFragmentFilePath, specyfication.device, debugMode);
        vk::PipelineShaderStageCreateInfo dfragmentShaderInfo = make_shader_info(dfragmentShader, vk::ShaderStageFlagBits::eFragment);
        shaderStages[1] = (dfragmentShaderInfo);
        pipelineInfo.subpass = 1;
        pipelineInfo.layout = deferedPipelineLayout;

        vk::Pipeline deferedPipeline;
        colorBlendState = make_color_blend_attachment_stage(blendAttachmentState);
        pipelineInfo.basePipelineHandle = nullptr;
        if (debugMode) std::cout << "Creating defered Graphics Pipeline " << std::endl;
        try {
            deferedPipeline = (specyfication.device.createGraphicsPipeline(nullptr, pipelineInfo)).value;
        }
        catch (vk::SystemError err) {
            if (debugMode) std::cout << "Failed create defererd Graphics Pipeline!" << std::endl;
        }

        output.layout = geometryPipelineLayout;
        output.deferedLayout = deferedPipelineLayout;
        output.graphicsPipeline = geometryPipeline;
        output.deferedGraphicsPipeline = deferedPipeline;
        output.renderpass = renderpass;

        specyfication.device.destroyShaderModule(vertexShader);
        specyfication.device.destroyShaderModule(fragmentShader);
        specyfication.device.destroyShaderModule(dvertexShader);
        specyfication.device.destroyShaderModule(dfragmentShader);
        return output;
    }

    ShadowGraphicsPipelineOutBundle createShadowsPipeline(ShadowGraphicsPipelineInBundle specyfication, bool debugMode) 
    {
        
        ShadowGraphicsPipelineOutBundle output;
        vk::Pipeline shadowPipeline = {};
        vk::PipelineLayout shadowPipelineLayout = create_pipeline_layout(specyfication.device, specyfication.shadowDescriptorSetLayout, debugMode);


        vk::PipelineInputAssemblyStateCreateInfo inputAssemblyState = make_input_assembly_info();
        vk::PipelineRasterizationStateCreateInfo rasterizationState = make_rasterizer_info();
        
        vk::PipelineDepthStencilStateCreateInfo depthStageInfo;
        depthStageInfo.flags = vk::PipelineDepthStencilStateCreateFlags();
        depthStageInfo.depthTestEnable = true;
        depthStageInfo.depthWriteEnable = true;
        depthStageInfo.depthCompareOp = vk::CompareOp::eLess;
        depthStageInfo.depthBoundsTestEnable = false;
        depthStageInfo.stencilTestEnable = false;
        //Viewport and Scissor
            vk::Rect2D scissor = {};
            scissor.offset.x = 0.0f;
            scissor.offset.y = 0.0f;
            scissor.extent.width = 1024;
            scissor.extent.height = 1024;
            vk::Viewport viewport = {};
            viewport.x = 0.0f;
            viewport.y = 0.0f;
            viewport.width = (float)1024;
            viewport.height = (float)1024;
            viewport.minDepth = 0.0f;
            viewport.maxDepth = 1.0f;

        vk::PipelineViewportStateCreateInfo viewportState = make_viewport_state(viewport, scissor);
        vk::PipelineMultisampleStateCreateInfo multisampleState = make_multisampling_info();
        std::array<vk::PipelineShaderStageCreateInfo, 2> shaderStages;
        vk::PipelineColorBlendAttachmentState blendAttachmentState = {};
        blendAttachmentState.colorWriteMask = vk::ColorComponentFlagBits::eR;
        blendAttachmentState.blendEnable = VK_FALSE;
        vk::PipelineColorBlendStateCreateInfo colorBlendState = make_color_blend_attachment_stage(blendAttachmentState);
        vk::RenderPass renderpass = vkInit::create_shadows_renderpass(specyfication.device, &specyfication.shadowAttachmentBuffer, specyfication.depthFormat);




        vk::GraphicsPipelineCreateInfo pipelineInfo = {};
        pipelineInfo.pInputAssemblyState = &inputAssemblyState;
        pipelineInfo.renderPass = renderpass;
        pipelineInfo.layout = shadowPipelineLayout;
        pipelineInfo.pRasterizationState = &rasterizationState;
        
        pipelineInfo.pMultisampleState = &multisampleState;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pDepthStencilState = &depthStageInfo;
        rasterizationState.depthBiasEnable = VK_TRUE;
        /////////////////////////////////////////////////////////////
        //offscreen
        pipelineInfo.layout = shadowPipelineLayout;
        vk::VertexInputBindingDescription bindingDescription = vkMesh::getPosColBindingDescription();
        std::vector <vk::VertexInputAttributeDescription> attributeDescriptions = vkMesh::getPosColorAttributeDescriptions();
        vk::PipelineVertexInputStateCreateInfo vertexInputInfo = make_vertex_input_info(bindingDescription, attributeDescriptions);
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        rasterizationState.cullMode = vk::CullModeFlagBits::eFront;

        // Offscreen pipeline
        vk::ShaderModule vertexShader = vkUtil::createModule(specyfication.vertexFilePath, specyfication.device, debugMode);
        vk::PipelineShaderStageCreateInfo vertexShaderInfo = make_shader_info(vertexShader, vk::ShaderStageFlagBits::eVertex);
        shaderStages[0] = (vertexShaderInfo);

        vk::ShaderModule geometryShader = vkUtil::createModule(specyfication.geometryFilePath, specyfication.device, debugMode);
        vk::PipelineShaderStageCreateInfo geometryShaderInfo = make_shader_info(geometryShader, vk::ShaderStageFlagBits::eGeometry);
        shaderStages[1] = geometryShaderInfo;
        

        // Blend attachment states required for all color attachments
        // This is important, as color write mask will otherwise be 0x0 and you
        // won't see anything rendered to the attachment
       

        colorBlendState.attachmentCount = 0;
        colorBlendState.pAttachments = nullptr;
        pipelineInfo.stageCount = shaderStages.size();
        pipelineInfo.pStages = shaderStages.data();
        pipelineInfo.subpass = 0;
        pipelineInfo.pColorBlendState = &colorBlendState;

        pipelineInfo.basePipelineHandle = nullptr;
        if (debugMode) std::cout << "Creating shadowMapping Graphics Pipeline " << std::endl;
        try {
            shadowPipeline = (specyfication.device.createGraphicsPipeline(nullptr, pipelineInfo)).value;
        }
        catch (vk::SystemError err) {
            if (debugMode) std::cout << "Failed create shadowMapping Graphics Pipeline!" << std::endl;
        }
        specyfication.device.destroyShaderModule(vertexShader);
        specyfication.device.destroyShaderModule(geometryShader);
        output.shadowPipeline = shadowPipeline;
        output.shadowRenderPass = renderpass;
        output.shadowPipelineLayout = shadowPipelineLayout;
        return output;
    }


    ParticleGraphicsPipelineOutBundle createParticlePipeline(ParticleGraphicsPipelineInBundle specification, bool debugMode)
    {

       
        vk::GraphicsPipelineCreateInfo pipelineInfo = {};
        vk::PipelineLayout particleGraphicPipelineLayout = create_pipeline_layout(specification.device, specification.particleGraphicDescriptorSetLayout, debugMode);
       
        
        //vk::PipelineInputAssemblyStateCreateInfo inputAssemblyState = make_input_assembly_info();
        vk::PipelineInputAssemblyStateCreateInfo inputAssemblyInfo = {};
        inputAssemblyInfo.flags = vk::PipelineInputAssemblyStateCreateFlags();
        inputAssemblyInfo.topology = vk::PrimitiveTopology::ePointList;
        vk::PipelineRasterizationStateCreateInfo rasterizationState = make_rasterizer_info();
        rasterizationState.polygonMode = vk::PolygonMode::eFill;

       
        vk::PipelineDepthStencilStateCreateInfo depthStageInfo;
        depthStageInfo.flags = vk::PipelineDepthStencilStateCreateFlags();
        depthStageInfo.depthTestEnable = true;
        depthStageInfo.depthWriteEnable = true;
        depthStageInfo.depthCompareOp = vk::CompareOp::eLess;
        depthStageInfo.depthBoundsTestEnable = false;
        depthStageInfo.stencilTestEnable = false;
        //Viewport and Scissor
        vk::Rect2D scissor = {};
        scissor.offset.x = 0.0f;
        scissor.offset.y = 0.0f;
        scissor.extent.width = specification.swapchainExtent.width;
        scissor.extent.height = specification.swapchainExtent.height;
        vk::Viewport viewport = {};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)specification.swapchainExtent.width;
        viewport.height = (float)specification.swapchainExtent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vk::PipelineViewportStateCreateInfo viewportState = make_viewport_state(viewport, scissor);
        

        vk::PipelineMultisampleStateCreateInfo multisampling = make_multisampling_info();
        
       



        vk::VertexInputBindingDescription bindingDescription = vkParticle::getParticleVBO();
        std::vector <vk::VertexInputAttributeDescription> attributeDescriptions = vkParticle::getParticleVAO();
        vk::PipelineVertexInputStateCreateInfo vertexInputInfo = make_vertex_input_info(bindingDescription, attributeDescriptions);
        
        
        vk::PipelineColorBlendAttachmentState blendAttachmentState = make_color_blend_attachment_state();
        // Additive blending
        //blendAttachmentState.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;;
       // blendAttachmentState.blendEnable = VK_TRUE;
       // blendAttachmentState.colorBlendOp = vk::BlendOp::eAdd;
       // blendAttachmentState.srcColorBlendFactor = vk::BlendFactor::eSrcAlpha;
      //  blendAttachmentState.dstColorBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha;
      //  blendAttachmentState.alphaBlendOp = vk::BlendOp::eAdd;
      //  blendAttachmentState.srcAlphaBlendFactor = vk::BlendFactor::eZero;
      //  blendAttachmentState.dstAlphaBlendFactor = vk::BlendFactor::eOne;

        vk::PipelineColorBlendStateCreateInfo colorBlendState = make_color_blend_attachment_stage(blendAttachmentState);
        
        vk::RenderPass renderpass = vkInit::create_particle_renderpass(specification.device, specification.depthFormat, specification.particleAttachment);
        pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;
        pipelineInfo.renderPass = renderpass;
        pipelineInfo.layout = particleGraphicPipelineLayout;
        pipelineInfo.pRasterizationState = &rasterizationState;

        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pDepthStencilState = &depthStageInfo;
        /////////////////////////////////////////////////////////////
        std::array<vk::PipelineShaderStageCreateInfo, 2> shaderStages;

        vk::ShaderModule vertexShader = vkUtil::createModule(specification.vertexFilePath, specification.device, debugMode);
        vk::PipelineShaderStageCreateInfo vertexShaderInfo = make_shader_info(vertexShader, vk::ShaderStageFlagBits::eVertex);
        shaderStages[0] = (vertexShaderInfo);

        vk::ShaderModule geometryShader = vkUtil::createModule(specification.fragmentFilePath, specification.device, debugMode);
        vk::PipelineShaderStageCreateInfo geometryShaderInfo = make_shader_info(geometryShader, vk::ShaderStageFlagBits::eFragment);
        shaderStages[1] = geometryShaderInfo;
        pipelineInfo.stageCount = shaderStages.size();
        pipelineInfo.pStages = shaderStages.data();
        pipelineInfo.subpass = 0;
        pipelineInfo.pColorBlendState = &colorBlendState;
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        vk::Pipeline particlePipeline = {};

        if (debugMode) std::cout << "Creating particle Graphics Pipeline " << std::endl;

        try {
            particlePipeline = specification.device.createGraphicsPipeline(nullptr, pipelineInfo).value;
        }
        catch (vk::SystemError err) {
            if (debugMode) std::cout << "Failed create particle Graphic Pipeline!" << std::endl;
        }



        //compute
        vk::PipelineLayout particleComputePipelineLayout = create_pipeline_layout(specification.device, specification.particleComputeDescriptorSetLayout, debugMode);
        vk::ComputePipelineCreateInfo computePipelineCreateInfo = {};
        computePipelineCreateInfo.sType = vk::StructureType::eComputePipelineCreateInfo;
        computePipelineCreateInfo.layout = particleComputePipelineLayout;
        computePipelineCreateInfo.flags = {};
        //
         //computePipelineCreateInfo.stage

       vk::PipelineShaderStageCreateInfo cxomputeshaderStages;

        vk::ShaderModule computeShader = vkUtil::createModule(specification.computeFilePath, specification.device, debugMode);
        vk::PipelineShaderStageCreateInfo computeShaderInfo = make_shader_info(computeShader, vk::ShaderStageFlagBits::eCompute);
        cxomputeshaderStages = (computeShaderInfo);
        computePipelineCreateInfo.stage = cxomputeshaderStages;
        vk::Pipeline computePipeline = {};
        if (debugMode) std::cout << "Creating compute particle Pipeline " << std::endl;
        try {
            computePipeline = specification.device.createComputePipeline(nullptr, computePipelineCreateInfo).value;
        }
        catch (vk::SystemError err) {
            if (debugMode) std::cout << "Failed create particle Compute Pipeline!" << std::endl;
        }

        specification.device.destroyShaderModule(geometryShader);
        specification.device.destroyShaderModule(vertexShader);
        specification.device.destroyShaderModule(computeShader);

        ParticleGraphicsPipelineOutBundle output;
        output.particleComputePipeline = computePipeline;
        output.particleGrphicPipeline = particlePipeline;
        output.particlePipelineLayout = particleGraphicPipelineLayout;
        output.particleComputePipelineLayout = particleComputePipelineLayout;
        output.particleRenderPass = renderpass;
        return output;
    }



    PostProcessGraphicsPipelineOutBundle create_postprocess_pipelines(PostProcessPipelineInBundle specyfication, bool debugMode)
    {
        PostProcessGraphicsPipelineOutBundle output;

        vk::PipelineLayout skyBoxPipelineLayout = create_pipeline_layout(specyfication.device, specyfication.postProcessSetLayout, debugMode);
        
        vk::PipelineInputAssemblyStateCreateInfo inputAssemblyState = make_input_assembly_info();
        vk::PipelineRasterizationStateCreateInfo rasterizationState = make_rasterizer_info();
        vk::PipelineColorBlendAttachmentState blendAttachmentState = make_color_blend_attachment_state();
        vk::PipelineColorBlendStateCreateInfo colorBlendState = make_color_blend_attachment_stage(blendAttachmentState);
        vk::PipelineDepthStencilStateCreateInfo depthStageInfo;
        depthStageInfo.flags = vk::PipelineDepthStencilStateCreateFlags();
        depthStageInfo.depthTestEnable = false;
        depthStageInfo.depthWriteEnable = false;
        depthStageInfo.depthCompareOp = vk::CompareOp::eLess;
        depthStageInfo.depthBoundsTestEnable = false;
        depthStageInfo.stencilTestEnable = false;
        //Viewport and Scissor
        vk::Rect2D scissor = {};
        scissor.offset.x = 0.0f;
        scissor.offset.y = 0.0f;
        scissor.extent.width = specyfication.swapchainExtent.width;
        scissor.extent.height = specyfication.swapchainExtent.height;
        vk::Viewport viewport = {};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)specyfication.swapchainExtent.width;
        viewport.height = (float)specyfication.swapchainExtent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vk::PipelineViewportStateCreateInfo viewportState = make_viewport_state(viewport, scissor);
        vk::PipelineMultisampleStateCreateInfo multisampleState = make_multisampling_info();
        std::array<vk::PipelineShaderStageCreateInfo, 2> shaderStages;

        vk::RenderPass renderpass = vkInit::create_combinedImage_renderpass(specyfication.device, specyfication.swapchainImageFormat);

        vk::GraphicsPipelineCreateInfo pipelineInfo = {};
        pipelineInfo.pInputAssemblyState = &inputAssemblyState;
        pipelineInfo.renderPass = renderpass;
        pipelineInfo.pRasterizationState = &rasterizationState;
        pipelineInfo.pColorBlendState = &colorBlendState;
        pipelineInfo.pMultisampleState = &multisampleState;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pDepthStencilState = &depthStageInfo;

        /////////////////////////////////////////////////////////////
        //offscreen
        pipelineInfo.layout = skyBoxPipelineLayout;
        vk::PipelineVertexInputStateCreateInfo emptyInputState = {};
        pipelineInfo.pVertexInputState = &emptyInputState;

        // Offscreen pipeline
        vk::ShaderModule vertexShader = vkUtil::createModule(specyfication.vertexFilePath, specyfication.device, debugMode);
        vk::PipelineShaderStageCreateInfo vertexShaderInfo = make_shader_info(vertexShader, vk::ShaderStageFlagBits::eVertex);
        shaderStages[0] = (vertexShaderInfo);
        vk::ShaderModule fragmentShader = vkUtil::createModule(specyfication.fragmentFilePath, specyfication.device, debugMode);
        vk::PipelineShaderStageCreateInfo fragmentShaderInfo = make_shader_info(fragmentShader, vk::ShaderStageFlagBits::eFragment);
        shaderStages[1] = (fragmentShaderInfo);

        // Blend attachment states required for all color attachments
        // This is important, as color write mask will otherwise be 0x0 and you
        // won't see anything rendered to the attachment
    
   
      
        pipelineInfo.stageCount = shaderStages.size();
        pipelineInfo.pStages = shaderStages.data();
        pipelineInfo.subpass = 0;
        vk::Pipeline skyBoxPipeline = {};

        pipelineInfo.basePipelineHandle = nullptr;
        if (debugMode) std::cout << "Creating postprocess Graphics Pipeline " << std::endl;
        try {
            skyBoxPipeline = specyfication.device.createGraphicsPipeline(nullptr, pipelineInfo).value;
        }
        catch (vk::SystemError err) {
            if (debugMode) std::cout << "Failed create postprocess Graphics Pipeline!" << std::endl;
        }

 

        specyfication.device.destroyShaderModule(vertexShader);
        specyfication.device.destroyShaderModule(fragmentShader);
       
        output.renderpass = renderpass;
        output.postProcessPipelineLayout = skyBoxPipelineLayout;
        output.postProcessgraphicsPipeline = skyBoxPipeline;

        return output;
    }

    skyBoxGraphicsPipelineOutBundle create_skybox_pipeline(skyBoxPipelineInBundle specyfication, bool debugMode)
    {
        skyBoxGraphicsPipelineOutBundle output;

        vk::PipelineLayout skyBoxPipelineLayout = create_pipeline_layout(specyfication.device, specyfication.skyBoxSetLayout, debugMode);

        vk::PipelineInputAssemblyStateCreateInfo inputAssemblyState = make_input_assembly_info();
        vk::PipelineRasterizationStateCreateInfo rasterizationState = make_rasterizer_info();
        rasterizationState.cullMode = vk::CullModeFlagBits::eFront;
        vk::PipelineColorBlendAttachmentState blendAttachmentState = make_color_blend_attachment_state();
        vk::PipelineColorBlendStateCreateInfo colorBlendState = make_color_blend_attachment_stage(blendAttachmentState);
        vk::PipelineDepthStencilStateCreateInfo depthStageInfo;
        depthStageInfo.flags = vk::PipelineDepthStencilStateCreateFlags();
        depthStageInfo.depthTestEnable = true;
        depthStageInfo.depthWriteEnable = true;
        depthStageInfo.depthCompareOp = vk::CompareOp::eLessOrEqual;
        depthStageInfo.depthBoundsTestEnable = false;
        depthStageInfo.stencilTestEnable = false;
        //Viewport and Scissor
        vk::Rect2D scissor = {};
        scissor.offset.x = 0.0f;
        scissor.offset.y = 0.0f;
        scissor.extent.width = specyfication.swapchainExtent.width;
        scissor.extent.height = specyfication.swapchainExtent.height;
        vk::Viewport viewport = {};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)specyfication.swapchainExtent.width;
        viewport.height = (float)specyfication.swapchainExtent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vk::PipelineViewportStateCreateInfo viewportState = make_viewport_state(viewport, scissor);
        vk::PipelineMultisampleStateCreateInfo multisampleState = make_multisampling_info();
        std::array<vk::PipelineShaderStageCreateInfo, 2> shaderStages;

        vk::RenderPass renderpass = vkInit::create_skybox_renderpass(specyfication.device, specyfication.swapchainImageFormat,specyfication.depthFormat,specyfication.Attachment);

        vk::GraphicsPipelineCreateInfo pipelineInfo = {};
        pipelineInfo.pInputAssemblyState = &inputAssemblyState;
        pipelineInfo.renderPass = renderpass;
        pipelineInfo.pRasterizationState = &rasterizationState;
        pipelineInfo.pColorBlendState = &colorBlendState;
        pipelineInfo.pMultisampleState = &multisampleState;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pDepthStencilState = &depthStageInfo;

        /////////////////////////////////////////////////////////////
        //offscreen
        pipelineInfo.layout = skyBoxPipelineLayout;
        vk::VertexInputBindingDescription bindingDescription = vkMesh::getMeshVBO();
        std::vector <vk::VertexInputAttributeDescription> attributeDescriptions = vkMesh::getMeshVAO();
        vk::PipelineVertexInputStateCreateInfo vertexInputInfo = make_vertex_input_info(bindingDescription, attributeDescriptions);
        pipelineInfo.pVertexInputState = &vertexInputInfo;

        // Offscreen pipeline
        vk::ShaderModule vertexShader = vkUtil::createModule(specyfication.vertexFilePath, specyfication.device, debugMode);
        vk::PipelineShaderStageCreateInfo vertexShaderInfo = make_shader_info(vertexShader, vk::ShaderStageFlagBits::eVertex);
        shaderStages[0] = (vertexShaderInfo);
        vk::ShaderModule fragmentShader = vkUtil::createModule(specyfication.fragmentFilePath, specyfication.device, debugMode);
        vk::PipelineShaderStageCreateInfo fragmentShaderInfo = make_shader_info(fragmentShader, vk::ShaderStageFlagBits::eFragment);
        shaderStages[1] = (fragmentShaderInfo);

        // Blend attachment states required for all color attachments
        // This is important, as color write mask will otherwise be 0x0 and you
        // won't see anything rendered to the attachment



        pipelineInfo.stageCount = shaderStages.size();
        pipelineInfo.pStages = shaderStages.data();
        pipelineInfo.subpass = 0;
        vk::Pipeline skyBoxPipeline = {};

        pipelineInfo.basePipelineHandle = nullptr;
        if (debugMode) std::cout << "Creating SkyBox Graphics Pipeline " << std::endl;
        try {
            skyBoxPipeline = specyfication.device.createGraphicsPipeline(nullptr, pipelineInfo).value;
        }
        catch (vk::SystemError err) {
            if (debugMode) std::cout << "Failed create skyBox Graphics Pipeline!" << std::endl;
        }



        specyfication.device.destroyShaderModule(vertexShader);
        specyfication.device.destroyShaderModule(fragmentShader);

        output.renderpass = renderpass;
        output.skyBoxPipelineLayout = skyBoxPipelineLayout;
        output.skyBoxgraphicsPipeline = skyBoxPipeline;

        return output;
    }


    updownGraphicsPipelineOutBundle create_updownsampling_pipeline(updownGraphicsPipelineInBundle specyfication, bool debugMode)
    {
        updownGraphicsPipelineOutBundle output;

        vk::PipelineLayout downScale = create_pipeline_layout(specyfication.device, specyfication.downSampleDescriptorSetLayout, debugMode);
        vk::PipelineLayout upScale = create_pipeline_layout(specyfication.device, specyfication.upSampleDescriptorSetLayout, debugMode);

        vk::PipelineInputAssemblyStateCreateInfo inputAssemblyState = make_input_assembly_info();
        vk::PipelineRasterizationStateCreateInfo rasterizationState = make_rasterizer_info();
        rasterizationState.cullMode = vk::CullModeFlagBits::eFront;
        vk::PipelineColorBlendAttachmentState blendAttachmentState = make_color_blend_attachment_state();
        vk::PipelineColorBlendStateCreateInfo colorBlendState = make_color_blend_attachment_stage(blendAttachmentState);

   


        vk::PipelineDepthStencilStateCreateInfo depthStageInfo;
        depthStageInfo.flags = vk::PipelineDepthStencilStateCreateFlags();
        depthStageInfo.depthTestEnable = false;
        depthStageInfo.depthWriteEnable = false;
        depthStageInfo.depthCompareOp = vk::CompareOp::eLessOrEqual;
        depthStageInfo.depthBoundsTestEnable = false;
        depthStageInfo.stencilTestEnable = false;
        //Viewport and Scissor
        
        vk::PipelineMultisampleStateCreateInfo multisampleState = make_multisampling_info();
        std::array<vk::PipelineShaderStageCreateInfo, 2> shaderStages;

        vk::RenderPass renderpass = vkInit::create_updownscale_renderpass(specyfication.device);

        vk::GraphicsPipelineCreateInfo pipelineInfo = {};
        pipelineInfo.pInputAssemblyState = &inputAssemblyState;
        pipelineInfo.renderPass = renderpass;
        pipelineInfo.pRasterizationState = &rasterizationState;
        pipelineInfo.pColorBlendState = &colorBlendState;
        pipelineInfo.pMultisampleState = &multisampleState;
        
        pipelineInfo.pDepthStencilState = &depthStageInfo;
       

        /////////////////////////////////////////////////////////////
        //offscreen
        
        vk::PipelineVertexInputStateCreateInfo emptyInputState = {};
        pipelineInfo.pVertexInputState = &emptyInputState;

        // Offscreen pipeline
        vk::ShaderModule downScalevertexShader = vkUtil::createModule(specyfication.downScalevertexFilePath, specyfication.device, debugMode);
        vk::PipelineShaderStageCreateInfo downScalevertexShaderInfo = make_shader_info(downScalevertexShader, vk::ShaderStageFlagBits::eVertex);
        shaderStages[0] = (downScalevertexShaderInfo);
        vk::ShaderModule downScalefragmentShader = vkUtil::createModule(specyfication.downScalefragmentFilePath, specyfication.device, debugMode);
        vk::PipelineShaderStageCreateInfo downScalefragmentShaderInfo = make_shader_info(downScalefragmentShader, vk::ShaderStageFlagBits::eFragment);
        shaderStages[1] = (downScalefragmentShaderInfo);
        pipelineInfo.layout = downScale;
        // Blend attachment states required for all color attachments
        // This is important, as color write mask will otherwise be 0x0 and you
        // won't see anything rendered to the attachment



        pipelineInfo.stageCount = shaderStages.size();
        pipelineInfo.pStages = shaderStages.data();
        
        pipelineInfo.basePipelineHandle = nullptr;
        std::vector<vk::Pipeline> downsampepipelines;
        std::vector<vk::Pipeline> upsampepipelines;

        for (uint32_t i = 0; i < specyfication.screenSize.size(); ++i) {
            vk::Rect2D scissor = {};
            scissor.offset.x = 0.0f;
            scissor.offset.y = 0.0f;
            scissor.extent.width = specyfication.screenSize[i].x;
            scissor.extent.height = specyfication.screenSize[i].y;
            vk::Viewport viewport = {};
            viewport.x = 0.0f;
            viewport.y = 0.0f;
            viewport.width = (float)specyfication.screenSize[i].x;
            viewport.height = (float)specyfication.screenSize[i].y;
            viewport.minDepth = 0.0f;
            viewport.maxDepth = 1.0f;

            vk::PipelineViewportStateCreateInfo viewportState = make_viewport_state(viewport, scissor);
            pipelineInfo.subpass = i;
            pipelineInfo.pViewportState = &viewportState;
            if (debugMode) std::cout << "Creating downScale "<<i<<" Graphics Pipeline " << std::endl;
            try {
                
               downsampepipelines.push_back(specyfication.device.createGraphicsPipeline(nullptr, pipelineInfo).value);
            }
            catch (vk::SystemError err) {
                if (debugMode) std::cout << "Failed create downScale " << i << " Graphics Pipeline !" << std::endl;
            }
        }


        vk::ShaderModule upScalevertexShader = vkUtil::createModule(specyfication.upScalevertexFilePath, specyfication.device, debugMode);
        vk::PipelineShaderStageCreateInfo upScalevertexShaderInfo = make_shader_info(upScalevertexShader, vk::ShaderStageFlagBits::eVertex);
        shaderStages[0] = (upScalevertexShaderInfo);
        vk::ShaderModule upScalefragmentShader = vkUtil::createModule(specyfication.upScalefragmentFilePath, specyfication.device, debugMode);
        vk::PipelineShaderStageCreateInfo upScalefragmentShaderInfo = make_shader_info(upScalefragmentShader, vk::ShaderStageFlagBits::eFragment);
        shaderStages[1] = (upScalefragmentShaderInfo);
        pipelineInfo.layout = upScale;


        for (uint32_t i = 0; i < specyfication.screenSize.size(); ++i) {
            vk::Rect2D scissor = {};
            scissor.offset.x = 0.0f;
            scissor.offset.y = 0.0f;
            scissor.extent.width = specyfication.screenSize[i].x;
            scissor.extent.height = specyfication.screenSize[i].y;
            vk::Viewport viewport = {};
            viewport.x = 0.0f;
            viewport.y = 0.0f;
            viewport.width = (float)specyfication.screenSize[i].x;
            viewport.height = (float)specyfication.screenSize[i].y;
            viewport.minDepth = 0.0f;
            viewport.maxDepth = 1.0f;

            vk::PipelineViewportStateCreateInfo viewportState = make_viewport_state(viewport, scissor);
            pipelineInfo.subpass = 6 + specyfication.screenSize.size() - i-1;
            pipelineInfo.pViewportState = &viewportState;
            if (debugMode) std::cout << "Creating upScale " << i << " Graphics Pipeline " << std::endl;
           // if (debugMode) std::cout << "Creating upScale subpass" << 6 + specyfication.screenSize.size() - i - 1 << " Graphics Pipeline " << std::endl;
            try {

                upsampepipelines.push_back(specyfication.device.createGraphicsPipeline(nullptr, pipelineInfo).value);
            }
            catch (vk::SystemError err) {
                if (debugMode) std::cout << "Failed create upScale " << i << " Graphics Pipeline !" << std::endl;
            }
        }
        

        



        specyfication.device.destroyShaderModule(downScalevertexShader);
        specyfication.device.destroyShaderModule(upScalevertexShader);
        specyfication.device.destroyShaderModule(downScalefragmentShader);
        specyfication.device.destroyShaderModule(upScalefragmentShader);

        output.renderpass = renderpass;
        output.downscalePipelineLayout = downScale;
        output.upscalePipelineLayout = upScale;
        output.downscalePipeline = downsampepipelines;
        output.upscaleGrphicPipeline = upsampepipelines;
        return output;
    }

}

