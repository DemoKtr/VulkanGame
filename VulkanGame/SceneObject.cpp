#include "SceneObject.h"

//SceneObject



void SceneObject::update()
{
	
}

Transform& SceneObject::getTransform()
{
	return this->transform;
}

void Box::draw()
{
}

void Box::update()
{
}

Box::Box()
{
	objMaterial.setFilenames("box.obj", "box.mtl", "res/textures/floorStone.jpg", "res/textures/floorNormal.jpg", "res/textures/floorARM.jpg", "res/textures/floorDisp.jpg");
	objMaterial.meshType = meshTypes::KITTY;
}

Box::~Box()
{
	objMaterial.clear();
}

void Floor::draw()
{
}

void Floor::update()
{
}

Floor::Floor()
{
	objMaterial.setFilenames("res/models/plane.obj", "res/models/plane.mtl", "res/textures/floorStone.jpg", "res/textures/floorNormal.jpg", "res/textures/floorARM.jpg", "res/textures/floorDisp.jpg");
	objMaterial.meshType = meshTypes::DOG;
}

Floor::~Floor()
{
	objMaterial.clear();
}
