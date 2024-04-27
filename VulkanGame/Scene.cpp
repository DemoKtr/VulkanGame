#include "Scene.h"

Scene::Scene()
{

}

Scene::~Scene()
{
	for (SceneObject *sb : staticSceneObjects) {
		delete sb;
	}
	for (SceneObject* sb : dynamicSceneObjects) {
		delete sb;
	}
}
