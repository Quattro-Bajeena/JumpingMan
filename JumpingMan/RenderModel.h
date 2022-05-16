#pragma once
#include <glm/gtc/quaternion.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "shaderprogram.h"

namespace objl {
	class Mesh;
}

class RenderModel
{

private:
	
	int vertexCount;
	std::vector<float> positions;
	std::vector<float> normals;
	std::vector<float> vertexNormals;
	std::vector<float> texCoords;
	std::vector<float> colors;

	GLuint texture;
	ShaderProgram* shader;
	

public:
	std::string name;
	void SetTexture(GLuint texture);
	void SetShader(ShaderProgram* shader);

	RenderModel();
	RenderModel(objl::Mesh mesh);
	void Render(glm::mat4 V, glm::mat4 P, glm::mat4 M);
};