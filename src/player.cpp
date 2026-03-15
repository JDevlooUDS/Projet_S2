#include "../header/player.h"

Player::Player() {
	QPixmap playerSprite(":/sprites/placeHolderSprite.png");
	if (playerSprite.isNull())
		qDebug() << "Failed to load sprite!";
	setPixmap(playerSprite);
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