#pragma once
#include "gameObject.h"

class Player; // eviter inclusion circulaire

class Trap : public GameObject {
public:
    enum {Type = UserType + 2};
    Trap(float slowFactor, float fallSpeedFactor = 0.3f);
    ~Trap();
    void applyEffect(Player* player);
    void updateEffect(double deltaTime);
    void update(double deltaTime, const Inputs& inputs) override;
    int type() const override;

private:
    float slowFactor;
    float fallSpeedFactor;
};