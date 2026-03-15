#pragma once
#include "gameObject.h"

class Player : public GameObject {
public:
	Player();
	~Player();
	void jump();
	void fall();
	bool isJumping();
	void cancelJump();

	bool jumping = false;
};