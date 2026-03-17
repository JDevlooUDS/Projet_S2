#pragma once
#include "baseScene.h"
#include <QPixmap>
#include "player.h"
#include <QKeyEvent>
#include "wall.h"
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
	bool playerJumpedLastFrame = false;
	Player* player = nullptr;
	bool playerCollides = false;
	vector<GameObject*> walls;
	vector<GameObject*> grass;
};