#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>


class Camera
{
private:
	glm::vec3 ComputeDir(float angleX, float angleY);


public:
	float speed;
	float rotateSpeed;
	float rotateMouseSpeed;

	glm::vec3 position;
	glm::quat rotation;
	float yaw;
	float pitch;

	Camera();

	glm::mat4 GetViewMatrix();
	
	void SetPosition(glm::vec3 pos);
	void Move(glm::vec3 dir, float dt);
	void Move(int forward, int side, float dt);
	void Rotate(int pitchChange, int yawChange, float dt);
	void RotateMouse(float pitchChange, float yawChange, float dt);
};

