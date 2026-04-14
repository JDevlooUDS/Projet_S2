#include "../header/wall.h"

Wall::Wall() {
	collides = true;
	setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
}

Wall::~Wall() {}

void Wall::update(double deltaTime, const Inputs& inputs) {}

int Wall::type() const {
	return Type;
}