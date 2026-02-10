#include "../header/gameObject.h"

GameObject::GameObject() {

}

GameObject::~GameObject()  {

}

void GameObject::move(int x, int y) {
	if (!active) return;
	lastPos.x = pos.x;
	lastPos.y = pos.y;
	pos.x += x;
	pos.y += y;
}

void GameObject::damage(int damage) {
	
}

void GameObject::draw(ostream& os) {

}

bool GameObject::isActive() {
	return active;
}

void GameObject::activate() {
	active = true;
}

void GameObject::deactivate() {
	active = false;
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

void GameObject::setSprite(char sprite) {
	this->sprite = sprite;
}

char GameObject::getSprite() {
	return sprite;
}

void GameObject::setPosition(int x, int y) {
	pos.x = x;
	pos.y = y;
}

Position GameObject::getPosition() {
	return pos;
}

Position GameObject::getLastPosition() {
	return lastPos;
}