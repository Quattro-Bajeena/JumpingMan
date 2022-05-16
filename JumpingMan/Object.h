#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>

#include "RenderModel.h"

class Object
{
	RenderModel* model;

public:
	std::string name;

	glm::vec3 position;
	glm::quat rotation;

	

	Object(RenderModel* model);
	void Render(glm::mat4 V, glm::mat4 P);

	void SetRotation(float pitch, float yaw, float roll);

};

