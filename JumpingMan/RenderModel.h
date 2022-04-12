#pragma once
#include <glm/gtc/quaternion.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shaderprogram.h"


class RenderModel
{

private:
	

	int vertexCount;
	float* vertices;
	float* normals;
	float* vertexNormals;
	float* texCoords;
	float* colors;

	GLuint texture;

	ShaderProgram* shader;
	

public:
	glm::vec3 position;
	glm::quat rotation;

	RenderModel();
	RenderModel(float* vertices, float* texCoords, int vertexCount, GLuint texture);
	void Render();


};

