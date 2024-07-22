#include "AnimatedSceneObjects.h"

struct animatedObjectMaterial {
	meshTypes meshType;
	char* model;
	char* material;
	char* diffuse;

	void setFilenames(const char* newModel, const char* newMaterial, const char* newDiffuse, const char* newNormalMap, const char* newArmMap, const char* newDepthMap) {
		// Zwolnienie poprzednio alokowanej pami�ci (je�li istnieje)
		// Alokowanie nowej pami�ci i kopiowanie �a�cuch�w znak�w
		model = _strdup(newModel);
		material = _strdup(newMaterial);
		diffuse = _strdup(newDiffuse);

	}

	void clear() {
		delete[] model;
		delete[] material;
		delete[] diffuse;

	}

};



void AnimatedSceneObjects::draw()
{
}

void AnimatedSceneObjects::update(float deltaTime)
{
}

void AnimatedSceneObjects::create_vertex_buffer()
{

}

AnimatedSceneObjects::AnimatedSceneObjects(char* const& path)
{
	animatedModel = new  AnimatedModel(path);
}
