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
	virtual void move(int x, int y, double deltaTime);
	void damage(int damage);
	bool isActive();
	void draw(ostream& os);
	void setHp(int hp);
	int getHp();
	void setSpeed(int speed);
	int getSpeed();
	void activate();
	void deactivate();
	void setCollision(bool collides);
	QPointF getLastPosition();

	

protected:
	int hp = 0;
	float speed = 0;
	float reverseSpeed = 0;
	bool active = false;
	bool collides = false;
	QPointF lastPosition;
};