#include "pch.h"
#include "RenderModel.h"
#include "myCube.h"
#include "OBJ_Loader.h"




RenderModel::RenderModel(objl::Mesh mesh)
{
	diffuseMap = normalMap = metallicMap = roughnessMap = heightMap = -1;
	name = mesh.MeshName;
	vertexCount = mesh.Vertices.size();
	for (int j = 0; j < mesh.Vertices.size(); j++)
	{
		objl::Vertex vertex = mesh.Vertices[j];
		positions.insert(positions.end(), { vertex.Position.X, vertex.Position.Y, vertex.Position.Z, 1 });
		normals.insert(normals.end(), { vertex.Normal.X, vertex.Normal.Y, vertex.Normal.Z });
		texCoords.insert(texCoords.end(), { vertex.TextureCoordinate.X, -1 * vertex.TextureCoordinate.Y });
	}

	for (int i = 0; i < mesh.Vertices.size(); i += 3) {

		// Shortcuts for vertices
		objl::Vertex v0 = mesh.Vertices[i];
		objl::Vertex v1 = mesh.Vertices[i + 1];
		objl::Vertex v2 = mesh.Vertices[i + 2];

		auto uv0 = v0.TextureCoordinate;
		auto uv1 = v1.TextureCoordinate;
		auto uv2 = v2.TextureCoordinate;

		// Edges of the triangle : position delta
		auto deltaPos1 = v1.Position - v0.Position;
		auto deltaPos2 = v2.Position - v0.Position;

		// UV delta
		auto deltaUV1 = uv1 - uv0;
		auto deltaUV2 = uv2 - uv0;

		float r = 1.0f / (deltaUV1.X * deltaUV2.Y - deltaUV1.Y * deltaUV2.X);
		auto tangent = (deltaPos1 * deltaUV2.Y - deltaPos2 * deltaUV1.Y) * r;
		auto bitangent = (deltaPos2 * deltaUV1.X - deltaPos1 * deltaUV2.X) * r;

		tangent.Normalize();
		bitangent.Normalize();
		

		for (int j = 0; j < 3; j++) {
			tangents.insert(tangents.end(), { tangent.X, tangent.Y, tangent.Z });
		}

		for (int j = 0; j < 3; j++) {
			bitangents.insert(bitangents.end(), { bitangent.X, bitangent.Y, bitangent.Z });
		}
		



	}

}



void RenderModel::Render(glm::mat4 V, glm::mat4 P, glm::mat4 M, glm::vec3 cameraPos, glm::vec3 lightPos1, glm::vec3 lightPos2) {


	auto sp = this->shader;

	sp->use();//Aktywacja programu cieniującego
	//Przeslij parametry programu cieniującego do karty graficznej
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
	glUniform3fv(sp->u("viewPos"), 1, glm::value_ptr(cameraPos));
	glUniform3fv(sp->u("lightPos1"), 1, glm::value_ptr(lightPos1));
	glUniform3fv(sp->u("lightPos2"), 1, glm::value_ptr(lightPos2));
	

	glEnableVertexAttribArray(sp->a("vertex"));  //Włącz przesyłanie danych do atrybutu vertex
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, positions.data()); //Wskaż tablicę z danymi dla atrybutu vertex

	/*glEnableVertexAttribArray(sp->a("color"));
	glVertexAttribPointer(sp->a("color"), 4, GL_FLOAT, false, 0, colors.data());*/

	glEnableVertexAttribArray(sp->a("texCoord"));
	glVertexAttribPointer(sp->a("texCoord"), 2, GL_FLOAT, false, 0, texCoords.data());

	glEnableVertexAttribArray(sp->a("normal"));
	glVertexAttribPointer(sp->a("normal"), 3, GL_FLOAT, false, 0, normals.data());

	glEnableVertexAttribArray(sp->a("tangent"));
	glVertexAttribPointer(sp->a("tangent"), 3, GL_FLOAT, false, 0, tangents.data());

	glEnableVertexAttribArray(sp->a("bitangent"));
	glVertexAttribPointer(sp->a("bitangent"), 3, GL_FLOAT, false, 0, bitangents.data());

	if (diffuseMap != -1) {
		glUniform1i(sp->u("diffuseMap"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
	}
	if (normalMap != -1) {
		glUniform1i(sp->u("normalMap"), 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, normalMap);
	}
	if (metallicMap != -1) {
		glUniform1i(sp->u("metallicMap"), 2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, metallicMap);
	}
	if (roughnessMap != -1) {
		glUniform1i(sp->u("roughnessMap"), 3);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, roughnessMap);
	}
	if (heightMap != -1) {
		glUniform1i(sp->u("heightMap"), 4);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, heightMap);
	}

	

	glDrawArrays(GL_TRIANGLES, 0, vertexCount); 

	glDisableVertexAttribArray(sp->a("vertex"));  
	glDisableVertexAttribArray(sp->a("normal"));
	glDisableVertexAttribArray(sp->a("texCoord"));
	glDisableVertexAttribArray(sp->a("tangent"));
	glDisableVertexAttribArray(sp->a("bitangent"));

}




RenderModel::RenderModel()
{
}


