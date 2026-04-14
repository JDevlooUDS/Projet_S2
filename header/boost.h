#pragma once
#include "gameObject.h"

class Player; 

class Boost : public GameObject {
public:
    enum {Type = UserType + 6};
    Boost(float speedMultiplier);
    ~Boost();
    void update(double deltaTime, const Inputs& inputs) override;

    void applyEffect(Player* player);
    int type() const override;

private:
    float speedMultiplier;
};