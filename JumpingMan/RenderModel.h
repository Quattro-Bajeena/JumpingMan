#pragma once


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