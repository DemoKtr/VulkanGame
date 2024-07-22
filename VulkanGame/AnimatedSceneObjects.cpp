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
	AobjMaterial.setFilenames("res/animations/hero.fbx");
	AobjMaterial.meshType = animatedModelTypes::DANCE;
}

AnimatedSceneObjects::~AnimatedSceneObjects()
{
}
