#include "pch.h"
#include "Object.h"

Object::Object()
{
}

Object::Object(RenderModel* model, Collider collider)
	:model(model), collider(collider), name(model->name)
{

}

void Object::Render(glm::mat4 V, glm::mat4 P, glm::vec3 cameraPos, glm::vec3 lightPos1, glm::vec3 lightPos2)
{
	glm::mat4 M = glm::mat4(1.0f);
	M = glm::translate(M, position);
	glm::mat4 rotM = glm::mat4_cast(rotation); 
	M = M * rotM;
	//M = glm::scale(M, glm::vec3(2, 2, 2));

	model->Render(V, P, M, cameraPos, lightPos1, lightPos2);
}

void Object::SetRotation(float pitch, float yaw, float roll)
{
	rotation = glm::quat(glm::vec3(pitch, yaw, roll));
}
