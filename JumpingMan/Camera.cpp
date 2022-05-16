#include "Camera.h"

Camera::Camera()
{
	position = glm::vec3(0, 0, 0);
	rotation = glm::quat(0.f, 0.f, 0.f, 0.f);
	speed = 5;
	rotateSpeed = 1;
	rotateMouseSpeed = 50;
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

	V = glm::lookAt(
		position,
		position + ComputeDir(pitch, yaw),
		glm::vec3(0.0f, 1.0f, 0.0f)); //Wylicz macierz widoku

	return V;
}

void Camera::SetPosition(glm::vec3 pos)
{
	position = pos;
}

void Camera::Move(glm::vec3 dir, float dt)
{
	position += glm::normalize(dir) * speed * dt;
}

void Camera::Move(int forward, int side, float dt)
{
	position += forward * speed * ComputeDir(0, yaw) * dt;
	position += side * speed * ComputeDir(0, yaw + glm::pi<float>() / 2) * dt;
}

void Camera::Rotate(int pitchChange, int yawChange, float dt)
{
	//rotation += glm::quat(glm::vec3(pitch, yaw, 0) * rotateSpeed * dt);
	pitch += pitchChange * rotateSpeed * dt;
	yaw += yawChange * rotateSpeed * dt;

}

void Camera::RotateMouse(float pitchChange, float yawChange, float dt)
{
	//rotation += glm::quat(glm::vec3(pitch, yaw, 0) * rotateSpeed * dt);


	pitch += pitchChange * rotateMouseSpeed * dt;
	yaw += yawChange * rotateMouseSpeed * dt;

}
