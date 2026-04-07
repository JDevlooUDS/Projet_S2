#pragma once
#include "gameObject.h"

class End : public GameObject {
public:
    End();
    ~End();
    void update(double deltaTime, const Inputs& inputs) override;
};