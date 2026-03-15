#pragma once
#include "gameObject.h"
#include "resourceManager.h"

class Player : public GameObject {
public:
	Player();
	~Player();
	void jump();
	void move(int x, int y, double deltaTime);
	bool isJumping();
	void updateJump(double deltaTime);
	void ground();

	bool jumping = false;

private:
	const float JUMP_VELOCITY = -950.0f;
	const float GRAVITY = 3000.0f;
	const float BASE_FALL_VELOCITY = 500.0f;

	float fallVelocity = BASE_FALL_VELOCITY;
	bool isGrounded = true;
};