#pragma once
#include "QGraphicsPixmapItem"
#include "inputs.h"
using namespace std;

struct Position {
	int x = 0;
	int y = 0;
};

class GameObject : public QGraphicsPixmapItem{
public:
	GameObject();
	~GameObject();
	virtual void move(double deltaTime);
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
	virtual void update(double deltaTime, Inputs& inputs) = 0;
	QPointF getLastPosition();

	

protected:
	int hp = 0;
	float xVelocity = 0;
	float yVelocity = 0;
	float speed = 0;
	float reverseSpeed = 0;
	bool active = false;
	bool collides = false;
	QPointF lastPosition;
};