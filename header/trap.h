#pragma once
#include "gameObject.h"

class Player; // eviter inclusion circulaire

class Trap : public GameObject {
public:
    Trap(float slowFactor);
    ~Trap();
    void applyEffect(Player* player);
    void updateEffect(double deltaTime);

private:
    float slowFactor;
};