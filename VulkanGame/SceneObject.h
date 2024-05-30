#pragma once
#include "config.h"
#include <glm/fwd.hpp>
#include "Transform.h"

struct objectMaterial {
	meshTypes meshType;
	char* model;
	char* material;
	char* diffuse;
	char* normalMap;

	void setFilenames(const char* newModel, const char* newMaterial, const char* newDiffuse, const char* newNormalMap) {
		// Zwolnienie poprzednio alokowanej pamiêci (jeœli istnieje)
		

		// Alokowanie nowej pamiêci i kopiowanie ³añcuchów znaków
		model = _strdup(newModel);
		material = _strdup(newMaterial);
		diffuse = _strdup(newDiffuse);
		normalMap = _strdup(newNormalMap);

	}

	void clear() {
		delete[] model;
		delete[] material;
		delete[] diffuse;
		delete[] normalMap;
	}

};



class SceneObject
{
protected:
	Transform transform;
public:
	 virtual void draw() = 0;
	 virtual void update() =0;
	 Transform& getTransform();
	 objectMaterial objMaterial;
};


class Light {
public:
	Transform transform;
};



class Box : public SceneObject {
	// Inherited via StaticSceneObject
public:
	void draw() override;
	void update() override;

	Box();
	~Box();
};
