#pragma once
#include "baseScene.h"
#include <QPixmap>
#include "player.h"
#include <QKeyEvent>
#include "wall.h"
#include "resourceManager.h"
#include "jon.h"

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
	Jon jon;

	int   joyX = 512, joyY = 512;
	bool  B1 = false, B2 = false, B3 = false, B4 = false;
	float X = 0, Y = 0, Z = 0, v = 0;
	float gameTime = 0.0f;
	int   bar = 0;
};