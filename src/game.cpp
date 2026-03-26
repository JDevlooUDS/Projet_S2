#include "../header/game.h"

Game::Game(QGraphicsScene* scene) : QGraphicsView(scene) {
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setFocusPolicy(Qt::StrongFocus);
	fps = new QTimer(this);
	QObject::connect(fps, &QTimer::timeout, this, &Game::gameLoop);
	fps->start(16);
	elapsedTimer.start();
	setSceneRect(0, -1000, 1920, 50000);
}

Game::~Game() {
}

void Game::gameLoop() {
	qint64 deltaTimeMs = elapsedTimer.restart();
	double deltaTime = deltaTimeMs / 1000.0;

	BaseScene* currentScene = static_cast<BaseScene*>(scene());
	currentScene->updateScene(deltaTime);
	if (PrisonScene* prisonScene = dynamic_cast<PrisonScene*>(currentScene)) {
		centerOn(prisonScene->getPlayer());
	}
}

