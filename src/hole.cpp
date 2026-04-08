#include "../header/Hole.h"

Hole::Hole() {}

Hole::~Hole() {}

void Hole::update(double deltaTime, const Inputs& inputs) {}

void applyEffect(Player* player) {
	player->damage();
}