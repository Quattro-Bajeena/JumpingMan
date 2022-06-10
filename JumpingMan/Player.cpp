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
	onGround = false;
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

	glm::vec3 dir = glm::vec3(0.f);
	if (forward != 0 || side != 0) {
		dir = (float)forward * ComputeDir(0, yaw) + (float)side * ComputeDir(0, yaw + glm::pi<float>() / 2);
	}

	direction = dir;
	prevPosition = position;
	position += dir * speed * dt;



	//gravity
	if (!onGround)
	{
		position.y += velocityY;
		velocityY -= gravity;
	}
	else
	{
		velocityY = 0;
		gravity = 0;
	}


	if (position.y <= floorLevel) {
		position.y = floorLevel;
		//std::cout << "spadniecie na podlage" << "\n";
		onGround = true;
	}
	else if (position.y > floorLevel + 0.18) {
		onGround = false;
		gravity = 0.01;
	}

	//reset pos if fall to low
	//if (position.y < -10) pos = glm::vec3(position.x, 5, position.z);


}
void Player::Jump() {
	if (onGround)
	{
		velocityY = 0.75;
		gravity = 0.01;
		onGround = false;
	}
}
void Player::GoToStartingPos() {
	position = glm::vec3(0, 0, 0);
}
void Player::Rotate(float pitchChange, float yawChange, float dt)
{

	pitch += pitchChange * rotateMouseSpeed * dt;
	yaw += yawChange * rotateMouseSpeed * dt;

	pitch = std::clamp(pitch, -1.2f, 1.2f);

	//std::cout << pitch << " " << yaw << "\n";

}
