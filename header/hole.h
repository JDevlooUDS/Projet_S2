#pragma once

#include "gameObject.h"
#include "player.h"

class Hole : public GameObject {
public:
	enum {Type = UserType + 4};
	Hole();
	~Hole();
	void update(double deltaTime, const Inputs& inputs) override;
	int type() const override;

private:

};