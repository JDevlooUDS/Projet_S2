#pragma once
#include "gameObject.h"
#include "resourceManager.h"
#include "timer.h"
#include <QVector2D>
#include <cmath>
#include "audioManager.h"
#include "wall.h"


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
	void setFallSpeedMultiplier(float multiplier);
	void dash(const Inputs& inputs);
	bool isDashing();
	void updateDash(double deltaTime);
	void setDashDirection(DashDirection dashDirection);
	void updateFlip(double deltaTime);
	QVector2D getFixedVelocity();
	void update(double deltaTime, const Inputs& inputs) override;
	float getXVelocity();
	QRectF boundingRect() const override;
	QPainterPath shape() const override;
	vector<QGraphicsPixmapItem*> getAfterImages();
	bool isAlive();
	void resetAcceleration();
	void damage() override;
	int getAccelerationMapped();
	void setInvinsible();
	void replace();

	void setInBoost(bool inBoost);
	void resetDash();


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

	void manageDashState(double deltaTime, const Inputs& inputs);
	void manageNormalState(double deltaTime, const Inputs& inputs);

	void manageAnimation(double deltaTime);
	void setAnimation(const AnimationState state);


	const bool KEYBOARD_CONTROL = true;

	const float BASE_SPEED = 300.0; 
	const float REVERSE_SPEED = 450.0f; //pas utilise?
	const float JUMP_VELOCITY = -770.0f;

	const float GRAVITY = 3000.0f;
	const float BASE_FALL_VELOCITY = 500.0f;
	const float UP_DASH_VELOCITY_RETAIN = 250.0f; // vitesse vers le haut conservee apres un dash UP
	const float FLIP_CONST = 0.2f;
	float speedMultiplier = 1.0f; // < 1 pour ralentir, > 1 pour accelerer

	float facingRight = true;
	float flipHoldTime = 0.0f;

	double dashTimer = 0.0;
	const double DASH_LIMIT = 0.15; //longeur dash
	bool dashing = false;
	bool isJumpingFromDash = false;

	double jumpBufferTimer = 0.0;
	const double JUMP_BUFFER_LIMIT = 0.2;

	float fallVelocity = BASE_FALL_VELOCITY;
	float fallSpeedMultiplier = 1.0f;
	bool isGrounded = true;
	bool jumpEnabled = true;
	bool jumping = false;
	DashDirection dashDirection = NONE;

	int dashCount = 1;
	const float DASH_MULTIPLIER = 2.8f; //longueur dash
	const float WAVE_DASH_MULTIPLIER = 1.2f;
	float dashXVelocity;
	float dashYVelocity;

	double coyoteTimer = 0.0;
	const double COYOTE_TIME_LIMIT = 0.1;
	bool wasGroundedLastFrame = false;

	const float MAX_ACCELERATION = 2;
	const float MIN_ACCELERATION = 1;
	const float ACCELERATION_DELTA = 0.5; //0.5 * deltaTime -> 1 a 2 fois la vitesse en 2s
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
	const double AFTER_IMAGE_SPEED = 0.02f;

	bool isDead = false;
	QPointF lastGroundPosition;

	const float ACCEL_BONUS = 1.2f;
	float accelBonus = 1;

	bool invinsible = false;
	double invinsibilityTimer = 0.0;
	const double INVINSIBILITY_LIMIT = 1.5;
	const double FLICKER_LIMIT = 0.1;
	double flickerTimer = 0.0;

	bool inBoost = false;
	double inBoostBuffer = 0.0;
	const double BOOST_BUFFER_LIMIT = 0.1;
};