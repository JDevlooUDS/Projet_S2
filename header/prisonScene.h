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
#include "menuButton.h"

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
	void drawForeground(QPainter* painter, const QRectF& rect) override;

private:
	void showEnd();
	void showDeath();

	void replayGame();
	void goToMenu();

	void displayDebugInfo(double deltaTime);

	Player* player = nullptr;
	bool playerCollides = false;
	vector<GameObject*> walls;
	vector<Trap*> traps;
	vector<Boost*> boosts;
	vector<End*> endZones;
	vector<Hole*> holes;
	Jon jon;
	bool gameEndMenu = false;

	double timer = 0.0;

	const bool KEYBOARD_INPUT = true; // regarde le clavier seulement si vrai

	MenuButton* replay = nullptr;
	MenuButton* returnMenu = nullptr;

	MenuButton* selectedButton = nullptr;

	bool debug = false;

	vector<QPixmap> healths;


};