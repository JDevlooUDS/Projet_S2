#pragma once
#include "baseScene.h"
#include "menuButton.h"
#include "resourceManager.h"
#include <QPainter>
#include "audioManager.h"
#include "leaderboardManager.h"

class LeaderboardScene : public BaseScene {
public:
	LeaderboardScene();
	~LeaderboardScene();
	void updateScene(double deltaTime, const Inputs& inputs) override;

protected:
	void drawBackground(QPainter* painter, const QRectF& rect) override;
	void showLeaderboardOverlay();

private:

	MenuButton* menu = nullptr;
	MenuButton* selectedButton = nullptr;

	double selectTimer = 0.0;
	const double SELECT_SPEED = 0.3;

	QPixmap background;

	QGraphicsRectItem* overlay = nullptr;
};