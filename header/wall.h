#pragma once
#include "gameObject.h"

class Wall : public GameObject {
public:
	enum {Type = UserType + 1};
	Wall();
	~Wall();
	void update(double deltaTime, const Inputs& inputs) override;
	int type() const override;
private:
};