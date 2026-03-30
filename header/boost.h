#pragma once
#include "gameObject.h"

class Player; 

class Boost : public GameObject {
public:
    Boost(float speedMultiplier);
    ~Boost();
    void update(double deltaTime, Inputs& inputs) override;

    void applyEffect(Player* player);

private:
    float speedMultiplier;
};