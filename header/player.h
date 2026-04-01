#pragma once
#include "gameObject.h"
#include "resourceManager.h"
#include "timer.h"
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
	void updateFlip(double deltaTime);
	QVector2D getFixedVelocity();
	void setWalls(vector<GameObject*> walls);
	void update(double deltaTime, Inputs& inputs) override;


private:
	void moveX(double deltaTime);
	void moveY(double deltaTime);
	void resolveCollisionX();
	void resolveCollisionY();
	void manageDashDirection(Inputs& inputs);
	void manageAcceleration(double deltaTime);
	void resetAcceleration();


	const bool KEYBOARD_CONTROL = true;

	const float BASE_SPEED = 200.0;
	const float REVERSE_SPEED = 300.0f;
	const float DASH_SPEED = 600.0;
	const float JUMP_VELOCITY = -950.0f;

	const float GRAVITY = 4000.0f;
	const float BASE_FALL_VELOCITY = 500.0f;
	const float FLIP_CONST = 0.2f;
	float speedMultiplier = 1.0f; // < 1 pour ralentir, > 1 pour accelerer

	float facingRight = true;
	float flipHoldTime = 0.0f;

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

	double coyoteTimer = 0.0;
	const double COYOTE_TIME_LIMIT = 0.1;
	bool wasGroundedLastFrame = false;

	const float MAX_ACCELERATION = 2;
	const float MIN_ACCELERATION = 1;
	const float ACCELERATION_DELTA = 0.5; //0.5 * deltaTime -> 1 à 2 fois la vitesse en 2s
	float accelerationMultiplier = 1;
	float lastFrameXVelocity = 0.0;
};