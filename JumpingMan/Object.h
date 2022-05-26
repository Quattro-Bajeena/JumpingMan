#pragma once

#include "RenderModel.h"
#include "Collider.h"

class Object
{
	RenderModel* model;
	

public:
	Collider collider;

	std::string name;

	glm::vec3 position;
	glm::quat rotation;

	

	Object(RenderModel* model, Collider collider);
	void Render(glm::mat4 V, glm::mat4 P);

	void SetRotation(float pitch, float yaw, float roll);

};

