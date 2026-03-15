#include "../header/gameObject.h"

GameObject::GameObject() {

}

GameObject::~GameObject()  {

}

void GameObject::move(int x, int y, double deltaTime) {
	if (!active) return;
	moveBy(speed * x * deltaTime, speed * y);
}

void GameObject::damage(int damage) {
	hp -= damage;
}

void GameObject::draw(ostream& os) {

}

bool GameObject::isActive() {
	return active;
}

void GameObject::activate() {
	active = true;
	setVisible(true);
}

void GameObject::deactivate() {
	active = false;
	setVisible(false);
}

void GameObject::setHp(int hp) {
	this->hp = hp;
}

int GameObject::getHp() {
	return hp;
}

void GameObject::setSpeed(int speed) {
	this->speed = speed;
}

int GameObject::getSpeed() {
	return speed;
}

void GameObject::setPosition(int x, int y) {
	pos.x = x;
	pos.y = y;
}

void GameObject::setCollision(bool collides) {
	this->collides = collides;
}

Position GameObject::getPosition() {
	return pos;
}

Position GameObject::getLastPosition() {
	return lastPos;
}