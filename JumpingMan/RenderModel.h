#pragma once
#include <glm/gtc/quaternion.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "shaderprogram.h"



class RenderModel
{

private:
	

	int vertexCount;
	//float* vertices;
	//float* normals;
	//float* vertexNormals;
	//float* texCoords;
	//float* colors;

	std::vector<float> positions;
	std::vector<float> normals;
	std::vector<float> vertexNormals;
	std::vector<float> texCoords;
	std::vector<float> colors;

	GLuint texture;

	ShaderProgram* shader;
	

public:
	glm::vec3 position;
	glm::quat rotation;

	void SetTexture(GLuint texture);
	void SetShader(ShaderProgram* shader);

	RenderModel();
	//RenderModel(float* vertices, float* texCoords, int vertexCount, GLuint texture, ShaderProgram* shader);
	void LoadFromFile(std::string path);
	void Render(glm::mat4 cameraMat, glm::mat4 perspectiveMat);


};

