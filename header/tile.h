#pragma once
#include <QPixmap>

class Tile {
public:
	Tile(int x, int y, int height, int lenght, bool collides, QPixmap& sprite, QString& name);
	~Tile();

	int getHeight();
	int getLenght();
	int getXPosition();
	int getYPosition();
	QPixmap& getPixmap();
	QString& getName();
	bool isCollide();
private:
	bool collides;
	int height;
	int lenght;
	int x;
	int y;
	QPixmap sprite;
	QString name;
};