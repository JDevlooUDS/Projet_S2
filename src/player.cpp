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

void Player::jump() {
	Position p = getPosition();
	lastPos.x = p.x;
	lastPos.y = p.y;
	setPosition(p.x, p.y - 1);
	jumping = true;
}

void Player::fall() {
	Position p = getPosition();
	lastPos.x = p.x;
	lastPos.y = p.y;
	setPosition(p.x, p.y + 1);
	jumping = false;
}

bool Player::isJumping() {
	return jumping;
}

void Player::cancelJump() {
	jumping = false;
}