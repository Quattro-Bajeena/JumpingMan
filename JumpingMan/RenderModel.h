#pragma once



class RenderModel
{

private:
	
	int vertexCount;
	std::vector<float> positions;
	std::vector<float> normals;
	std::vector<float> vertexNormals;
	std::vector<float> texCoords;
	std::vector<float> colors;

	std::vector<float> tangents;
	std::vector<float> bitangents;
	

public:

	GLuint diffuseMap;
	GLuint normalMap;
	GLuint metallicMap;
	GLuint roughnessMap;
	ShaderProgram* shader;

	std::string name;


	RenderModel();
	RenderModel(objl::Mesh mesh);
	void Render(glm::mat4 V, glm::mat4 P, glm::mat4 M, glm::vec3 cameraPos);
};