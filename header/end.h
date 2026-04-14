#pragma once
#include "gameObject.h"

class End : public GameObject {
public:
    enum {Type = UserType + 5};
    End();
    ~End();
    void update(double deltaTime, const Inputs& inputs) override;
    int type() const override;
};