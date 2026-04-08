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

enum PlayerState {
	NORMAL,
	DASH
};

enum AnimationState {
	IDLE,
	RUN,
	DASH_ANIM,
	JUMP
};

class Player : public GameObject {
public:
	Player();
	~Player();
	void jump();
	void move(double deltaTime);
	bool isJumping();
	bool isOnGround();
	void updateGravity(double deltaTime);
	void disableJump();
	void enableJump();
	void ground();
	void setSpeedMultiplier(float multiplier);
	float getSpeedMultiplier();
	void dash(const Inputs& inputs);
	bool isDashing();
	void updateDash(double deltaTime);
	void setDashDirection(DashDirection dashDirection);
	void updateFlip(double deltaTime);
	QVector2D getFixedVelocity();
	void setWalls(vector<GameObject*> walls);
	void update(double deltaTime, const Inputs& inputs) override;
	float getXVelocity();
	QRectF boundingRect() const override;
	QPainterPath shape() const override;
	vector<QGraphicsPixmapItem*> getAfterImages();

	bool isAlive();

	void damage() override;


private:
	void moveX(double deltaTime);
	void moveY(double deltaTime);
	void moveDash(double deltaTime);
	void moveDashX(double deltaTime);
	void moveDashY(double deltaTime);

	void resolveCollisionX();
	void resolveCollisionY();
	void manageDashDirection(const Inputs& inputs);
	void manageAcceleration(double deltaTime);
	void resetAcceleration();

	void manageDashState(double deltaTime, const Inputs& inputs);
	void manageNormalState(double deltaTime, const Inputs& inputs);

	void manageAnimation(double deltaTime);
	void setAnimation(const AnimationState state);


	const bool KEYBOARD_CONTROL = true;

	const float BASE_SPEED = 300.0;
	const float REVERSE_SPEED = 450.0f;
	const float JUMP_VELOCITY = -750.0f;

	const float GRAVITY = 3000.0f;
	const float BASE_FALL_VELOCITY = 1000.0f;
	const float FLIP_CONST = 0.2f;
	float speedMultiplier = 1.0f; // < 1 pour ralentir, > 1 pour accelerer

	float facingRight = true;
	float flipHoldTime = 0.0f;

	vector<GameObject*> walls;

	double dashTimer = 0.0;
	const double DASH_LIMIT = 0.2;
	bool dashing = false;
	bool isJumpingFromDash = false;

	double jumpBufferTimer = 0.0;
	const double JUMP_BUFFER_LIMIT = 0.2;

	float fallVelocity = BASE_FALL_VELOCITY;
	bool isGrounded = true;
	bool jumpEnabled = true;
	bool jumping = false;
	DashDirection dashDirection = NONE;

	int dashCount = 1;
	const float DASH_MULTIPLIER = 4.0f;
	const float WAVE_DASH_MULTIPLIER = 1.5f;
	float dashXVelocity;
	float dashYVelocity;

	double coyoteTimer = 0.0;
	const double COYOTE_TIME_LIMIT = 0.1;
	bool wasGroundedLastFrame = false;

	const float MAX_ACCELERATION = 2;
	const float MIN_ACCELERATION = 1;
	const float ACCELERATION_DELTA = 0.5; //0.5 * deltaTime -> 1 à 2 fois la vitesse en 2s
	float accelerationMultiplier = 1;
	float lastFrameXVelocity = 0.0;

	PlayerState playerState = NORMAL;

	AnimationState animationState = IDLE;
	vector<QPixmap> runAnimation;
	vector<QPixmap> idleAnimation;
	vector<QPixmap> jumpAnimation;
	vector<QPixmap> dashAnimation;
	int animationIndex = 0;
	double animationTimer = 0.0f;
	const double ANIMATION_SPEED = 0.1f;

	vector<QGraphicsPixmapItem*> afterImages;
	int const AFTER_IMAGE_MAX = 5;
	int afterImageIndex = 0;
	double afterImageTimer = 0.0f;
	const double AFTER_IMAGE_SPEED = 0.05f;

	bool isDead = false;
	QPointF lastGroundPosition;
};