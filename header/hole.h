#pragma once

#include "gameObject.h"
#include "player.h"

class Hole : public GameObject {
public:
	Hole();
	~Hole();
	void update(double deltaTime, const Inputs& inputs) override;

private:

};