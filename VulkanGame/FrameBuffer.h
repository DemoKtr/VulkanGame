#pragma once
#include "config.h"
#include "Frame.h"


namespace vkInit {

	/**
		Data structures involved in making framebuffers for the
		swapchain.
	*/
	struct framebufferInput {
		vk::Device device;
		vk::RenderPass renderpass;
		vk::Extent2D swapchainExtent;
	};

	/**
		Make framebuffers for the swapchain

		\param inputChunk required input for creation
		\param frames the vector to be populated with the created framebuffers
		\param debug whether the system is running in debug mode.
	*/
	void make_framebuffers(framebufferInput inputChunk, std::vector<vkUtil::SwapChainFrame>& frames, bool debug) {


		for (int i = 0; i < frames.size(); ++i) {

			std::vector<vk::ImageView> attachments = {
				frames[i].imageView, frames[i].depthBufferView
				//, frames[i].gbuffer.position.view, frames[i].gbuffer.normal.view, frames[i].gbuffer.albedo.view
			};
			
			vk::FramebufferCreateInfo framebufferInfo;
			framebufferInfo.flags = vk::FramebufferCreateFlags();
			framebufferInfo.renderPass = inputChunk.renderpass;
			framebufferInfo.attachmentCount = attachments.size();
			framebufferInfo.pAttachments = attachments.data();
			framebufferInfo.width = inputChunk.swapchainExtent.width;
			framebufferInfo.height = inputChunk.swapchainExtent.height;
			framebufferInfo.layers = 1;

			try {
				frames[i].framebuffer = inputChunk.device.createFramebuffer(framebufferInfo);

				if (debug) {
					std::cout << "Created framebuffer for frame " << i << std::endl;
				}
			}
			catch (vk::SystemError err) {
				if (debug) {
					std::cout << "Failed to create framebuffer for frame " << i << std::endl;
				}
			}

		}
	}


	void make_particle_framebuffers(framebufferInput inputChunk, std::vector<vkUtil::SwapChainFrame>& frames, bool debug) {


		for (int i = 0; i < frames.size(); ++i) {

			std::vector<vk::ImageView> attachments = {
		frames[i].particleAttachment.view, frames[i].particledepthBufferView,
		//, frames[i].gbuffer.position.view, frames[i].gbuffer.normal.view, frames[i].gbuffer.albedo.view
			};

			vk::FramebufferCreateInfo framebufferInfo;
			framebufferInfo.flags = vk::FramebufferCreateFlags();
			framebufferInfo.renderPass = inputChunk.renderpass;
			framebufferInfo.attachmentCount = attachments.size();
			framebufferInfo.pAttachments = attachments.data();
			framebufferInfo.width = inputChunk.swapchainExtent.width;
			framebufferInfo.height = inputChunk.swapchainExtent.height;
			framebufferInfo.layers = 1;

			try {
				frames[i].particleFramebuffer = inputChunk.device.createFramebuffer(framebufferInfo);

				if (debug) {
					std::cout << "Created framebuffer for frame " << i << std::endl;
				}
			}
			catch (vk::SystemError err) {
				if (debug) {
					std::cout << "Failed to create framebuffer for frame " << i << std::endl;
				}
			}
		}
}



	void make_postprocess_framebuffers(framebufferInput inputChunk, std::vector<vkUtil::SwapChainFrame>& frames, bool debug) {


		for (int i = 0; i < frames.size(); ++i) {
			
			std::vector<vk::ImageView> attachments = {
				frames[i].gbuffer.normal.view,
			};

			vk::FramebufferCreateInfo framebufferInfo;
			framebufferInfo.flags = vk::FramebufferCreateFlags();
			framebufferInfo.renderPass = inputChunk.renderpass;
			framebufferInfo.attachmentCount = attachments.size();
			framebufferInfo.pAttachments = attachments.data();
			framebufferInfo.width = inputChunk.swapchainExtent.width;
			framebufferInfo.height = inputChunk.swapchainExtent.height;
			framebufferInfo.layers = 1;

			try {
				frames[i].postProcessFramebuffer = inputChunk.device.createFramebuffer(framebufferInfo);

				if (debug) {
					std::cout << "Created postprocess framebuffer for frame " << i << std::endl;
				}
			}
			catch (vk::SystemError err) {
				if (debug) {
					std::cout << "Failed to create postprocess framebuffer for frame " << i << std::endl;
				}
			}
		}
	}

	void make_final_framebuffers(framebufferInput inputChunk, std::vector<vkUtil::SwapChainFrame>& frames, bool debug) {


		for (int i = 0; i < frames.size(); ++i) {

			std::vector<vk::ImageView> attachments = {
				frames[i].imageView,
			};

			vk::FramebufferCreateInfo framebufferInfo;
			framebufferInfo.flags = vk::FramebufferCreateFlags();
			framebufferInfo.renderPass = inputChunk.renderpass;
			framebufferInfo.attachmentCount = attachments.size();
			framebufferInfo.pAttachments = attachments.data();
			framebufferInfo.width = inputChunk.swapchainExtent.width;
			framebufferInfo.height = inputChunk.swapchainExtent.height;
			framebufferInfo.layers = 1;

			try {
				frames[i].finalFramebuffer = inputChunk.device.createFramebuffer(framebufferInfo);

				if (debug) {
					std::cout << "Created final framebuffer for frame " << i << std::endl;
				}
			}
			catch (vk::SystemError err) {
				if (debug) {
					std::cout << "Failed to create final framebuffer for frame " << i << std::endl;
				}
			}
		}
	}

	void make_downscale_framebuffers(framebufferInput inputChunk, std::vector<vkUtil::SwapChainFrame>& frames,vkBloom::PBBloom *bloom,bool debug) {


		for (int i = 0; i < frames.size(); ++i) {
			
			
			

			for (uint32_t j = 0; j < bloom->downScalepipeline.size(); ++j)
			{
				std::vector<vk::ImageView> attachments = {
					bloom->mipImagesView[j],
				};


				vk::FramebufferCreateInfo framebufferInfo;
				framebufferInfo.flags = vk::FramebufferCreateFlags();
				framebufferInfo.renderPass = inputChunk.renderpass;
				framebufferInfo.attachmentCount = attachments.size();
				framebufferInfo.pAttachments = attachments.data();
				framebufferInfo.width = bloom->intMipSize[j].x;
				framebufferInfo.height = bloom->intMipSize[j].y;
				framebufferInfo.layers = 1;

				try {
					frames[i].downupscaleFramebuffer.push_back(inputChunk.device.createFramebuffer(framebufferInfo));

					if (debug) {
						std::cout << "Created downupscale framebuffer for frame " << i << std::endl;
					}
				}
				catch (vk::SystemError err) {
					if (debug) {
						std::cout << "Failed to create downupscale framebuffer for frame " << i << std::endl;
					}
				}

			}

			

			
		}
	}



	void make_upscale_framebuffers(framebufferInput inputChunk, std::vector<vkUtil::SwapChainFrame>& frames, vkBloom::PBBloom* bloom ,bool debug) {


		for (int i = 0; i < frames.size(); ++i) {




			for (uint32_t j = bloom->upScalepipeline.size()-2; j >0 ; --j)
			{
				std::vector<vk::ImageView> attachments = {
					bloom->mipImagesView[j],
				};


				vk::FramebufferCreateInfo framebufferInfo;
				framebufferInfo.flags = vk::FramebufferCreateFlags();
				framebufferInfo.renderPass = inputChunk.renderpass;
				framebufferInfo.attachmentCount = attachments.size();
				framebufferInfo.pAttachments = attachments.data();
				framebufferInfo.width = bloom->intMipSize[j].x;
				framebufferInfo.height = bloom->intMipSize[j].y;
				framebufferInfo.layers = 1;

				try {
					frames[i].downupscaleFramebuffer.push_back(inputChunk.device.createFramebuffer(framebufferInfo));

					if (debug) {
						std::cout << "Created upscale framebuffer for frame " << i << std::endl;
					}
				}
				catch (vk::SystemError err) {
					if (debug) {
						std::cout << "Failed to create downupscale framebuffer for frame " << i << std::endl;
					}
				}

			}
			
			std::vector<vk::ImageView> attachments = {
				frames[i].gbuffer.albedo.view,
			};


			vk::FramebufferCreateInfo framebufferInfo;
			framebufferInfo.flags = vk::FramebufferCreateFlags();
			framebufferInfo.renderPass = bloom->finalRenderpass;
			framebufferInfo.attachmentCount = attachments.size();
			framebufferInfo.pAttachments = attachments.data();
			framebufferInfo.width = inputChunk.swapchainExtent.width;
			framebufferInfo.height = inputChunk.swapchainExtent.height;
			framebufferInfo.layers = 1;

			try {
				frames[i].downupscaleFramebuffer.push_back(inputChunk.device.createFramebuffer(framebufferInfo));

				if (debug) {
					std::cout << "Created upscale framebuffer for frame " << i << std::endl;
				}
			}
			catch (vk::SystemError err) {
				if (debug) {
					std::cout << "Failed to create downupscale framebuffer for frame " << i << std::endl;
				}
			}

			
		}
	}

	void make_skybox_framebuffers(framebufferInput inputChunk, std::vector<vkUtil::SwapChainFrame>& frames, bool debug) {


		for (int i = 0; i < frames.size(); ++i) {
			
			std::vector<vk::ImageView> attachments = {
			frames[i].skyBoxAttachment.view, frames[i].skyboxdepthBufferView
			};

			vk::FramebufferCreateInfo framebufferInfo;
			framebufferInfo.flags = vk::FramebufferCreateFlags();
			framebufferInfo.renderPass = inputChunk.renderpass;
			framebufferInfo.attachmentCount = attachments.size();
			framebufferInfo.pAttachments = attachments.data();
			framebufferInfo.width = inputChunk.swapchainExtent.width;
			framebufferInfo.height = inputChunk.swapchainExtent.height;
			framebufferInfo.layers = 1;

			try {
				frames[i].skyBoxFramebuffer = inputChunk.device.createFramebuffer(framebufferInfo);

				if (debug) {
					std::cout << "Created postprocess framebuffer for frame " << i << std::endl;
				}
			}
			catch (vk::SystemError err) {
				if (debug) {
					std::cout << "Failed to create postprocess framebuffer for frame " << i << std::endl;
				}
			}
		}
	}


	void make_shadow_framebuffers(framebufferInput inputChunk, std::vector<vkUtil::SwapChainFrame>& frames, bool debug) {


		for (int i = 0; i < frames.size(); ++i) {

			std::vector<vk::ImageView> attachments = {
				frames[i].shadowMapBuffer.shadowBufferDepthAttachment.view
			};

			vk::FramebufferCreateInfo framebufferInfo;
			framebufferInfo.flags = vk::FramebufferCreateFlags();
			framebufferInfo.renderPass = inputChunk.renderpass;
			framebufferInfo.attachmentCount = attachments.size();
			framebufferInfo.pAttachments = attachments.data();
			framebufferInfo.width = 1024;
			framebufferInfo.height =1024;
			framebufferInfo.layers = 12;

			try {
				frames[i].shadowFramebuffer = inputChunk.device.createFramebuffer(framebufferInfo);

				if (debug) {
					std::cout << "Created framebuffer for frame " << i << std::endl;
				}
			}
			catch (vk::SystemError err) {
				if (debug) {
					std::cout << "Failed to create framebuffer for frame " << i << std::endl;
				}
			}

		}
	}

	void make_framebuffers_withGbuffer(framebufferInput inputChunk, std::vector<vkUtil::SwapChainFrame>& frames, bool debug) {

		for (int i = 0; i < frames.size(); ++i) {
			
			std::vector<vk::ImageView> attachments = {
			frames[i].postProcessInputAttachment.view, frames[i].gbuffer.position.view,frames[i].gbuffer.normal.view,frames[i].gbuffer.albedo.view,frames[i].gbuffer.arm.view, frames[i].gbuffer.T.view, frames[i].gbuffer.worldPos.view ,frames[i].depthBufferView,
			};

			vk::FramebufferCreateInfo framebufferInfo;
			framebufferInfo.flags = vk::FramebufferCreateFlags();
			framebufferInfo.renderPass = inputChunk.renderpass;
			framebufferInfo.attachmentCount = attachments.size();
			framebufferInfo.pAttachments = attachments.data();
			framebufferInfo.width = inputChunk.swapchainExtent.width;
			framebufferInfo.height = inputChunk.swapchainExtent.height;
			framebufferInfo.layers = 1;

			try {
				frames[i].framebuffer = inputChunk.device.createFramebuffer(framebufferInfo);

				if (debug) {
					std::cout << "Created framebuffer for frame " << i << std::endl;
				}
			}
			catch (vk::SystemError err) {
				if (debug) {
					std::cout << "Failed to create framebuffer for frame " << i << std::endl;
				}
			}

		}
	}
}




