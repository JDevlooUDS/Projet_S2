#pragma once
#include "QGraphicsPixmapItem"
using namespace std;

struct Position {
	int x = 0;
	int y = 0;
};

class GameObject : public QGraphicsPixmapItem{
public:
	GameObject();
	~GameObject();
	void move(int x, int y, double deltaTime);
	void damage(int damage);
	bool isActive();
	void draw(ostream& os);
	void setHp(int hp);
	int getHp();
	void setSpeed(int speed);
	int getSpeed();
	void activate();
	void deactivate();
	void setPosition(int x, int y);
	Position getPosition();
	Position getLastPosition();

	

protected:
	int hp = 0;
	int speed = 0;
	bool active = false;
	Position pos;
	Position lastPos;
};