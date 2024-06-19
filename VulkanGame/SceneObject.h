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
	char* armMap;
	char* depthMap;

	void setFilenames(const char* newModel, const char* newMaterial, const char* newDiffuse, const char* newNormalMap,const char* newArmMap, const char* newDepthMap) {
		// Zwolnienie poprzednio alokowanej pamiêci (jeœli istnieje)
		

		// Alokowanie nowej pamiêci i kopiowanie ³añcuchów znaków
		model = _strdup(newModel);
		material = _strdup(newMaterial);
		diffuse = _strdup(newDiffuse);
		normalMap = _strdup(newNormalMap);
		armMap = _strdup(newArmMap);
		depthMap = _strdup(newDepthMap);

	}

	void clear() {
		delete[] model;
		delete[] material;
		delete[] diffuse;
		delete[] normalMap;
		delete[] armMap;
		delete[] depthMap;
	}

};



class SceneObject
{
protected:
	Transform transform;
public:
	 virtual void draw() = 0;
	 virtual void update(float deltaTime) =0;
	 Transform& getTransform();
	 objectMaterial objMaterial;
};


class Light {
public:
	Transform transform;
	glm::vec4 diffuse;
	float angle = { 0};
	Light();
	void move();
	~Light();
};



class Box : public SceneObject {
	// Inherited via StaticSceneObject
public:
	void draw() override;
	void update(float deltaTime) override;

	Box();
	~Box();
};


class Floor : public SceneObject {
	// Inherited via StaticSceneObject
public:
	void draw() override;
	void update(float deltaTime) override;

	Floor();
	~Floor();
};