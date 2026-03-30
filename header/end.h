#pragma once
#include "gameObject.h"

class End : public GameObject {
public:
    End();
    ~End();
    void update(double deltaTime, Inputs& inputs) override;
};