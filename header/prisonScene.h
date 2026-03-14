#pragma once
#include <QGraphicsScene>
#include "baseScene.h"
#include <QPixmap>
#include "player.h"

class PrisonScene : public BaseScene {
public:
	PrisonScene();
	~PrisonScene();

private:
	Player player;
};