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

AnimatedSceneObjects::AnimatedSceneObjects()
{
	animator = new vkAnimation::Animator();
	AobjMaterial.setFilenames("res/animations/hero.fbx","res/textures/diffuse/floor.jpg", "res/textures/normals/floor.jpg", "res/textures/arm/floor.jpg", "res/textures/depth/floor.jpg");
	AobjMaterial.meshType = animatedModelTypes::DANCE;
}

AnimatedSceneObjects::~AnimatedSceneObjects()
{
}
