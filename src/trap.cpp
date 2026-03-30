#include "../header/trap.h"
#include "../header/player.h"

Trap::Trap(float slowFactor) {
    this->slowFactor = slowFactor;
    collides = false;
}

Trap::~Trap() {}

void Trap::applyEffect(Player* player) {
    player->setSpeedMultiplier(slowFactor);
    player->disableJump();
    // retirer vie, etc
}

void Trap::updateEffect(double deltaTime) {
    // animation ou autres
}

void Trap::update(double deltaTime, Inputs& inputs) {}