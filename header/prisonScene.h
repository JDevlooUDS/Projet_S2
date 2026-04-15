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
#include "fallingStar.h"
#include "QSlider"
#include "QGraphicsProxyWidget"
#include "game.h"
#include "leaderboardManager.h"

class PrisonScene : public BaseScene {
	Q_OBJECT
public:
	PrisonScene();
	~PrisonScene();
	void updateScene(double deltaTime, const Inputs& inputs) override;

signals:
	void setVolume(int volume);

protected:
	void loadMap();
	void drawForeground(QPainter* painter, const QRectF& rect) override;
	void drawBackground(QPainter* painter, const QRectF& rect) override;

private:
	void showEnd();
	void showDeath();
	void showPause(const Inputs& inputs);

	void cleanPause();

	void replayGame();
	void goToMenu();
	void clickContinue();
	void clickSettings();
	void displayDebugInfo(double deltaTime);


	Player* player = nullptr;
	bool playerCollides = false;
	vector<Trap*> traps;
	vector<Boost*> boosts;
	vector<End*> endZones;
	vector<Hole*> holes;
	vector<Spike*> spikes;
	bool gameEndMenu = false;

	double timer = 0.0;

	const bool KEYBOARD_INPUT = true;

	MenuButton* replay = nullptr;
	MenuButton* settings = nullptr;
	MenuButton* returnMenu = nullptr;
	MenuButton* continueButton = nullptr;
	MenuButton* backButton = nullptr;
	vector<MenuButton*> pauseButtons;
	vector<MenuButton*>::iterator it;
	QGraphicsProxyWidget* slider = nullptr;

	MenuButton* selectedButton = nullptr;

	double selectTimer = 0.0;
	const double SELECT_SPEED = 0.08;

	bool toggleSettings = false;
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

	vector<FallingStar*> fallingStars;	

	int lastFrameHp;

	bool cancelPause = false;

	QPixmap background;
	QPixmap scaledBackground;
	QSize lastSize;

};