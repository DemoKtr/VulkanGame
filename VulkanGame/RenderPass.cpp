#include "RenderPass.h"

vk::RenderPass vkInit::create_defered_renderpass(vk::Device logicalDevice,vkUtil::Gbuffer gBuffer, vk::Format swapchainImageFormat, vk::Format depthFormat)
{


	std::array<vk::AttachmentDescription,5> attachments;
	vk::AttachmentReference attachmentRefertences[4];


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

	
	

	// Init attachment properties
	for (uint32_t i = 1; i < 4; ++i)
	{
		attachments[i].flags = vk::AttachmentDescriptionFlags();
		attachments[i].samples =  vk::SampleCountFlagBits::e1;
		attachments[i].loadOp =  vk::AttachmentLoadOp::eClear;
		attachments[i].storeOp = vk::AttachmentStoreOp::eDontCare;
		attachments[i].stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
		attachments[i].stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
		attachments[i].initialLayout = vk::ImageLayout::eUndefined;
		attachments[i].finalLayout = vk::ImageLayout::eColorAttachmentOptimal;

		if(i==1) attachments[i].format = gBuffer.position.format;
		else if(i==2)attachments[i].format = gBuffer.normal.format;
		else attachments[i].format = gBuffer.albedo.format;

	}


	attachments[4].flags = vk::AttachmentDescriptionFlags();
	attachments[4].format = depthFormat;
	attachments[4].samples = vk::SampleCountFlagBits::e1;
	attachments[4].loadOp = vk::AttachmentLoadOp::eClear;
	attachments[4].storeOp = vk::AttachmentStoreOp::eStore;
	attachments[4].stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
	attachments[4].stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
	attachments[4].initialLayout = vk::ImageLayout::eUndefined;
	attachments[4].finalLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

	


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
	vk::AttachmentReference depthAttachmentRef = {};
	depthAttachmentRef.attachment = 4;
	depthAttachmentRef.layout = vk::ImageLayout::eDepthStencilAttachmentOptimal;



	// Pierwszy subpass: Renderowanie geometrii i wype³nienie za³¹czników position, normal i albedo
	std::vector<vk::SubpassDescription> subpassDescriptions;
	vk::SubpassDescription subpassGeometry = {};
	subpassGeometry.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
	subpassGeometry.colorAttachmentCount = 4; // Liczba za³¹czników kolorowych
	subpassGeometry.pColorAttachments = attachmentRefertences; // Tablica referencji do za³¹czników kolorowych
	subpassGeometry.pDepthStencilAttachment = &depthAttachmentRef; // Za³¹cznik g³êbokoœci i/lub szablonu
	subpassGeometry.inputAttachmentCount = 0;
	subpassGeometry.pInputAttachments = nullptr;
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
	dependencies[2].dstStageMask = vk::PipelineStageFlagBits::eBottomOfPipe;
	dependencies[2].srcAccessMask = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite;
	dependencies[2].dstAccessMask = vk::AccessFlagBits::eMemoryRead;
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

vk::RenderPass vkInit::create_shadows_renderpass(vk::Device logicalDevice, vkUtil::shadowMapBuffer* shadowmapBuffer, vk::Format attachmentFormat, vk::Format depthFormat)
{
	vk::RenderPass shadowRenderPass = {};
	vk::AttachmentDescription shadowAttachment = {};

	shadowAttachment.format = depthFormat;
	shadowAttachment.samples = vk::SampleCountFlagBits::e1;
	shadowAttachment.loadOp = vk::AttachmentLoadOp::eClear;
	shadowAttachment.storeOp = vk::AttachmentStoreOp::eDontCare;
	shadowAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
	shadowAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
	shadowAttachment.initialLayout = vk::ImageLayout::eUndefined;
	shadowAttachment.finalLayout = vk::ImageLayout::eDepthStencilReadOnlyOptimal;



	vk::SamplerCreateInfo samplerInfo = {};
	samplerInfo.magFilter = vk::Filter::eLinear;
	samplerInfo.minFilter = vk::Filter::eLinear;
	samplerInfo.mipmapMode = vk::SamplerMipmapMode::eLinear;
	samplerInfo.addressModeU = vk::SamplerAddressMode::eClampToEdge;
	samplerInfo.addressModeV = vk::SamplerAddressMode::eClampToEdge;
	samplerInfo.addressModeW = vk::SamplerAddressMode::eClampToEdge;
	samplerInfo.mipLodBias = 0.0f;
	samplerInfo.maxAnisotropy = 1.0f;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = 1.0f;
	samplerInfo.borderColor = vk::BorderColor::eFloatOpaqueWhite;

	shadowmapBuffer->sampler = logicalDevice.createSampler(samplerInfo);


	vk::AttachmentReference depthReference = {};
	depthReference.attachment = 5;
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
	dependencies[0].dstAccessMask =  vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite;
	dependencies[0].dependencyFlags = vk::DependencyFlagBits::eByRegion;

	dependencies[1].srcSubpass = 0;
	dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[1].srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
	dependencies[1].dstStageMask = vk::PipelineStageFlagBits::eBottomOfPipe;
	dependencies[1].srcAccessMask = vk::AccessFlagBits::eMemoryRead;
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
