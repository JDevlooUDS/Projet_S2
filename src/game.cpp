#include "../header/game.h"
#include <QDirIterator>

Game::Game(QGraphicsScene* scene) : QGraphicsView(scene) {
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setFocusPolicy(Qt::StrongFocus);
	fps = new QTimer(this);
	fps->setTimerType(Qt::PreciseTimer);
	QObject::connect(fps, &QTimer::timeout, this, &Game::gameLoop);
	fps->start(16);
	elapsedTimer.start();
	setSceneRect(0, -1000, 1920, 50000);


	if (!AudioManager::getInstance().init(this)) {
		qDebug() << "error loading audio files\n";
		return;
	}
	AudioManager::getInstance().updateMusic(MusicState::GAMEPLAY);
}

Game::~Game() {
	AudioManager::getInstance().clean();
}

void Game::gameLoop() {
	if (!scene()) return;
	qint64 deltaTimeMs = elapsedTimer.restart();
	double deltaTime = deltaTimeMs / 1000.0;

	BaseScene* currentScene = static_cast<BaseScene*>(scene());
	currentScene->updateScene(deltaTime);
	if (PrisonScene* prisonScene = dynamic_cast<PrisonScene*>(currentScene)) {
		QPointF target = prisonScene->getPlayer()->pos();
		QPointF current = mapToScene(viewport()->rect().center());

		double smoothing = 8.0f;

		QPointF newCenter = current + (target - current) * smoothing * deltaTime;
		centerOn(newCenter);
	}
}

