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
#include "audioManager.h"
#include "spike.h"

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
	void showPause();

	void cleanPause();

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
	vector<Spike*> spikes;
	bool gameEndMenu = false;

	double timer = 0.0;

	const bool KEYBOARD_INPUT = true; // regarde le clavier seulement si vrai

	MenuButton* replay = nullptr;
	MenuButton* settings = nullptr;
	MenuButton* returnMenu = nullptr;
	vector<MenuButton*> pauseButtons;
	vector<MenuButton*>::iterator it;
	MenuButton* selectedButton = nullptr;

	bool debug = false;

	vector<QPixmap> healths;


	bool pause = false;
	double changeSelectTimer = 0.0;
	const double CHANGE_SELECT_LIMIT = 0.2;

	double pauseTimer = 0.0;
	const double TOGGLE_PAUSE_LIMIT = 0.2;

	QGraphicsRectItem* overlay = nullptr;
	QGraphicsTextItem* title = nullptr;
	QGraphicsTextItem* timeDisplay = nullptr;

};