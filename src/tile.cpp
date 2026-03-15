#include "../header/tile.h"

Tile::Tile(int x, int y, int height, int lenght, bool collides, QPixmap& sprite, QString& name) {
	this->x = x;
	this->y = y;
	this->height = height;
	this->lenght = lenght;
	this->collides = collides;
	this->sprite = sprite;
	this->name = name;
}

Tile::~Tile() {}

int Tile::getHeight() {
	return height;
}

int Tile::getLenght() {
	return lenght;
}

int Tile::getXPosition() {
	return x;
}

int Tile::getYPosition() {
	return y;
}

QPixmap& Tile::getPixmap() {
	return sprite;
}

QString& Tile::getName() {
	return name;
}

bool Tile::isCollide() {
	return collides;
}

