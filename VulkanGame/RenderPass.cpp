#include "RenderPass.h"

void vkInit::create_defered_renderpass(vk::Device logicalDevice,vkUtil::Gbuffer gBuffer, vk::Format swapchainImageFormat, vk::Format depthFormat)
{


	std::vector<vk::AttachmentDescription> attachments;
	std::vector<vk::AttachmentReference> attachmentRefertences;

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
	//Define a general attachment, with its load/store operations
	attachments.push_back(colorAttachment);
	vk::AttachmentReference colorAttachmentRef = {};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal;
	//Declare that attachment to be color buffer 0 of the framebuffer
	
	attachmentRefertences.push_back(colorAttachmentRef);
	

	// Init attachment properties
	for (uint32_t i = 0; i < 3; ++i)
	{
		vk::AttachmentDescription gAttachment = {};
		gAttachment.samples =  vk::SampleCountFlagBits::e1;
		gAttachment.loadOp =  vk::AttachmentLoadOp::eClear;
		gAttachment.storeOp = vk::AttachmentStoreOp::eStore;
		gAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
		gAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;

		if(i==0) gAttachment.format = gBuffer.position.format;
		else if(i==1)gAttachment.format = gBuffer.normal.format;
		else gAttachment.format = gBuffer.albedo.format;
		attachments.push_back(gAttachment);
	}

	


	vk::AttachmentReference att;
	att.attachment = 1;
	att.layout = vk::ImageLayout::eColorAttachmentOptimal;
	attachmentRefertences.push_back(att);
	att.attachment = 2;
	attachmentRefertences.push_back(att);
	att.attachment = 3;
	attachmentRefertences.push_back(att);


	vk::AttachmentDescription detpthAttachment = {};
	colorAttachment.flags = vk::AttachmentDescriptionFlags();
	colorAttachment.format = depthFormat;
	colorAttachment.samples = vk::SampleCountFlagBits::e1;
	colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
	colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
	colorAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
	colorAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
	colorAttachment.initialLayout = vk::ImageLayout::eUndefined;
	colorAttachment.finalLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

	//attachments.push_back(detpthAttachment);
	vk::AttachmentReference depthAttachmentRef = {};
	colorAttachmentRef.attachment = 4;
	colorAttachmentRef.layout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
	//Declare that attachment to be color buffer 0 of the framebuffer
	//attachmentRefertences.push_back(depthAttachmentRef);






	// Pierwszy subpass: Renderowanie geometrii i wype³nienie za³¹czników position, normal i albedo
	std::vector<vk::SubpassDescription> subpassDescriptions;
	vk::SubpassDescription subpassGeometry = {};
	subpassGeometry.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
	subpassGeometry.colorAttachmentCount = 4; // Liczba za³¹czników kolorowych
	subpassGeometry.pColorAttachments = attachmentRefertences.data(); // Tablica referencji do za³¹czników kolorowych
	subpassGeometry.pDepthStencilAttachment = &depthAttachmentRef; // Za³¹cznik g³êbokoœci i/lub szablonu

	// Drugi subpass: Obliczenia opóŸnione na podstawie za³¹czników position, normal i albedo

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

	vk::SubpassDescription subpassDeferred = {};
	subpassDeferred.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
	subpassDeferred.inputAttachmentCount = 3; // Liczba za³¹czników wejœciowych
	subpassDeferred.pInputAttachments = inputReferences; // Tablica referencji do za³¹czników wejœciowych
	subpassDeferred.colorAttachmentCount = 1; // Liczba za³¹czników kolorowych wyjœciowych
	subpassDeferred.pColorAttachments = &colorReference; // Tablica referencji do za³¹czników kolorowych wyjœciowych
	subpassDescriptions.push_back(subpassGeometry);
	subpassDescriptions.push_back(subpassDeferred);

	// Use subpass dependencies for attachment layout transitions
	std::array<vk::SubpassDependency, 2> dependencies;

	// Zale¿noœæ dla pierwszego subpassu (Renderowanie geometrii)
	dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[0].dstSubpass = 0;
	dependencies[0].srcStageMask = vk::PipelineStageFlagBits::eBottomOfPipe;
	dependencies[0].dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
	dependencies[0].srcAccessMask = vk::AccessFlagBits::eMemoryRead;
	dependencies[0].dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
	dependencies[0].dependencyFlags = vk::DependencyFlagBits::eByRegion;

	// Zale¿noœæ dla drugiego subpassu (Obliczenia opóŸnione)
	dependencies[1].srcSubpass = 0;
	dependencies[1].dstSubpass = 1;
	dependencies[1].srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
	dependencies[1].dstStageMask = vk::PipelineStageFlagBits::eFragmentShader;
	dependencies[1].srcAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
	dependencies[1].dstAccessMask = vk::AccessFlagBits::eInputAttachmentRead;
	dependencies[1].dependencyFlags = vk::DependencyFlagBits::eByRegion;

	vk::RenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType =  vk::StructureType::eRenderPassCreateInfo;
	renderPassInfo.pAttachments = attachments.data();
	renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	renderPassInfo.subpassCount = subpassDescriptions.size();
	renderPassInfo.pSubpasses = subpassDescriptions.data();
	renderPassInfo.dependencyCount = 2;
	renderPassInfo.pDependencies = dependencies.data();

	vk::RenderPass renderpass= logicalDevice.createRenderPass(renderPassInfo);



	// Create sampler to sample from the color attachments
	vk::SamplerCreateInfo sampler = {};
	sampler.magFilter =  vk::Filter::eNearest;
	sampler.minFilter =  vk::Filter::eNearest;
	sampler.mipmapMode =  vk::SamplerMipmapMode::eLinear;
	sampler.addressModeU =  vk::SamplerAddressMode::eClampToEdge;
	sampler.addressModeV = sampler.addressModeU;
	sampler.addressModeW = sampler.addressModeU;
	sampler.mipLodBias = 0.0f;
	sampler.maxAnisotropy = 1.0f;
	sampler.minLod = 0.0f;
	sampler.maxLod = 1.0f;
	sampler.borderColor =  vk::BorderColor::eFloatOpaqueWhite;
	vk::Sampler colorSampler;
	logicalDevice.createSampler(sampler);

}
