#pragma once
#include "gameObject.h"

class Deco : public GameObject {
public:
	enum {Type = UserType + 7};
	Deco();
	~Deco();
	void update(double deltaTime, const Inputs& inputs) override;
	int type() const override;
private:
};
