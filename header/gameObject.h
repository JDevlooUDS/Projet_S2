#include <iostream>
using namespace std;

class GameObject {
public:
	GameObject();
	~GameObject();
	void move(int x, int y);
	void damage(int damage);
	bool isActive();
	void draw(ostream& os);
	void setHp(int hp);
	int getHp();
	void setSpeed(int speed);
	int getSpeed();
	void activate();
	void deactivate();
	void setSprite(char sprite);

	

private:
	int hp = 0;
	int speed = 0;
	char sprite = ' ';
	bool active = false;
};