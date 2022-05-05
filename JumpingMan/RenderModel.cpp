#include "RenderModel.h"
#include "OBJ_Loader.h"




void RenderModel::SetTexture(GLuint texture)
{
	this->texture = texture;
}

void RenderModel::SetShader(ShaderProgram* shader)
{
	this->shader = shader;
}

RenderModel::RenderModel()
{
}

//RenderModel::RenderModel(float* vertices, float* texCoords, int vertexCount, GLuint texture, ShaderProgram* shader)
//	:vertices(vertices), texCoords(texCoords), vertexCount(vertexCount), texture(texture), shader(shader),
//	position(glm::vec3(0))
//{
//}

void RenderModel::Render(glm::mat4 cameraMat, glm::mat4 perspectiveMat)
{
	glm::mat4 translate = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 rotate = glm::mat4_cast(rotation);
	glm::mat4 transform = translate * rotate;

	shader->use();
	glUniformMatrix4fv(shader->u("P"), 1, false, glm::value_ptr(perspectiveMat));
	glUniformMatrix4fv(shader->u("V"), 1, false, glm::value_ptr(cameraMat));
	glUniformMatrix4fv(shader->u("M"), 1, false, glm::value_ptr(transform));

	glEnableVertexAttribArray(shader->a("vertex"));
	glVertexAttribPointer(shader->a("vertex"), 4, GL_FLOAT, false, 0, positions.data());

	glEnableVertexAttribArray(shader->a("texCoord"));
	glVertexAttribPointer(shader->a("texCoord"), 2, GL_FLOAT, false, 0, texCoords.data());

	//glEnableVertexAttribArray(shader->a("color"));
	//glVertexAttribPointer(shader->a("color"), 4, GL_FLOAT, false, 0, colors.data());

	glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(shader->u("tex"), 0);

	glDrawArrays(GL_TRIANGLES, 0, vertexCount);

	glDisableVertexAttribArray(shader->a("vertex"));
	glDisableVertexAttribArray(shader->a("texCoord"));
	//glDisableVertexAttribArray(shader->a("color"));
}

void RenderModel::LoadFromFile(std::string path)
{
	objl::Loader loader;
	bool loadout = loader.LoadFile(path);

	if (loadout)
	{
		objl::Mesh curMesh = loader.LoadedMeshes[0];

		vertexCount = curMesh.Vertices.size();
		for (int j = 0; j < curMesh.Vertices.size(); j++)
		{
			objl::Vertex vertex = curMesh.Vertices[j];
			positions.insert(positions.end(), { vertex.Position.X, vertex.Position.Y, vertex.Position.Z, 1 });
			normals.insert(normals.end(), { vertex.Normal.X, vertex.Normal.Y, vertex.Normal.Z, 0 });
			texCoords.insert(texCoords.end(), { vertex.TextureCoordinate.X, vertex.TextureCoordinate.Y });

		}

		/*for (int j = 0; j < curMesh.Indices.size(); j += 3)
		{
			std::cout << "T" << j / 3 << ": " << curMesh.Indices[j] << ", " << curMesh.Indices[j + 1] << ", " << curMesh.Indices[j + 2] << "\n";
		}*/	
	}
	else
	{
		std::cout << "Failed to Load File. May have failed to find it or it was not an .obj file.\n";
	}
}
