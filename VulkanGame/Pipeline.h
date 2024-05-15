#pragma once
#include "config.h"
#include "Shaders.h"
#include "RenderStruct.h"
#include "Mesh.h"
#include "RenderStruct.h"
namespace vkInit {
	struct GraphicsPipelineInBundle {
		vk::Device device;
		std::string vertexFilePath;
		std::string fragmentFilePath;
        vk::Extent2D swapchainExtent;
        vk::Format swapchainImageFormat, depthFormat;
        std::vector<vk::DescriptorSetLayout> descriptorSetLayouts;
	};
	struct GraphicsPipelineOutBundle {
        vk::PipelineLayout layout;
        vk::RenderPass renderpass;
        vk::Pipeline graphicsPipeline;
	};
    vk::PipelineLayout create_pipeline_layout(vk::Device device, std::vector<vk::DescriptorSetLayout> descriptorSetLayouts ,bool debugMode);
    vk::RenderPass create_renderpass(vk::Device device, vk::Format swapchainImageFormat, vk::Format depthFormat, bool debugMode);
    GraphicsPipelineOutBundle create_graphic_pipeline(GraphicsPipelineInBundle specyfication, bool debugMode);
    vk::PipelineInputAssemblyStateCreateInfo make_input_assembly_info();
    vk::PipelineShaderStageCreateInfo make_shader_info(
        const vk::ShaderModule& shaderModule, const vk::ShaderStageFlagBits& stage);

    vk::PipelineVertexInputStateCreateInfo make_vertex_input_info(
        const vk::VertexInputBindingDescription& bindingDescription,
        const std::vector<vk::VertexInputAttributeDescription>& attributeDescriptions);

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
    vk::PipelineDepthStencilStateCreateInfo makePipelineDepthStencilStageCreateInfo();
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
    vk::RenderPassCreateInfo makeDeferedRenderpassInfo(const vk::Format& swapchainImageFormat, vkUtil::GbufferAttachments gAttachments, const vk::Format& depth);
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
        vk::PipelineLayout pipelineLayout = create_pipeline_layout(specyfication.device, specyfication.descriptorSetLayouts,debugMode);
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

    
    vk::RenderPassCreateInfo makeDeferedRenderpassInfo(const vk::Format& swapchainImageFormat, vkUtil::GbufferAttachments gAttachments, const vk::Format& depth) {

        std::array<vk::AttachmentDescription, 5> attachments{};
        // Color attachment
        vk::AttachmentDescription colorAttachment = {};
        attachments[0].flags = vk::AttachmentDescriptionFlags();
        attachments[0].format = swapchainImageFormat;
        attachments[0].samples = vk::SampleCountFlagBits::e1;
        attachments[0].loadOp = vk::AttachmentLoadOp::eClear;
        attachments[0].storeOp = vk::AttachmentStoreOp::eStore;
        attachments[0].stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
        attachments[0].stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
        attachments[0].initialLayout = vk::ImageLayout::eUndefined;
        attachments[0].finalLayout = vk::ImageLayout::ePresentSrcKHR;

        // Deferred attachments
        // Position
        attachments[1].format = gAttachments.position.format;
        attachments[1].samples = vk::SampleCountFlagBits::e1;
        attachments[1].loadOp = vk::AttachmentLoadOp::eClear;
        attachments[1].storeOp = vk::AttachmentStoreOp::eDontCare;
        attachments[1].stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
        attachments[1].stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
        attachments[1].initialLayout = vk::ImageLayout::eUndefined;
        attachments[1].finalLayout = vk::ImageLayout::eColorAttachmentOptimal;
        // Normals
        attachments[2].format = gAttachments.normal.format;
        attachments[2].samples = vk::SampleCountFlagBits::e1;
        attachments[2].loadOp = vk::AttachmentLoadOp::eClear;
        attachments[2].storeOp = vk::AttachmentStoreOp::eDontCare;
        attachments[2].stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
        attachments[2].stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
        attachments[2].initialLayout = vk::ImageLayout::eUndefined;
        attachments[2].finalLayout = vk::ImageLayout::eColorAttachmentOptimal;
        // Albedo
        attachments[3].format = gAttachments.albedo.format;
        attachments[3].samples = vk::SampleCountFlagBits::e1;
        attachments[3].loadOp = vk::AttachmentLoadOp::eClear;
        attachments[3].storeOp = vk::AttachmentStoreOp::eDontCare;
        attachments[3].stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
        attachments[3].stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
        attachments[3].initialLayout = vk::ImageLayout::eUndefined;
        attachments[3].finalLayout = vk::ImageLayout::eColorAttachmentOptimal;
        // Depth attachment
        attachments[4].format = depth;
        attachments[4].samples = vk::SampleCountFlagBits::e1;
        attachments[4].loadOp = vk::AttachmentLoadOp::eClear;
        attachments[4].storeOp = vk::AttachmentStoreOp::eStore;
        attachments[4].stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
        attachments[4].stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
        attachments[4].initialLayout = vk::ImageLayout::eUndefined;
        attachments[4].finalLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

        // Three subpasses
        std::array<vk::SubpassDescription, 2> subpassDescriptions{};

        // First subpass: Fill G-Buffer components
        // ----------------------------------------------------------------------------------------

        vk::AttachmentReference colorAttachmentReferences[4];
        colorAttachmentReferences[0].layout = vk::ImageLayout::eColorAttachmentOptimal;
        colorAttachmentReferences[0].attachment = 0;
        colorAttachmentReferences[1].layout = vk::ImageLayout::eColorAttachmentOptimal;
        colorAttachmentReferences[1].attachment = 1;
        colorAttachmentReferences[2].layout = vk::ImageLayout::eColorAttachmentOptimal;
        colorAttachmentReferences[2].attachment = 2;
        colorAttachmentReferences[3].layout = vk::ImageLayout::eColorAttachmentOptimal;
        colorAttachmentReferences[3].attachment = 3;

   
        vk::AttachmentReference depthAttachmentRef = {};
        depthAttachmentRef.attachment = 4;
        depthAttachmentRef.layout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

        subpassDescriptions[0].pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
        subpassDescriptions[0].colorAttachmentCount = 4;
        subpassDescriptions[0].pColorAttachments = colorAttachmentReferences;
        subpassDescriptions[0].pDepthStencilAttachment = &depthAttachmentRef;

        // Second subpass: Final composition (using G-Buffer components)
        // ----------------------------------------------------------------------------------------

        vk::AttachmentReference colorReference = { };
        colorReference.attachment = 0;
        colorReference.layout = vk::ImageLayout::eColorAttachmentOptimal;
        vk::AttachmentReference inputReferences[3];
        inputReferences[0].attachment = 1;
        inputReferences[0].layout = vk::ImageLayout::eShaderReadOnlyOptimal;
        inputReferences[1].attachment = 2;
        inputReferences[1].layout = vk::ImageLayout::eShaderReadOnlyOptimal;
        inputReferences[2].attachment = 3;
        inputReferences[2].layout = vk::ImageLayout::eShaderReadOnlyOptimal;

        subpassDescriptions[1].pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
        subpassDescriptions[1].colorAttachmentCount = 1;
        subpassDescriptions[1].pColorAttachments = &colorReference;
        subpassDescriptions[1].pDepthStencilAttachment = &depthAttachmentRef;
        // Use the color attachments filled in the first pass as input attachments
        subpassDescriptions[1].inputAttachmentCount = 3;
        subpassDescriptions[1].pInputAttachments = inputReferences;

        // Third subpass: Forward transparency
        // ----------------------------------------------------------------------------------------
    

        // Subpass dependencies for layout transitions
        std::array<vk::SubpassDependency, 3> dependencies;

        // This makes sure that writes to the depth image are done before we try to write to it again
        dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
        dependencies[0].dstSubpass = 0;
        dependencies[0].srcStageMask =  vk::PipelineStageFlagBits::eEarlyFragmentTests | vk::PipelineStageFlagBits::eLateFragmentTests;;
        dependencies[0].dstStageMask = vk::PipelineStageFlagBits::eEarlyFragmentTests | vk::PipelineStageFlagBits::eLateFragmentTests;;
        dependencies[0].srcAccessMask = vk::AccessFlagBits(0);
        dependencies[0].dstAccessMask = vk::AccessFlagBits::eDepthStencilAttachmentWrite;
        dependencies[0].dependencyFlags = vk::DependencyFlagBits(0);

        dependencies[1].srcSubpass = VK_SUBPASS_EXTERNAL;
        dependencies[1].dstSubpass = 0;
        dependencies[1].srcStageMask =  vk::PipelineStageFlagBits::eColorAttachmentOutput;
        dependencies[1].dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
        dependencies[1].srcAccessMask = vk::AccessFlagBits(0);
        dependencies[1].dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
        dependencies[1].dependencyFlags = vk::DependencyFlagBits(0);

        // This dependency transitions the input attachment from color attachment to input attachment read
        dependencies[2].srcSubpass = 0;
        dependencies[2].dstSubpass = 1;
        dependencies[2].srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
        dependencies[2].dstStageMask =  vk::PipelineStageFlagBits::eFragmentShader;
        dependencies[2].srcAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
        dependencies[2].dstAccessMask =  vk::AccessFlagBits::eInputAttachmentRead;
        dependencies[2].dependencyFlags = vk::DependencyFlagBits::eByRegion;

        vk::RenderPassCreateInfo renderPassInfo = {};
        renderPassInfo.sType = vk::StructureType::eRenderPassCreateInfo;
        renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        renderPassInfo.pAttachments = attachments.data();
        renderPassInfo.subpassCount = static_cast<uint32_t>(subpassDescriptions.size());
        renderPassInfo.pSubpasses = subpassDescriptions.data();
        renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
        renderPassInfo.pDependencies = dependencies.data();

        return renderPassInfo;
    }


    void createPipeline(vk::Device logicalDevice, const GraphicsPipelineInBundle& specification, vk::RenderPass renderPass, std::vector<vk::DescriptorSetLayout> geometryDescriptorSetLayouts, std::vector<vk::DescriptorSetLayout> deferedDescriptorSetLayouts, std::vector<vk::DescriptorSetLayout>  transparentDescriptorSetLayouts) {
        // Layout
        vk::PipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
        pipelineLayoutCreateInfo.setLayoutCount = static_cast<uint32_t>(geometryDescriptorSetLayouts.size());
        pipelineLayoutCreateInfo.pSetLayouts = geometryDescriptorSetLayouts.data();
        pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
        vk::PipelineLayout geometryPipelineLayout = logicalDevice.createPipelineLayout(pipelineLayoutCreateInfo);
       

        // Pipeline
        vk::PipelineInputAssemblyStateCreateInfo inputAssemblyState = make_input_assembly_info();
        vk::PipelineRasterizationStateCreateInfo rasterizationState = make_rasterizer_info();

        vk::PipelineColorBlendAttachmentState blendAttachmentState = make_color_blend_attachment_state();
        vk::PipelineColorBlendStateCreateInfo colorBlendState = make_color_blend_attachment_stage(blendAttachmentState);
        vk::PipelineDepthStencilStateCreateInfo depthStencilState = makePipelineDepthStencilStageCreateInfo();
        vk::Viewport viewport = make_viewport(specification);
        vk::Rect2D scissor = make_scissor(specification);
        vk::PipelineViewportStateCreateInfo viewportState = make_viewport_state(viewport,scissor);
        vk::PipelineMultisampleStateCreateInfo multisampleState = make_multisampling_info();
        //std::vector<vk::DynamicState> dynamicStateEnables = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR }; Maybe useful
        //vk::PipelineDynamicStateCreateInfo dynamicState = vks::initializers::pipelineDynamicStateCreateInfo(dynamicStateEnables);
        std::array<vk::PipelineShaderStageCreateInfo, 2> shaderStages;
        vk::VertexInputBindingDescription bindingDescription = vkMesh::getPosColBindingDescription();
        std::vector <vk::VertexInputAttributeDescription> attributeDescriptions = vkMesh::getPosColorAttributeDescriptions();
        vk::PipelineVertexInputStateCreateInfo vertexInputInfo = make_vertex_input_info(bindingDescription, attributeDescriptions);
        // Final fullscreen pass pipeline


          // Offscreen scene rendering pipeline
        vk::ShaderModule vertexShader = vkUtil::createModule(specification.vertexFilePath, specification.device, true); ////////////////////////////////////////////////////
        vk::PipelineShaderStageCreateInfo vertexShaderInfo = make_shader_info(vertexShader, vk::ShaderStageFlagBits::eVertex);
        shaderStages[0] = vertexShaderInfo;

        vk::ShaderModule fragmentShader = vkUtil::createModule(specification.fragmentFilePath, specification.device, true);
        vk::PipelineShaderStageCreateInfo fragmentShaderInfo = make_shader_info(fragmentShader, vk::ShaderStageFlagBits::eFragment);
        shaderStages[1] = fragmentShaderInfo;

        vk::GraphicsPipelineCreateInfo pipelineInfo = {};
        pipelineInfo.flags = vk::PipelineCreateFlags();
        pipelineInfo.layout = geometryPipelineLayout;
        pipelineInfo.renderPass = renderPass;
        pipelineInfo.pInputAssemblyState = &inputAssemblyState;
        pipelineInfo.pRasterizationState = &rasterizationState;
        pipelineInfo.pColorBlendState = &colorBlendState;
        pipelineInfo.pMultisampleState = &multisampleState;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pDepthStencilState = &depthStencilState;
        pipelineInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
        pipelineInfo.pStages = shaderStages.data();
        pipelineInfo.subpass = 0;
        pipelineInfo.pVertexInputState = &vertexInputInfo;

        std::array<vk::PipelineColorBlendAttachmentState, 4> blendAttachmentStates = makeDeferedAttachmentState();

        colorBlendState.attachmentCount = static_cast<uint32_t>(blendAttachmentStates.size());
        colorBlendState.pAttachments = blendAttachmentStates.data();
        vk::Pipeline geometryPipeline = logicalDevice.createGraphicsPipeline(nullptr,pipelineInfo).value;

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Descriptor set layout

        /*
        VkDescriptorSetLayoutCreateInfo descriptorLayout =
            vks::initializers::descriptorSetLayoutCreateInfo(
                setLayoutBindings.data(),
                static_cast<uint32_t>(setLayoutBindings.size()));

        VK_CHECK_RESULT(vkCreateDescriptorSetLayout(device, &descriptorLayout, nullptr, &descriptorSetLayouts.composition));  //nie musze tak tego robic

        */

        // Pipeline layout
        vk::PipelineLayoutCreateInfo deferedPipelineLayoutCreateInfo = {};
        pipelineLayoutCreateInfo.setLayoutCount = static_cast<uint32_t>(deferedDescriptorSetLayouts.size());
        pipelineLayoutCreateInfo.pSetLayouts = deferedDescriptorSetLayouts.data();
        pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
        vk::PipelineLayout deferedPipelineLayout = logicalDevice.createPipelineLayout(deferedPipelineLayoutCreateInfo);
        /*
        // Descriptor sets
        VkDescriptorSetAllocateInfo allocInfo = vks::initializers::descriptorSetAllocateInfo(descriptorPool, &descriptorSetLayouts.composition, 1);
        VK_CHECK_RESULT(vkAllocateDescriptorSets(device, &allocInfo, &descriptorSets.composition));

        // Image descriptors for the offscreen color attachments
        VkDescriptorImageInfo texDescriptorPosition = vks::initializers::descriptorImageInfo(VK_NULL_HANDLE, attachments.position.view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        VkDescriptorImageInfo texDescriptorNormal = vks::initializers::descriptorImageInfo(VK_NULL_HANDLE, attachments.normal.view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        VkDescriptorImageInfo texDescriptorAlbedo = vks::initializers::descriptorImageInfo(VK_NULL_HANDLE, attachments.albedo.view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        std::vector<VkWriteDescriptorSet> writeDescriptorSets = {
            // Binding 0: Position texture target
            vks::initializers::writeDescriptorSet(descriptorSets.composition, VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 0, &texDescriptorPosition),
            // Binding 1: Normals texture target
            vks::initializers::writeDescriptorSet(descriptorSets.composition, VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1, &texDescriptorNormal),
            // Binding 2: Albedo texture target
            vks::initializers::writeDescriptorSet(descriptorSets.composition, VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 2, &texDescriptorAlbedo),
            // Binding 4: Fragment shader lights
            vks::initializers::writeDescriptorSet(descriptorSets.composition, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 3, &buffers.lights.descriptor),
        };
        
        vkUpdateDescriptorSets(logical, static_cast<uint32_t>(writeDescriptorSets.size()), writeDescriptorSets.data(), 0, NULL);
        */
        // Pipeline
        vk::PipelineInputAssemblyStateCreateInfo deferedInputAssemblyState = make_input_assembly_info(); 
        vk::PipelineRasterizationStateCreateInfo deferedRasterizationState = make_rasterizer_info(); 
        vk::PipelineColorBlendAttachmentState deferedBlendAttachmentState = make_color_blend_attachment_state(); 
        vk::PipelineColorBlendStateCreateInfo deferedColorBlendState = make_color_blend_attachment_stage(deferedBlendAttachmentState);
        vk::PipelineDepthStencilStateCreateInfo deferedDepthStencilState = makePipelineDepthStencilStageCreateInfo();
        vk::PipelineViewportStateCreateInfo deferedViewportState = make_viewport_state(viewport, scissor);
        vk::PipelineMultisampleStateCreateInfo deferedMultisampleState = make_multisampling_info();
        std::array<vk::PipelineShaderStageCreateInfo, 2> deferedShaderStages;

        // Offscreen scene rendering pipeline
        vk::ShaderModule deferedVertexShader = vkUtil::createModule(specification.vertexFilePath, specification.device, true); ////////////////////////////////////////////////////
        vk::PipelineShaderStageCreateInfo deferedVertexShaderInfo = make_shader_info(deferedVertexShader, vk::ShaderStageFlagBits::eVertex);
        deferedShaderStages[0] = deferedVertexShaderInfo;

        vk::ShaderModule deferedFragmentShader = vkUtil::createModule(specification.fragmentFilePath, specification.device, true);
        vk::PipelineShaderStageCreateInfo deferedFragmentShaderInfo = make_shader_info(deferedFragmentShader, vk::ShaderStageFlagBits::eFragment);
        deferedShaderStages[1] = deferedFragmentShaderInfo;

        vk::GraphicsPipelineCreateInfo deferedPipelineCI = {};

        vk::PipelineVertexInputStateCreateInfo emptyInputState{};
        emptyInputState.sType = vk::StructureType::ePipelineVertexInputStateCreateInfo;
        deferedPipelineCI.flags = vk::PipelineCreateFlags();
        deferedPipelineCI.renderPass = renderPass;
        deferedPipelineCI.pVertexInputState = &emptyInputState;
        deferedPipelineCI.pInputAssemblyState = &deferedInputAssemblyState;
        deferedPipelineCI.pRasterizationState = &deferedRasterizationState;
        deferedPipelineCI.pColorBlendState = &deferedColorBlendState;
        deferedPipelineCI.pMultisampleState = &deferedMultisampleState;
        deferedPipelineCI.pViewportState = &deferedViewportState;
        deferedPipelineCI.pDepthStencilState = &deferedDepthStencilState;
        deferedPipelineCI.stageCount = static_cast<uint32_t>(deferedShaderStages.size());
        deferedPipelineCI.pStages = deferedShaderStages.data();
        // Index of the subpass that this pipeline will be used in
        deferedPipelineCI.subpass = 1;
        depthStencilState.depthWriteEnable = VK_FALSE;

        vk::Pipeline deferedPipeline = logicalDevice.createGraphicsPipeline(nullptr, deferedPipelineCI).value;



    }

    vk::PipelineDepthStencilStateCreateInfo makePipelineDepthStencilStageCreateInfo() {
        vk::PipelineDepthStencilStateCreateInfo depthStageInfo;
        depthStageInfo.flags = vk::PipelineDepthStencilStateCreateFlags();
        depthStageInfo.depthTestEnable = true;
        depthStageInfo.depthWriteEnable = true;
        depthStageInfo.depthCompareOp = vk::CompareOp::eLess;
        depthStageInfo.depthBoundsTestEnable = false;
        depthStageInfo.stencilTestEnable = false;
        return depthStageInfo;
    }
    std::array<vk::PipelineColorBlendAttachmentState,4> makeDeferedAttachmentState() {

        std::array<vk::PipelineColorBlendAttachmentState, 4> blendAttachmentStates = {};
        blendAttachmentStates[0] = make_color_blend_attachment_state();
        blendAttachmentStates[1] = make_color_blend_attachment_state();
        blendAttachmentStates[2] = make_color_blend_attachment_state();
        blendAttachmentStates[3] = make_color_blend_attachment_state();
        return blendAttachmentStates;

    }
}

