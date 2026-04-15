#include "../header/deco.h"

Deco::Deco() {
	collides = false;
	setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
}

Deco::~Deco() {}

void Deco::update(double deltaTime, const Inputs& inputs) {}

int Deco::type() const {
	return Type;
}
