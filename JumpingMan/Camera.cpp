#include "pch.h"
#include "Camera.h"

Camera::Camera()
{
	position = glm::vec3(0, 0, 0);
	yaw = 0;
	pitch = 0;
}

glm::vec3 Camera::ComputeDir(float angleX, float angleY) {
	glm::vec4 dir(0, 0, 1, 0);
	glm::mat4 M = glm::mat4(1.0f);
	M = glm::rotate(M, angleY, glm::vec3(0, 1, 0));
	M = glm::rotate(M, angleX, glm::vec3(1, 0, 0));
	dir = M * dir;
	return glm::vec3(dir);
}

glm::mat4 Camera::GetViewMatrix()
{

	glm::mat4 V = glm::lookAt(
		position, 
		position + ComputeDir(pitch, yaw), 
		glm::vec3(0.0f, 1.0f, 0.0f)); //Wylicz macierz widoku

	return V;
}
