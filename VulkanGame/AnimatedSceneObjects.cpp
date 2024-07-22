#include "AnimatedSceneObjects.h"

struct animatedObjectMaterial {
	meshTypes meshType;
	char* model;
	char* material;
	char* diffuse;

	void setFilenames(const char* newModel, const char* newMaterial, const char* newDiffuse, const char* newNormalMap, const char* newArmMap, const char* newDepthMap) {
		// Zwolnienie poprzednio alokowanej pamiêci (jeœli istnieje)
		// Alokowanie nowej pamiêci i kopiowanie ³añcuchów znaków
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
