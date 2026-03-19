#include "../header/player.h"

Player::Player() {
	setPixmap(ResourceManager::getInstance().getPlayerSprite());
	setFlag(QGraphicsItem::ItemIsFocusable);
	setFocus();
	hp = 3;
	speed = BASE_SPEED;
}

Player::~Player() {

}

void Player::move(int x, int y, double deltaTime) {
	if (!active) return;
	lastPosition = pos();
	float dx = x;
	float dy = y;
	if (x != 0 && y != 0) {
		dx = std::sqrt(1 / 2);
		dy = std::sqrt(1 / 2);
	}
	moveBy(speed * dx * deltaTime, fallVelocity * dy * deltaTime);
}

void Player::jump() {
	if (!isGrounded && jumpCount == 0) return;
	fallVelocity = JUMP_VELOCITY;
	isGrounded = false;
	jumpCount--;
}

bool Player::isJumping() {
	return !isGrounded;
}

void Player::updateJump(double deltaTime) {
	fallVelocity += GRAVITY * deltaTime;
	if (fallVelocity > BASE_FALL_VELOCITY) fallVelocity = BASE_FALL_VELOCITY;
}

void Player::ground() {
	isGrounded = true;
	if (!dashing) dashCount = 1;
	jumpCount = 2;
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