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
	objMaterial.setFilenames("box.obj", "box.mtl", "tex/floordif.png", "tex/floornormal.png", "tex/floorarm.png", "tex/floordepth.png");
	objMaterial.meshType = meshTypes::KITTY;
}

Box::~Box()
{
	objMaterial.clear();
}
