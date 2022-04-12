#include "RenderModel.h"




RenderModel::RenderModel()
{
}

RenderModel::RenderModel(float* vertices, float* texCoords, int vertexCount, GLuint texture)
	:vertices(vertices), texCoords(texCoords), vertexCount(vertexCount), texture(texture),
	position(glm::vec3(0))
{
}

void RenderModel::Render()
{
	glm::mat4 translate = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 rotate = glm::mat4_cast(rotation);
	glm::mat4 transform = translate * rotate;
	

	glUniformMatrix4fv(spTextured->u("M"), 1, false, glm::value_ptr(transform));

	glEnableVertexAttribArray(spTextured->a("vertex"));
	glVertexAttribPointer(spTextured->a("vertex"), 4, GL_FLOAT, false, 0, vertices);

	glEnableVertexAttribArray(spTextured->a("texCoord"));
	glVertexAttribPointer(spTextured->a("texCoord"), 2, GL_FLOAT, false, 0, texCoords);

	glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(spTextured->u("tex"), 0);

	glDrawArrays(GL_TRIANGLES, 0, vertexCount);

	glDisableVertexAttribArray(spTextured->a("vertex"));
	glDisableVertexAttribArray(spTextured->a("texCoord"));
}
