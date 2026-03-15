#include "../header/player.h"

Player::Player() {
	setPixmap(ResourceManager::getInstance().getPlayerSprite());
	setFlag(QGraphicsItem::ItemIsFocusable);
	setFocus();
	hp = 3;
	speed = 200;
}

Player::~Player() {

}

void Player::move(int x, int y, double deltaTime) {
	if (!active) return;
	lastPosition = pos();
	moveBy(speed * x * deltaTime, fallVelocity * y * deltaTime);
}

void Player::jump() {
	if (!isGrounded) return;
	fallVelocity = JUMP_VELOCITY;
	isGrounded = false;
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
}