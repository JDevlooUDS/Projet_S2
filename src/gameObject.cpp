#include "../header/gameObject.h"

GameObject::GameObject() {

}

GameObject::~GameObject()  {

}

void GameObject::move(double deltaTime) {
	if (!active) return;
	lastPosition = pos();
	moveBy(speed * xVelocity * deltaTime, speed * yVelocity * deltaTime);
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

void GameObject::setCollision(bool collides) {
	this->collides = collides;
}

QPointF GameObject::getLastPosition() {
	return lastPosition;
}