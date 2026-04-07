#include "../header/boost.h"
#include "../header/player.h"

Boost::Boost(float speedMultiplier) {
    this->speedMultiplier = speedMultiplier;
    collides = false;
}

Boost::~Boost() {}

void Boost::applyEffect(Player* player) {
    player->setSpeedMultiplier(speedMultiplier);
    player->enableJump();
}

void Boost::update(double deltaTime, const Inputs& inputs) {}