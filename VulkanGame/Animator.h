#pragma once
#include "config.h"
#include <string>
#include "Animation.h"


namespace vkAnimation {

	struct {
		const std::vector<char*> idleAnimationFilename;
	};


	class Animator
	{
	public:
		Animator();
		Animator(Animation* animation);
		

		void UpdateAnimation(float dt);

		void PlayAnimation(Animation* pAnimation);


		void BlendAnimations(Animation* destAnimation);

		void CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform);

		std::vector<glm::mat4> GetFinalBoneMatrices();

	private:
		std::vector<glm::mat4> m_FinalBoneMatrices;
		Animation* m_CurrentAnimation;
		Animation* destAnimation;
		float m_CurrentTime;
		float m_DeltaTime;
	};


}



