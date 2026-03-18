#pragma once
#include "gameObject.h"

class Player; 

class Boost : public GameObject {
public:
    Boost(float speedMultiplier);
    ~Boost();

    void applyEffect(Player* player);

private:
    float speedMultiplier;
};