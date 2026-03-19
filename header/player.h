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
	void move(int x, int y, double deltaTime);
	bool isJumping();
	void updateJump(double deltaTime);
	void ground();
	void dash();
	bool isDashing();
	void updateDash(double deltaTime);
	void setDashDirection(DashDirection dashDirection);
	QVector2D getFixedVelocity();

	bool jumping = false;

private:
	const float BASE_SPEED = 200.0;
	const float DASH_SPEED = 600.0;
	const float JUMP_VELOCITY = -950.0f;
	const float GRAVITY = 3000.0f;
	const float BASE_FALL_VELOCITY = 500.0f;

	double dashTimer = 0.0;
	const double DASH_LIMIT = 0.3;
	bool dashing = false;

	float fallVelocity = BASE_FALL_VELOCITY;
	bool isGrounded = true;
	DashDirection dashDirection = NONE;

	int jumpCount = 2;
	int dashCount = 1;
};