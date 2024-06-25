#include "RenderPass.h"

vk::RenderPass vkInit::create_defered_renderpass(vk::Device logicalDevice,vkUtil::Gbuffer gBuffer, vk::Format swapchainImageFormat, vk::Format depthFormat, vkUtil::FrameBufferAttachment postProcessImageInput)
{


	std::array<vk::AttachmentDescription,8> attachments;
	vk::AttachmentReference attachmentRefertences[8];


	attachments[0].flags = vk::AttachmentDescriptionFlags();
	attachments[0].samples = vk::SampleCountFlagBits::e1;
	attachments[0].loadOp = vk::AttachmentLoadOp::eClear;
	attachments[0].storeOp = vk::AttachmentStoreOp::eStore;
	attachments[0].stencilLoadOp = vk::AttachmentLoadOp::eClear;
	attachments[0].stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
	attachments[0].initialLayout = vk::ImageLayout::eUndefined;
	attachments[0].finalLayout = vk::ImageLayout::eColorAttachmentOptimal;

	attachments[0].format = postProcessImageInput.format;

	//Define a general attachment, with its load/store operations

	
	

	// Init attachment properties
	for (uint32_t i = 1; i < 7; ++i)
	{
		attachments[i].flags = vk::AttachmentDescriptionFlags();
		attachments[i].samples =  vk::SampleCountFlagBits::e1;
		attachments[i].loadOp =  vk::AttachmentLoadOp::eClear;
		attachments[i].storeOp = vk::AttachmentStoreOp::eDontCare;
		attachments[i].stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
		attachments[i].stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
		attachments[i].initialLayout = vk::ImageLayout::eUndefined;
		attachments[i].finalLayout = vk::ImageLayout::eColorAttachmentOptimal;

		if (i == 1) attachments[i].format = gBuffer.position.format;
		else if (i == 2)attachments[i].format = gBuffer.normal.format;
		else if (i == 3) attachments[i].format = gBuffer.albedo.format;
		else if (i==4) attachments[i].format = gBuffer.arm.format;
		else if(i==5)  attachments[i].format = gBuffer.T.format;
		else   attachments[i].format = gBuffer.worldPos.format;
	}


	attachments[7].flags = vk::AttachmentDescriptionFlags();
	attachments[7].format = depthFormat;
	attachments[7].samples = vk::SampleCountFlagBits::e1;
	attachments[7].loadOp = vk::AttachmentLoadOp::eClear;
	attachments[7].storeOp = vk::AttachmentStoreOp::eStore;
	attachments[7].stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
	attachments[7].stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
	attachments[7].initialLayout = vk::ImageLayout::eUndefined;
	attachments[7].finalLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

	


	vk::AttachmentReference colorAttachmentRef = {};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal;
	//Declare that attachment to be color buffer 0 of the framebuffer
	attachmentRefertences[0] = colorAttachmentRef;
	colorAttachmentRef.attachment = 1;
	attachmentRefertences[1] = colorAttachmentRef;
	colorAttachmentRef.attachment = 2;
	attachmentRefertences[2] = colorAttachmentRef;
	colorAttachmentRef.attachment = 3;
	attachmentRefertences[3] = colorAttachmentRef;
	colorAttachmentRef.attachment = 4;
	attachmentRefertences[4] = colorAttachmentRef;
	colorAttachmentRef.attachment = 5;
	attachmentRefertences[5] = colorAttachmentRef;
	colorAttachmentRef.attachment = 6;
	attachmentRefertences[6] = colorAttachmentRef;
	vk::AttachmentReference depthAttachmentRef = {};
	depthAttachmentRef.attachment = 7;
	depthAttachmentRef.layout = vk::ImageLayout::eDepthStencilAttachmentOptimal;



	// Pierwszy subpass: Renderowanie geometrii i wype³nienie za³¹czników position, normal i albedo
	std::vector<vk::SubpassDescription> subpassDescriptions;
	vk::SubpassDescription subpassGeometry = {};
	subpassGeometry.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
	subpassGeometry.colorAttachmentCount = 7; // Liczba za³¹czników kolorowych
	subpassGeometry.pColorAttachments = attachmentRefertences; // Tablica referencji do za³¹czników kolorowych
	subpassGeometry.pDepthStencilAttachment = &depthAttachmentRef; // Za³¹cznik g³êbokoœci i/lub szablonu
	subpassGeometry.inputAttachmentCount = 0;
	subpassGeometry.pInputAttachments = nullptr;
	// Drugi subpass: Obliczenia opóŸnione na podstawie za³¹czników position, normal i albedo

	vk::AttachmentReference colorReference = { };
	colorReference.attachment = 0;
	colorReference.layout = vk::ImageLayout::eColorAttachmentOptimal;
	vk::AttachmentReference inputReferences[6];
	inputReferences[0].attachment = 1;
	inputReferences[0].layout = vk::ImageLayout::eShaderReadOnlyOptimal;
	inputReferences[1].attachment = 2;
	inputReferences[1].layout = vk::ImageLayout::eShaderReadOnlyOptimal;
	inputReferences[2].attachment = 3;
	inputReferences[2].layout = vk::ImageLayout::eShaderReadOnlyOptimal;
	inputReferences[3].attachment = 4;
	inputReferences[3].layout = vk::ImageLayout::eShaderReadOnlyOptimal;
	inputReferences[4].attachment = 5;
	inputReferences[4].layout = vk::ImageLayout::eShaderReadOnlyOptimal;
	inputReferences[5].attachment = 6;
	inputReferences[5].layout = vk::ImageLayout::eShaderReadOnlyOptimal;


	vk::SubpassDescription subpassDeferred = {};
	subpassDeferred.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
	subpassDeferred.inputAttachmentCount = 6; // Liczba za³¹czników wejœciowych
	subpassDeferred.pInputAttachments = inputReferences; // Tablica referencji do za³¹czników wejœciowych
	subpassDeferred.colorAttachmentCount = 1; // Liczba za³¹czników kolorowych wyjœciowych
	subpassDeferred.pColorAttachments = &colorReference; // Tablica referencji do za³¹czników kolorowych wyjœciowych
	subpassDeferred.pDepthStencilAttachment = nullptr;
	
	subpassDescriptions.push_back(subpassGeometry);
	subpassDescriptions.push_back(subpassDeferred);

	// Use subpass dependencies for attachment layout transitions
	std::array<vk::SubpassDependency, 3> dependencies;
	/*
	// Zale¿noœæ dla pierwszego subpassu (Renderowanie geometrii)
	dependencies[0].srcSubpass = vk::SubpassExternal;;
	dependencies[0].dstSubpass = 0;
	dependencies[0].srcStageMask = vk::PipelineStageFlagBits::eEarlyFragmentTests | vk::PipelineStageFlagBits::eLateFragmentTests;
	dependencies[0].dstStageMask = vk::PipelineStageFlagBits::eEarlyFragmentTests | vk::PipelineStageFlagBits::eLateFragmentTests;
	dependencies[0].srcAccessMask = {};
	dependencies[0].dstAccessMask =  vk::AccessFlagBits::eDepthStencilAttachmentWrite;
	dependencies[0].dependencyFlags = {};
	*/
	// Zale¿noœæ dla drugiego subpassu (Obliczenia opóŸnione)
	dependencies[0].srcSubpass = vk::SubpassExternal;
	dependencies[0].dstSubpass = 0;
	dependencies[0].srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
	dependencies[0].dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
	dependencies[0].srcAccessMask = {};
	dependencies[0].dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
	dependencies[0].dependencyFlags = {};

	dependencies[1].srcSubpass = 0;
	dependencies[1].dstSubpass = 1;
	dependencies[1].srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
	dependencies[1].dstStageMask = vk::PipelineStageFlagBits::eFragmentShader;
	dependencies[1].srcAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
	dependencies[1].dstAccessMask = vk::AccessFlagBits::eInputAttachmentRead;
	dependencies[1].dependencyFlags = vk::DependencyFlagBits::eByRegion;

	dependencies[2].srcSubpass = 1;
	dependencies[2].dstSubpass = vk::SubpassExternal;
	dependencies[2].srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
	dependencies[2].dstStageMask = vk::PipelineStageFlagBits::eFragmentShader;
	dependencies[2].srcAccessMask = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite;
	dependencies[2].dstAccessMask = vk::AccessFlagBits::eShaderRead;
	dependencies[2].dependencyFlags = vk::DependencyFlagBits::eByRegion;


	vk::RenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType =  vk::StructureType::eRenderPassCreateInfo;
	renderPassInfo.pAttachments = attachments.data();
	renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	renderPassInfo.subpassCount = subpassDescriptions.size();
	renderPassInfo.pSubpasses = subpassDescriptions.data();
	renderPassInfo.dependencyCount = 3;
	renderPassInfo.pDependencies = dependencies.data();

	vk::RenderPass renderpass= logicalDevice.createRenderPass(renderPassInfo);

	return renderpass;

	
	
}

vk::RenderPass vkInit::create_shadows_renderpass(vk::Device logicalDevice, vkUtil::shadowMapBuffer* shadowmapBuffer, vk::Format depthFormat)
{
	vk::RenderPass shadowRenderPass = {};
	vk::AttachmentDescription shadowAttachment = {};

	shadowAttachment.format = depthFormat;
	shadowAttachment.samples = vk::SampleCountFlagBits::e1;
	shadowAttachment.loadOp = vk::AttachmentLoadOp::eClear;
	shadowAttachment.storeOp = vk::AttachmentStoreOp::eStore;
	shadowAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
	shadowAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
	shadowAttachment.initialLayout = vk::ImageLayout::eUndefined;
	shadowAttachment.finalLayout = vk::ImageLayout::eDepthStencilReadOnlyOptimal;


	vk::AttachmentReference depthReference = {};
	depthReference.attachment = 0;
	depthReference.layout = vk::ImageLayout::eDepthStencilAttachmentOptimal;


	vk::SubpassDescription subpass = {};
	subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
	subpass.colorAttachmentCount = 0;
	subpass.pColorAttachments = VK_NULL_HANDLE;
	subpass.pDepthStencilAttachment = &depthReference;

	


	std::array<vk::SubpassDependency, 2> dependencies;

	dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[0].dstSubpass = 0;
	dependencies[0].srcStageMask =  vk::PipelineStageFlagBits::eBottomOfPipe;
	dependencies[0].dstStageMask =  vk::PipelineStageFlagBits::eColorAttachmentOutput;
	dependencies[0].srcAccessMask =  vk::AccessFlagBits::eMemoryRead;
	dependencies[0].dstAccessMask =  vk::AccessFlagBits::eColorAttachmentWrite;
	dependencies[0].dependencyFlags = vk::DependencyFlagBits::eByRegion;

	dependencies[1].srcSubpass = 0;
	dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[1].srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
	dependencies[1].dstStageMask = vk::PipelineStageFlagBits::eBottomOfPipe;
	dependencies[1].srcAccessMask = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite;
	dependencies[1].dstAccessMask = vk::AccessFlagBits::eMemoryRead;
	dependencies[1].dependencyFlags = vk::DependencyFlagBits::eByRegion;

	vk::RenderPassCreateInfo renderPassCreateInfo = {};
	renderPassCreateInfo.sType = vk::StructureType::eRenderPassCreateInfo;
	renderPassCreateInfo.attachmentCount = 1;
	renderPassCreateInfo.pAttachments = &shadowAttachment;
	renderPassCreateInfo.subpassCount = 1;
	renderPassCreateInfo.pSubpasses = &subpass;
	renderPassCreateInfo.dependencyCount = 2;
	renderPassCreateInfo.pDependencies = dependencies.data();

	shadowRenderPass = logicalDevice.createRenderPass(renderPassCreateInfo);


	return shadowRenderPass;
}

vk::RenderPass vkInit::create_particle_renderpass(vk::Device logicalDevice, vk::Format depthFormat, vkUtil::FrameBufferAttachment particleAttachment)
{
	vk::RenderPass particleRenderPass = {};


	std::array<vk::AttachmentDescription,2> attachment = {};


	attachment[1].format = depthFormat;
	attachment[1].samples = vk::SampleCountFlagBits::e1;
	attachment[1].loadOp = vk::AttachmentLoadOp::eClear;
	attachment[1].storeOp = vk::AttachmentStoreOp::eStore;
	attachment[1].stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
	attachment[1].stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
	attachment[1].initialLayout = vk::ImageLayout::eUndefined;
	attachment[1].finalLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

	attachment[0].flags = vk::AttachmentDescriptionFlags();
	attachment[0].samples = vk::SampleCountFlagBits::e1;
	attachment[0].loadOp = vk::AttachmentLoadOp::eClear;
	attachment[0].storeOp = vk::AttachmentStoreOp::eDontCare;
	attachment[0].stencilLoadOp = vk::AttachmentLoadOp::eClear;
	attachment[0].stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
	attachment[0].initialLayout = vk::ImageLayout::eUndefined;
	attachment[0].finalLayout = vk::ImageLayout::eColorAttachmentOptimal;

	attachment[0].format = particleAttachment.format;


	vk::AttachmentReference colorAttachmentRef = {};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal;
	vk::AttachmentReference depthReference = {};
	depthReference.attachment = 1;
	depthReference.layout = vk::ImageLayout::eDepthStencilAttachmentOptimal;


	vk::SubpassDescription subpass = {};
	subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
	subpass.pDepthStencilAttachment = &depthReference;




	std::array<vk::SubpassDependency, 2> dependencies;

	dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[0].dstSubpass = 0;
	dependencies[0].srcStageMask = vk::PipelineStageFlagBits::eBottomOfPipe;
	dependencies[0].dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
	dependencies[0].srcAccessMask = vk::AccessFlagBits::eMemoryRead;
	dependencies[0].dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
	dependencies[0].dependencyFlags = vk::DependencyFlagBits::eByRegion;

	dependencies[1].srcSubpass = 0;
	dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[1].srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
	dependencies[1].dstStageMask = vk::PipelineStageFlagBits::eFragmentShader;
	dependencies[1].srcAccessMask = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite;
	dependencies[1].dstAccessMask = vk::AccessFlagBits::eShaderRead;
	dependencies[1].dependencyFlags = vk::DependencyFlagBits::eByRegion;




	vk::RenderPassCreateInfo renderPassCreateInfo = {};
	renderPassCreateInfo.sType = vk::StructureType::eRenderPassCreateInfo;
	renderPassCreateInfo.attachmentCount = attachment.size();
	renderPassCreateInfo.pAttachments = attachment.data();
	renderPassCreateInfo.subpassCount = 1;
	renderPassCreateInfo.pSubpasses = &subpass;
	renderPassCreateInfo.dependencyCount = 2;
	renderPassCreateInfo.pDependencies = dependencies.data();

	particleRenderPass = logicalDevice.createRenderPass(renderPassCreateInfo);


	return particleRenderPass;
}

vk::RenderPass vkInit::create_combinedImage_renderpass(vk::Device logicalDevice)
{
	std::array<vk::AttachmentDescription,1> attachments;
	vk::AttachmentReference attachmentRefertences[1];


	attachments[0].flags = vk::AttachmentDescriptionFlags();
	attachments[0].format = vk::Format::eR16G16B16A16Sfloat;
	attachments[0].samples = vk::SampleCountFlagBits::e1;
	attachments[0].loadOp = vk::AttachmentLoadOp::eLoad;
	attachments[0].storeOp = vk::AttachmentStoreOp::eStore;
	attachments[0].stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
	attachments[0].stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
	attachments[0].initialLayout = vk::ImageLayout::eColorAttachmentOptimal;
	attachments[0].finalLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
	//Define a general attachment, with its load/store operations

	/*
	attachments[1].flags = vk::AttachmentDescriptionFlags();
	attachments[1].format = depthFormat;
	attachments[1].samples = vk::SampleCountFlagBits::e1;
	attachments[1].loadOp = vk::AttachmentLoadOp::eClear;
	attachments[1].storeOp = vk::AttachmentStoreOp::eStore;
	attachments[1].stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
	attachments[1].stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
	attachments[1].initialLayout = vk::ImageLayout::eUndefined;
	attachments[1].finalLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;


	*/

	vk::AttachmentReference colorAttachmentRef = {};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal;
	//Declare that attachment to be color buffer 0 of the framebuffer
	attachmentRefertences[0] = colorAttachmentRef;
	/*
	vk::AttachmentReference depthAttachmentRef = {};
	depthAttachmentRef.attachment = 1;
	depthAttachmentRef.layout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
	*/

	// Pierwszy subpass: Renderowanie geometrii i wype³nienie za³¹czników position, normal i albedo
	std::vector<vk::SubpassDescription> subpassDescriptions;
	vk::SubpassDescription subpassGeometry = {};
	subpassGeometry.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
	subpassGeometry.colorAttachmentCount = 1; // Liczba za³¹czników kolorowych
	subpassGeometry.pColorAttachments = attachmentRefertences; // Tablica referencji do za³¹czników kolorowych
	subpassGeometry.pDepthStencilAttachment = nullptr; // Za³¹cznik g³êbokoœci i/lub szablonu
	subpassGeometry.inputAttachmentCount = 0;
	subpassGeometry.pInputAttachments = nullptr;
	// Drugi subpass: Obliczenia opóŸnione na podstawie za³¹czników position, normal i albedo

	vk::AttachmentReference colorReference = { };
	colorReference.attachment = 0;
	colorReference.layout = vk::ImageLayout::eColorAttachmentOptimal;
	subpassDescriptions.push_back(subpassGeometry);

	// Use subpass dependencies for attachment layout transitions
	std::array<vk::SubpassDependency, 2> dependencies;
	/*
	// Zale¿noœæ dla pierwszego subpassu (Renderowanie geometrii)
	dependencies[0].srcSubpass = vk::SubpassExternal;;
	dependencies[0].dstSubpass = 0;
	dependencies[0].srcStageMask = vk::PipelineStageFlagBits::eEarlyFragmentTests | vk::PipelineStageFlagBits::eLateFragmentTests;
	dependencies[0].dstStageMask = vk::PipelineStageFlagBits::eEarlyFragmentTests | vk::PipelineStageFlagBits::eLateFragmentTests;
	dependencies[0].srcAccessMask = {};
	dependencies[0].dstAccessMask =  vk::AccessFlagBits::eDepthStencilAttachmentWrite;
	dependencies[0].dependencyFlags = {};
	*/
	// Zale¿noœæ dla drugiego subpassu (Obliczenia opóŸnione)
	dependencies[0].srcSubpass = vk::SubpassExternal;
	dependencies[0].dstSubpass = 0;
	dependencies[0].srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
	dependencies[0].dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
	dependencies[0].srcAccessMask = {};
	dependencies[0].dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
	dependencies[0].dependencyFlags = {};
	dependencies[1].srcSubpass = 0;
	dependencies[1].dstSubpass = vk::SubpassExternal;
	dependencies[1].srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
	dependencies[1].dstStageMask = vk::PipelineStageFlagBits::eBottomOfPipe;
	dependencies[1].srcAccessMask = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite;
	dependencies[1].dstAccessMask = vk::AccessFlagBits::eMemoryRead;
	dependencies[1].dependencyFlags = vk::DependencyFlagBits::eByRegion;
	vk::RenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType = vk::StructureType::eRenderPassCreateInfo;
	renderPassInfo.pAttachments = attachments.data();
	renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	renderPassInfo.subpassCount = subpassDescriptions.size();
	renderPassInfo.pSubpasses = subpassDescriptions.data();
	renderPassInfo.dependencyCount = 2;
	renderPassInfo.pDependencies = dependencies.data();
	vk::RenderPass renderpass = {};
	
	try {
		
		renderpass = logicalDevice.createRenderPass(renderPassInfo);
	
		
	}
	catch (vk::SystemError err) {
		
			std::cout << "Failed create renderpass " << std::endl;
		
	}
	return renderpass;
}

vk::RenderPass vkInit::create_final_renderpass(vk::Device logicalDevice, vk::Format swapchainImageFormat)
{
		std::array<vk::AttachmentDescription, 1> attachments;
		vk::AttachmentReference attachmentRefertences[1];


		attachments[0].flags = vk::AttachmentDescriptionFlags();
		attachments[0].format = swapchainImageFormat;
		attachments[0].samples = vk::SampleCountFlagBits::e1;
		attachments[0].loadOp = vk::AttachmentLoadOp::eClear;
		attachments[0].storeOp = vk::AttachmentStoreOp::eStore;
		attachments[0].stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
		attachments[0].stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
		attachments[0].initialLayout = vk::ImageLayout::eUndefined;
		attachments[0].finalLayout = vk::ImageLayout::ePresentSrcKHR;
		//Define a general attachment, with its load/store operations

		/*
		attachments[1].flags = vk::AttachmentDescriptionFlags();
		attachments[1].format = depthFormat;
		attachments[1].samples = vk::SampleCountFlagBits::e1;
		attachments[1].loadOp = vk::AttachmentLoadOp::eClear;
		attachments[1].storeOp = vk::AttachmentStoreOp::eStore;
		attachments[1].stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
		attachments[1].stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
		attachments[1].initialLayout = vk::ImageLayout::eUndefined;
		attachments[1].finalLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;


		*/

		vk::AttachmentReference colorAttachmentRef = {};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal;
		//Declare that attachment to be color buffer 0 of the framebuffer
		attachmentRefertences[0] = colorAttachmentRef;
		/*
		vk::AttachmentReference depthAttachmentRef = {};
		depthAttachmentRef.attachment = 1;
		depthAttachmentRef.layout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
		*/

		// Pierwszy subpass: Renderowanie geometrii i wype³nienie za³¹czników position, normal i albedo
	

		std::vector<vk::SubpassDescription> subpassDescriptions;
		vk::SubpassDescription subpassGeometry = {};
		subpassGeometry.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
		subpassGeometry.colorAttachmentCount = 1; // Liczba za³¹czników kolorowych
		subpassGeometry.pColorAttachments = attachmentRefertences; // Tablica referencji do za³¹czników kolorowych
		subpassGeometry.pDepthStencilAttachment = nullptr; // Za³¹cznik g³êbokoœci i/lub szablonu
		subpassGeometry.inputAttachmentCount = 0;
		subpassGeometry.pInputAttachments = 0;
		// Drugi subpass: Obliczenia opóŸnione na podstawie za³¹czników position, normal i albedo

		
		subpassDescriptions.push_back(subpassGeometry);

		// Use subpass dependencies for attachment layout transitions
		std::array<vk::SubpassDependency, 2> dependencies;
		/*
		// Zale¿noœæ dla pierwszego subpassu (Renderowanie geometrii)
		dependencies[0].srcSubpass = vk::SubpassExternal;;
		dependencies[0].dstSubpass = 0;
		dependencies[0].srcStageMask = vk::PipelineStageFlagBits::eEarlyFragmentTests | vk::PipelineStageFlagBits::eLateFragmentTests;
		dependencies[0].dstStageMask = vk::PipelineStageFlagBits::eEarlyFragmentTests | vk::PipelineStageFlagBits::eLateFragmentTests;
		dependencies[0].srcAccessMask = {};
		dependencies[0].dstAccessMask =  vk::AccessFlagBits::eDepthStencilAttachmentWrite;
		dependencies[0].dependencyFlags = {};
		*/
		// Zale¿noœæ dla drugiego subpassu (Obliczenia opóŸnione)
		dependencies[0].srcSubpass = vk::SubpassExternal;
		dependencies[0].dstSubpass = 0;
		dependencies[0].srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
		dependencies[0].dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
		dependencies[0].srcAccessMask = {};
		dependencies[0].dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
		dependencies[0].dependencyFlags = {};
		dependencies[1].srcSubpass = 0;
		dependencies[1].dstSubpass = vk::SubpassExternal;
		dependencies[1].srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
		dependencies[1].dstStageMask = vk::PipelineStageFlagBits::eBottomOfPipe;
		dependencies[1].srcAccessMask = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite;
		dependencies[1].dstAccessMask = vk::AccessFlagBits::eMemoryRead;
		dependencies[1].dependencyFlags = vk::DependencyFlagBits::eByRegion;
		vk::RenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType = vk::StructureType::eRenderPassCreateInfo;
		renderPassInfo.pAttachments = attachments.data();
		renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		renderPassInfo.subpassCount = subpassDescriptions.size();
		renderPassInfo.pSubpasses = subpassDescriptions.data();
		renderPassInfo.dependencyCount = 2;
		renderPassInfo.pDependencies = dependencies.data();
		vk::RenderPass renderpass = {};

		try {

			renderpass = logicalDevice.createRenderPass(renderPassInfo);


		}
		catch (vk::SystemError err) {

			std::cout << "Failed create renderpass " << std::endl;

		}
		return renderpass;
	
}

vk::RenderPass vkInit::create_skybox_renderpass(vk::Device logicalDevice, vk::Format swapchainImageFormat, vk::Format depthFormat, vkUtil::FrameBufferAttachment skyBox)
{
	
	std::array<vk::AttachmentDescription, 2> attachment = {};


	

	attachment[0].flags = vk::AttachmentDescriptionFlags();
	attachment[0].samples = vk::SampleCountFlagBits::e1;
	attachment[0].loadOp = vk::AttachmentLoadOp::eClear;
	attachment[0].storeOp = vk::AttachmentStoreOp::eDontCare;
	attachment[0].stencilLoadOp = vk::AttachmentLoadOp::eClear;
	attachment[0].stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
	attachment[0].initialLayout = vk::ImageLayout::eUndefined;
	attachment[0].finalLayout = vk::ImageLayout::eColorAttachmentOptimal;
	attachment[0].format = skyBox.format;

	attachment[1].format = depthFormat;
	attachment[1].samples = vk::SampleCountFlagBits::e1;
	attachment[1].loadOp = vk::AttachmentLoadOp::eClear;
	attachment[1].storeOp = vk::AttachmentStoreOp::eStore;
	attachment[1].stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
	attachment[1].stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
	attachment[1].initialLayout = vk::ImageLayout::eUndefined;
	attachment[1].finalLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

	vk::AttachmentReference colorAttachmentRef = {};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal;
	vk::AttachmentReference depthReference = {};
	depthReference.attachment = 1;
	depthReference.layout = vk::ImageLayout::eDepthStencilAttachmentOptimal;


	vk::SubpassDescription subpass = {};
	subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
	subpass.pDepthStencilAttachment = &depthReference;




	std::array<vk::SubpassDependency, 2> dependencies;

	dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[0].dstSubpass = 0;
	dependencies[0].srcStageMask = vk::PipelineStageFlagBits::eBottomOfPipe;
	dependencies[0].dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
	dependencies[0].srcAccessMask = vk::AccessFlagBits::eMemoryRead;
	dependencies[0].dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
	dependencies[0].dependencyFlags = vk::DependencyFlagBits::eByRegion;

	dependencies[1].srcSubpass = 0;
	dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[1].srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
	dependencies[1].dstStageMask = vk::PipelineStageFlagBits::eFragmentShader;
	dependencies[1].srcAccessMask = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite;
	dependencies[1].dstAccessMask = vk::AccessFlagBits::eShaderRead;
	dependencies[1].dependencyFlags = vk::DependencyFlagBits::eByRegion;




	vk::RenderPassCreateInfo renderPassCreateInfo = {};
	renderPassCreateInfo.sType = vk::StructureType::eRenderPassCreateInfo;
	renderPassCreateInfo.attachmentCount = attachment.size();
	renderPassCreateInfo.pAttachments = attachment.data();
	renderPassCreateInfo.subpassCount = 1;
	renderPassCreateInfo.pSubpasses = &subpass;
	renderPassCreateInfo.dependencyCount = 2;
	renderPassCreateInfo.pDependencies = dependencies.data();

	vk::RenderPass skyBoxRenderpass = logicalDevice.createRenderPass(renderPassCreateInfo);


	return skyBoxRenderpass;
}

vk::RenderPass vkInit::create_downscale_renderpass(vk::Device logicalDevice)
{
	
	


	std::array<vk::AttachmentDescription, 1> attachments;
	vk::AttachmentReference attachmentRefertences[1];


	attachments[0].flags = vk::AttachmentDescriptionFlags();
	attachments[0].format = vk::Format::eB10G11R11UfloatPack32;
	attachments[0].samples = vk::SampleCountFlagBits::e1;
	attachments[0].loadOp = vk::AttachmentLoadOp::eDontCare;
	attachments[0].storeOp = vk::AttachmentStoreOp::eStore;
	attachments[0].stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
	attachments[0].stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
	attachments[0].initialLayout = vk::ImageLayout::eUndefined;
	attachments[0].finalLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
	//Define a general attachment, with its load/store operations

	/*
	attachments[1].flags = vk::AttachmentDescriptionFlags();
	attachments[1].format = depthFormat;
	attachments[1].samples = vk::SampleCountFlagBits::e1;
	attachments[1].loadOp = vk::AttachmentLoadOp::eClear;
	attachments[1].storeOp = vk::AttachmentStoreOp::eStore;
	attachments[1].stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
	attachments[1].stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
	attachments[1].initialLayout = vk::ImageLayout::eUndefined;
	attachments[1].finalLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;


	*/

	vk::AttachmentReference colorAttachmentRef = {};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal;
	//Declare that attachment to be color buffer 0 of the framebuffer
	attachmentRefertences[0] = colorAttachmentRef;
	/*
	vk::AttachmentReference depthAttachmentRef = {};
	depthAttachmentRef.attachment = 1;
	depthAttachmentRef.layout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
	*/

	// Pierwszy subpass: Renderowanie geometrii i wype³nienie za³¹czników position, normal i albedo
	std::vector<vk::SubpassDescription> subpassDescriptions;
	vk::SubpassDescription subpassGeometry = {};
	subpassGeometry.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
	subpassGeometry.colorAttachmentCount = 1; // Liczba za³¹czników kolorowych
	subpassGeometry.pColorAttachments = attachmentRefertences; // Tablica referencji do za³¹czników kolorowych
	subpassGeometry.pDepthStencilAttachment = nullptr; // Za³¹cznik g³êbokoœci i/lub szablonu
	subpassGeometry.inputAttachmentCount = 0;
	subpassGeometry.pInputAttachments = nullptr;
	// Drugi subpass: Obliczenia opóŸnione na podstawie za³¹czników position, normal i albedo

	vk::AttachmentReference colorReference = { };
	colorReference.attachment = 0;
	colorReference.layout = vk::ImageLayout::eColorAttachmentOptimal;
	subpassDescriptions.push_back(subpassGeometry);

	// Use subpass dependencies for attachment layout transitions
	std::array<vk::SubpassDependency, 2> dependencies;
	/*
	// Zale¿noœæ dla pierwszego subpassu (Renderowanie geometrii)
	dependencies[0].srcSubpass = vk::SubpassExternal;;
	dependencies[0].dstSubpass = 0;
	dependencies[0].srcStageMask = vk::PipelineStageFlagBits::eEarlyFragmentTests | vk::PipelineStageFlagBits::eLateFragmentTests;
	dependencies[0].dstStageMask = vk::PipelineStageFlagBits::eEarlyFragmentTests | vk::PipelineStageFlagBits::eLateFragmentTests;
	dependencies[0].srcAccessMask = {};
	dependencies[0].dstAccessMask =  vk::AccessFlagBits::eDepthStencilAttachmentWrite;
	dependencies[0].dependencyFlags = {};
	*/
	// Zale¿noœæ dla drugiego subpassu (Obliczenia opóŸnione)
	dependencies[0].srcSubpass = vk::SubpassExternal;
	dependencies[0].dstSubpass = 0;
	dependencies[0].srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
	dependencies[0].dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
	dependencies[0].srcAccessMask = {};
	dependencies[0].dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
	dependencies[0].dependencyFlags = {};
	dependencies[1].srcSubpass = 0;
	dependencies[1].dstSubpass = vk::SubpassExternal;
	dependencies[1].srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
	dependencies[1].dstStageMask = vk::PipelineStageFlagBits::eBottomOfPipe;
	dependencies[1].srcAccessMask = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite;
	dependencies[1].dstAccessMask = vk::AccessFlagBits::eMemoryRead;
	dependencies[1].dependencyFlags = vk::DependencyFlagBits::eByRegion;
	vk::RenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType = vk::StructureType::eRenderPassCreateInfo;
	renderPassInfo.pAttachments = attachments.data();
	renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	renderPassInfo.subpassCount = subpassDescriptions.size();
	renderPassInfo.pSubpasses = subpassDescriptions.data();
	renderPassInfo.dependencyCount = 2;
	renderPassInfo.pDependencies = dependencies.data();
	vk::RenderPass renderpass = {};

	try {

		renderpass = logicalDevice.createRenderPass(renderPassInfo);


	}
	catch (vk::SystemError err) {

		std::cout << "Failed create renderpass " << std::endl;

	}
	return renderpass;
}

vk::RenderPass vkInit::create_upscale_renderpass(vk::Device logicalDevice)
{
	std::array<vk::AttachmentDescription, 1> attachments;
	vk::AttachmentReference attachmentRefertences[1];


	attachments[0].flags = vk::AttachmentDescriptionFlags();
	attachments[0].format = vk::Format::eB10G11R11UfloatPack32;
	attachments[0].samples = vk::SampleCountFlagBits::e1;
	attachments[0].loadOp = vk::AttachmentLoadOp::eDontCare;
	attachments[0].storeOp = vk::AttachmentStoreOp::eStore;
	attachments[0].stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
	attachments[0].stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
	attachments[0].initialLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
	attachments[0].finalLayout = vk::ImageLayout::eColorAttachmentOptimal;
	//Define a general attachment, with its load/store operations

	/*
	attachments[1].flags = vk::AttachmentDescriptionFlags();
	attachments[1].format = depthFormat;
	attachments[1].samples = vk::SampleCountFlagBits::e1;
	attachments[1].loadOp = vk::AttachmentLoadOp::eClear;
	attachments[1].storeOp = vk::AttachmentStoreOp::eStore;
	attachments[1].stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
	attachments[1].stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
	attachments[1].initialLayout = vk::ImageLayout::eUndefined;
	attachments[1].finalLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;


	*/

	vk::AttachmentReference colorAttachmentRef = {};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal;
	//Declare that attachment to be color buffer 0 of the framebuffer
	attachmentRefertences[0] = colorAttachmentRef;
	/*
	vk::AttachmentReference depthAttachmentRef = {};
	depthAttachmentRef.attachment = 1;
	depthAttachmentRef.layout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
	*/

	// Pierwszy subpass: Renderowanie geometrii i wype³nienie za³¹czników position, normal i albedo
	std::vector<vk::SubpassDescription> subpassDescriptions;
	vk::SubpassDescription subpassGeometry = {};
	subpassGeometry.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
	subpassGeometry.colorAttachmentCount = 1; // Liczba za³¹czników kolorowych
	subpassGeometry.pColorAttachments = attachmentRefertences; // Tablica referencji do za³¹czników kolorowych
	subpassGeometry.pDepthStencilAttachment = nullptr; // Za³¹cznik g³êbokoœci i/lub szablonu
	subpassGeometry.inputAttachmentCount = 0;
	subpassGeometry.pInputAttachments = nullptr;
	// Drugi subpass: Obliczenia opóŸnione na podstawie za³¹czników position, normal i albedo

	vk::AttachmentReference colorReference = { };
	colorReference.attachment = 0;
	colorReference.layout = vk::ImageLayout::eColorAttachmentOptimal;
	subpassDescriptions.push_back(subpassGeometry);

	// Use subpass dependencies for attachment layout transitions
	std::array<vk::SubpassDependency, 2> dependencies;
	/*
	// Zale¿noœæ dla pierwszego subpassu (Renderowanie geometrii)
	dependencies[0].srcSubpass = vk::SubpassExternal;;
	dependencies[0].dstSubpass = 0;
	dependencies[0].srcStageMask = vk::PipelineStageFlagBits::eEarlyFragmentTests | vk::PipelineStageFlagBits::eLateFragmentTests;
	dependencies[0].dstStageMask = vk::PipelineStageFlagBits::eEarlyFragmentTests | vk::PipelineStageFlagBits::eLateFragmentTests;
	dependencies[0].srcAccessMask = {};
	dependencies[0].dstAccessMask =  vk::AccessFlagBits::eDepthStencilAttachmentWrite;
	dependencies[0].dependencyFlags = {};
	*/
	// Zale¿noœæ dla drugiego subpassu (Obliczenia opóŸnione)
	dependencies[0].srcSubpass = vk::SubpassExternal;
	dependencies[0].dstSubpass = 0;
	dependencies[0].srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
	dependencies[0].dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
	dependencies[0].srcAccessMask = {};
	dependencies[0].dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
	dependencies[0].dependencyFlags = {};
	dependencies[1].srcSubpass = 0;
	dependencies[1].dstSubpass = vk::SubpassExternal;
	dependencies[1].srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
	dependencies[1].dstStageMask = vk::PipelineStageFlagBits::eBottomOfPipe;
	dependencies[1].srcAccessMask = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite;
	dependencies[1].dstAccessMask = vk::AccessFlagBits::eMemoryRead;
	dependencies[1].dependencyFlags = vk::DependencyFlagBits::eByRegion;
	vk::RenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType = vk::StructureType::eRenderPassCreateInfo;
	renderPassInfo.pAttachments = attachments.data();
	renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	renderPassInfo.subpassCount = subpassDescriptions.size();
	renderPassInfo.pSubpasses = subpassDescriptions.data();
	renderPassInfo.dependencyCount = 2;
	renderPassInfo.pDependencies = dependencies.data();
	vk::RenderPass renderpass = {};

	try {

		renderpass = logicalDevice.createRenderPass(renderPassInfo);


	}
	catch (vk::SystemError err) {

		std::cout << "Failed create renderpass " << std::endl;

	}
	return renderpass;
}
