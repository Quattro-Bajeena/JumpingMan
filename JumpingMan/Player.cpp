#include "pch.h"
#include "Player.h"
#include "pch.h"
#include "Camera.h"

Player::Player()
{
	position = glm::vec3(0, 0, 0);
	rotateSpeed = 1;
	rotateMouseSpeed = 50;

	acceleration = 20;
	maxSpeed = 10;

	velocityY = 0;
	gravity = 0.01;
	floorLevel = 0;
}

glm::vec3 Player::ComputeDir(float angleX, float angleY) {
	glm::vec4 dir(0, 0, 1, 0);
	glm::mat4 M = glm::mat4(1.0f);
	M = glm::rotate(M, angleY, glm::vec3(0, 1, 0));
	M = glm::rotate(M, angleX, glm::vec3(1, 0, 0));
	dir = M * dir;
	return glm::vec3(dir);
}




void Player::Move(int forward, int side, float dt)
{

	if (forward != 0 || side != 0) {
		speed += acceleration * dt;
	}
	else {
		speed = 0;
	}
	speed = std::clamp(speed, 0.f, maxSpeed);


	//std::cout << speed << "\n";

	glm::vec3 dir = glm::vec3(0.f);
	if (forward != 0 || side != 0) {
		dir = (float)forward * ComputeDir(0, yaw) + (float)side * ComputeDir(0, yaw + glm::pi<float>() / 2);
	}

	direction = dir;
	prevPosition = position;

	position += dir * speed * dt;

	//gravity
	position.y += velocityY;
	velocityY -= gravity;
	glm::vec3 pos = glm::vec3(position.x, position.y, position.z);

	if (position.y < floorLevel) {
		pos = glm::vec3(position.x, floorLevel, position.z);
		velocityY = 0;
	}

	//reset pos if fall to low
	//if (position.y < -10) pos = glm::vec3(position.x, 5, position.z);

	position = pos;

}
void Player::Jump() {
	velocityY = 0.25;
}
void Player::Rotate(float pitchChange, float yawChange, float dt)
{

	pitch += pitchChange * rotateMouseSpeed * dt;
	yaw += yawChange * rotateMouseSpeed * dt;

	//std::cout << pitch << " " << yaw << "\n";

}
