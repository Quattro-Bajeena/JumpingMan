#pragma once



class Camera
{
private:
	glm::vec3 ComputeDir(float angleX, float angleY);


public:
	glm::vec3 position;
	float yaw;
	float pitch;

	Camera();

	glm::mat4 GetViewMatrix();
};

