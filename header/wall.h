#pragma once
#include "gameObject.h"

class Wall : public GameObject {
public:
	Wall();
	~Wall();
	void update(double deltaTime, const Inputs& inputs) override;
private:
};