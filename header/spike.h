#pragma once
#include "gameObject.h"

class Spike : public GameObject {
public:
	enum {Type = UserType + 3};
	Spike();
	~Spike();
	void update(double deltaTime, const Inputs& inputs) override;
	int type() const override;

private:
};