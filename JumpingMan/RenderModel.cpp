#include "RenderModel.h"
#include "myCube.h"
#include "OBJ_Loader.h"




void RenderModel::SetTexture(GLuint texture)
{
	this->texture = texture;
}

void RenderModel::SetShader(ShaderProgram* shader)
{
	this->shader = shader;
}

RenderModel::RenderModel(objl::Mesh mesh)
{
	name = mesh.MeshName;
	vertexCount = mesh.Vertices.size();
	for (int j = 0; j < mesh.Vertices.size(); j++)
	{
		objl::Vertex vertex = mesh.Vertices[j];
		positions.insert(positions.end(), { vertex.Position.X, vertex.Position.Y, vertex.Position.Z, 1 });
		normals.insert(normals.end(), { vertex.Normal.X, vertex.Normal.Y, vertex.Normal.Z, 0 });
		texCoords.insert(texCoords.end(), { vertex.TextureCoordinate.X, -1 * vertex.TextureCoordinate.Y });
	}

}



void RenderModel::Render(glm::mat4 V, glm::mat4 P, glm::mat4 M) {

	glm::vec4 lp = glm::vec4(0, 3, -6, 1);

	auto sp = this->shader;

	sp->use();//Aktywacja programu cieniującego
	//Przeslij parametry programu cieniującego do karty graficznej
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
	glUniform4fv(sp->u("lp"), 1, glm::value_ptr(lp));

	glEnableVertexAttribArray(sp->a("vertex"));  //Włącz przesyłanie danych do atrybutu vertex
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, positions.data()); //Wskaż tablicę z danymi dla atrybutu vertex

	/*glEnableVertexAttribArray(sp->a("color"));
	glVertexAttribPointer(sp->a("color"), 4, GL_FLOAT, false, 0, colors.data());*/

	glEnableVertexAttribArray(sp->a("texCoord"));
	glVertexAttribPointer(sp->a("texCoord"), 2, GL_FLOAT, false, 0, texCoords.data());

	glEnableVertexAttribArray(sp->a("normal"));
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, normals.data());

	glUniform1i(sp->u("textureMap"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glDrawArrays(GL_TRIANGLES, 0, vertexCount); //Narysuj obiekt

	glDisableVertexAttribArray(sp->a("vertex"));  //Wyłącz przesyłanie danych do atrybutu vertex
	glDisableVertexAttribArray(sp->a("color"));
	//glDisableVertexAttribArray(sp->a("normal"));
	glDisableVertexAttribArray(sp->a("texCoord"));

}




RenderModel::RenderModel()
{
}


