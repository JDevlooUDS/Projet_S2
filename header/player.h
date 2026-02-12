#include "gameObject.h"

class Player : public GameObject {
public:
	Player();
	~Player();
	void jump();
	void fall();
	bool isJumping();
	void cancelJump();
private:
	bool jumping = false;
};