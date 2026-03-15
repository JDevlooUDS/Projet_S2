#pragma once
#include "baseScene.h"
#include <QPixmap>
#include "player.h"
#include <QKeyEvent>
#include "resourceManager.h"

class PrisonScene : public BaseScene {
public:
	PrisonScene();
	~PrisonScene();
	void updateScene(double deltaTime) override;

protected:
	void loadMap();
	void keyPressEvent(QKeyEvent* event) override;
	void keyReleaseEvent(QKeyEvent* event) override;

private:
	Player* player = nullptr;
};