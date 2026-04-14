#include "../header/trap.h"
#include "../header/player.h"

Trap::Trap(float slowFactor, float fallSpeedFactor) {
    this->slowFactor = slowFactor;
    this->fallSpeedFactor = fallSpeedFactor;
    collides = false;
}

Trap::~Trap() {}

void Trap::applyEffect(Player* player) {
    player->setSpeedMultiplier(slowFactor);
    player->setFallSpeedMultiplier(fallSpeedFactor);
    player->disableJump();
    // retirer vie, etc
}

void Trap::updateEffect(double deltaTime) {
    // animation ou autres
}

void Trap::update(double deltaTime, const Inputs& inputs) {}

int Trap::type() const {
    return Type;
}