#pragma once
#include "gameObject.h"
#include "resourceManager.h"
#include <QVector2D>
#include <cmath>

enum DashDirection {
	NONE,
	UP,
	UP_RIGHT,
	RIGHT,
	DOWN_RIGHT,
	DOWN,
	DOWN_LEFT,
	LEFT,
	UP_LEFT
};

class Player : public GameObject {
public:
	Player();
	~Player();
	void jump();
	void move(double deltaTime);
	bool isJumping();
	void updateGravity(double deltaTime);
	void disableJump();
	void enableJump();
	void ground();
	void setSpeedMultiplier(float multiplier);
	float getSpeedMultiplier();
	void dash();
	bool isDashing();
	void updateDash(double deltaTime);
	void setDashDirection(DashDirection dashDirection);
	QVector2D getFixedVelocity();
	void setWalls(vector<GameObject*> walls);
	void update(double deltaTime, Inputs& inputs) override;


private:
	void moveX(double deltaTime);
	void moveY(double deltaTime);
	void resolveCollisionX();
	void resolveCollisionY();

	const float BASE_SPEED = 200.0;
	const float DASH_SPEED = 600.0;
	const float JUMP_VELOCITY = -950.0f;
	const float GRAVITY = 3000.0f;
	const float BASE_FALL_VELOCITY = 2000.0f;
	float speedMultiplier = 1.0f; // < 1 pour ralentir, > 1 pour accelerer

	vector<GameObject*> walls;

	double dashTimer = 0.0;
	const double DASH_LIMIT = 0.3;
	bool dashing = false;

	double jumpBufferTimer = 0.0;
	const double JUMP_BUFFER_LIMIT = 0.1;

	float fallVelocity = BASE_FALL_VELOCITY;
	bool isGrounded = true;
	bool jumpEnabled = true;
	bool jumping = false;
	DashDirection dashDirection = NONE;

	int dashCount = 1;
};