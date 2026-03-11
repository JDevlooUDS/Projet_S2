#pragma once
#include <QGraphicsScene>
#include "baseScene.h"
#include <QPixmap>
#include "player.h"

class PrisonScene : public BaseScene {
public:
	PrisonScene(QGraphicsView *parent = nullptr);
	~PrisonScene();

private:
	Player player;
};