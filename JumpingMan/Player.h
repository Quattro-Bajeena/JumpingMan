#pragma once
class Player
{
private:
	glm::vec3 ComputeDir(float angleX, float angleY);


public:
	float rotateSpeed;
	float rotateMouseSpeed;

	float acceleration;
	float deceleration;

	
	float speed;
	float maxSpeed;
	glm::vec3 direction;

	glm::vec3 prevPosition;
	glm::vec3 position;
	float yaw;
	float pitch;

	//jumping variables
	float velocityY;
	float gravity;
	float floorLevel;

	Player();

	void Move(int forward, int side, float dt);
	void Jump();
	void Rotate(float pitchChange, float yawChange, float dt);
};

