#pragma once
#include <QGraphicsScene>
#include <QPixmap>
#include "player.h"

class PrisonScene : public QGraphicsScene {
public:
	PrisonScene(QGraphicsView *parent = nullptr);
	~PrisonScene();

private:
	Player player;
};