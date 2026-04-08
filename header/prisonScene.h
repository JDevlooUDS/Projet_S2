#pragma once
#include "baseScene.h"
#include <QPixmap>
#include "player.h"
#include <QKeyEvent>
#include "wall.h"
#include "trap.h"
#include "boost.h"
#include "end.h"
#include "resourceManager.h"
#include "Jon.h"
#include "hole.h"
#include "debug.h"

class PrisonScene : public BaseScene {
public:
	PrisonScene();
	~PrisonScene();
	void updateScene(double deltaTime) override;
	QGraphicsItem* getPlayer();

protected:
	void loadMap();
	void keyPressEvent(QKeyEvent* event) override;
	void keyReleaseEvent(QKeyEvent* event) override;

private:
	void showEnd();

	Player* player = nullptr;
	bool playerCollides = false;
	vector<GameObject*> walls;
	vector<Trap*> traps;
	vector<Boost*> boosts;
	vector<End*> endZones;
	vector<Hole*> holes;
	Jon jon;

	const bool KEYBOARD_INPUT = true; // regarde le clavier seulement si vrai
};