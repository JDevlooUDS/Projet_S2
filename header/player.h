#include "gameObject.h"

class Player : public GameObject {
public:
	Player();
	~Player();
	void jump();
	void fall();
	bool isJumping();
private:
	bool jumping = false;
};