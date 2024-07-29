#include "AnimatedSceneObjects.h"





void AnimatedSceneObjects::draw()
{
}

void AnimatedSceneObjects::update(float deltaTime)
{
	
}

void AnimatedSceneObjects::create_vertex_buffer()
{

}

void AnimatedSceneObjects::load_animations(AnimatedModel model)
{
	this->loadAnimations(AobjMaterial.animationFilenames, model);
	this->animator.PlayAnimation(animations["dance"]);
}



AnimatedSceneObjects::AnimatedSceneObjects()
{
	
	AobjMaterial.setFilenames("res/animations/Arissa.fbx","res/textures/diffuse/floor.jpg", "res/textures/normals/floor.jpg", "res/textures/arm/floor.jpg", "res/textures/depth/floor.jpg");
	AobjMaterial.meshType = animatedModelTypes::DANCE;
	std::vector<std::string> animationFilenames = { {"res/anim/dance"}};
	removePrefix(animationFilenames,"res/anim/");



}

AnimatedSceneObjects::~AnimatedSceneObjects()
{
}


