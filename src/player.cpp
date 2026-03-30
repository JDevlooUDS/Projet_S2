#include "../header/player.h"


Player::Player() {
	setPixmap(ResourceManager::getInstance().getPlayerSprite());
	setFlag(QGraphicsItem::ItemIsFocusable);
	setFocus();
	hp = 3;
	speed = BASE_SPEED;
	reverseSpeed = REVERSE_SPEED;
}

Player::~Player() {

}

void Player::move(int x, int y, double deltaTime) {
	bool pressingOpposite = (x > 0 && !facingRight) || (x < 0 && facingRight);

	if (!active) return;
	lastPosition = pos();
	if (pressingOpposite) {
		moveBy(reverseSpeed * speedMultiplier * x * deltaTime, fallVelocity * y * deltaTime);
	}
	else {
	moveBy(speed * speedMultiplier * x * deltaTime, fallVelocity * y * deltaTime);
	}
}

void Player::updateFlip(int x, double deltaTime) {
	if (!isGrounded || x == 0) return;  // accumule seulement, ne reset PAS si x==0

	bool pressingOpposite = (x > 0 && !facingRight) || (x < 0 && facingRight);

	if (pressingOpposite) {
		flipHoldTime += deltaTime;
		if (flipHoldTime >= FLIP_CONST) {
			facingRight = !facingRight;
			setPixmap(pixmap().transformed(QTransform().scale(-1, 1)));
			flipHoldTime = 0.0f;
		}
	}
	else {
		flipHoldTime = 0.0f;  // reset seulement si on va dans la bonne direction
	}
}

void Player::jump() {
	if ((!isGrounded && jumpCount == 0) || !jumpEnabled) return;
 	fallVelocity = JUMP_VELOCITY;
	isGrounded = false;
	jumpCount--;
}

bool Player::isJumping() {
	return !isGrounded;
}

void Player::updateJump(double deltaTime) {
	fallVelocity += GRAVITY * deltaTime;
	if (fallVelocity > BASE_FALL_VELOCITY) {
		fallVelocity = BASE_FALL_VELOCITY;
	}
}

void Player::disableJump() {
	jumpEnabled = false; 
}

void Player::enableJump() {
	jumpEnabled = true; 
}

void Player::ground() {
	isGrounded = true;
	jumpCount = 1;
	dashCount = 1;
}

void Player::setSpeedMultiplier(float multiplier) {
	speedMultiplier = multiplier;
}

float Player::getSpeedMultiplier() {
	return speedMultiplier;
	if (!dashing) dashCount = 1;
	jumpCount = 1;
}

void Player::dash() {
	if (dashCount == 0) return;
	speed = DASH_SPEED;
	fallVelocity = DASH_SPEED;
	dashing = true;
	dashCount--;
}

bool Player::isDashing() {
	return dashing;
}

void Player::updateDash(double deltaTime) {
	if (dashTimer >= DASH_LIMIT) {
		dashing = false;
		dashTimer = 0.0;
		speed = BASE_SPEED;
		fallVelocity = 0;
		dashDirection = NONE;
		return;
	}
	dashTimer += deltaTime;
}

void Player::setDashDirection(DashDirection dashDirection) {
	this->dashDirection = dashDirection;
}

QVector2D Player::getFixedVelocity() {
	switch (dashDirection) {
		case UP:
			return QVector2D(0.0,-1.0);
		case UP_RIGHT:
			return QVector2D(1.0, -1.0);
		case RIGHT:
			return QVector2D(1.0,0.0);
		case DOWN_RIGHT:
			return QVector2D(1.0,1.0);
		case DOWN:
			return QVector2D(0.0,1.0);
		case DOWN_LEFT:
			return QVector2D(-1.0,1.0);
		case LEFT:
			return QVector2D(-1.0,0.0);
		case UP_LEFT:
			return QVector2D(-1.0,-1.0);
		default:
			return QVector2D(0,0);
	}
}