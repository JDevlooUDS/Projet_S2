#include "../header/game.h"
#include <QDirIterator>

Game::Game(QGraphicsScene* scene) : QGraphicsView(scene) {
	setViewport(new QOpenGLWidget());
	setRenderHint(QPainter::Antialiasing);
	setRenderHint(QPainter::SmoothPixmapTransform);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setFocusPolicy(Qt::StrongFocus);
	fps = new QTimer(this);
	fps->setTimerType(Qt::PreciseTimer);
	QObject::connect(fps, &QTimer::timeout, this, &Game::gameLoop);
	fps->start(16);
	elapsedTimer.start();
	setSceneRect(0, -9000, 19920, 50000); // plus grand pis c chill


	if (!AudioManager::getInstance().init(this)) {
		qDebug() << "error loading audio files\n";
		return;
	}
	inputs.volume = 0.5f;
}

Game::~Game() {
	AudioManager::getInstance().clean();
}

void Game::gameLoop() {
	if (!scene()) return;
	qint64 deltaTimeMs = elapsedTimer.restart();
	double deltaTime = deltaTimeMs / 1000.0;
	BaseScene* currentScene = static_cast<BaseScene*>(scene());
	if (Jon::getInstance().isConnected()) {
		Jon::getInstance().RcvFromSerial(&inputs);
	}
	else {
		//while (!Jon::getInstance().openPort());
	}
	AudioManager::getInstance().setVolume(inputs.volume);
	currentScene->updateScene(deltaTime, inputs);
}

void Game::keyPressEvent(QKeyEvent* event) {
	if (event->isAutoRepeat()) return;
	event->accept();
	if (event->key() == Qt::Key_A) {
		inputs.isLeftPressed = true;
	}
	if (event->key() == Qt::Key_Left) {
		inputs.isLeftPressed = true;
	}
	if (event->key() == Qt::Key_D) {
		inputs.isRightPressed = true;
	}
	if (event->key() == Qt::Key_Right) {
		inputs.isRightPressed = true;
	}
	if (event->key() == Qt::Key_E) {
		inputs.isDashPressed = true;
	}
	if (event->key() == Qt::Key_W) {
		inputs.isUpPressed = true;
	}
	if (event->key() == Qt::Key_Up) {
		inputs.isUpPressed = true;
	}
	if (event->key() == Qt::Key_S) {
		inputs.isDownPressed = true;
	}
	if (event->key() == Qt::Key_Down) {
		inputs.isDownPressed = true;
	}
	if (event->key() == Qt::Key_Space) {
		inputs.isSpacePressed = true;
		inputs.isSelectPressed = true;
	}
	if (event->key() == Qt::Key_P) {
		inputs.isPausePressed = true;
	}
}

void Game::keyReleaseEvent(QKeyEvent* event) {
	if (event->isAutoRepeat()) return;
	event->accept();
	if (event->key() == Qt::Key_A) {
		inputs.isLeftPressed = false;
	}
	if (event->key() == Qt::Key_Left) {
		inputs.isLeftPressed = false;
	}
	if (event->key() == Qt::Key_D) {
		inputs.isRightPressed = false;
	}
	if (event->key() == Qt::Key_Right) {
		inputs.isRightPressed = false;
	}
	if (event->key() == Qt::Key_E) {
		inputs.isDashPressed = false;
	}
	if (event->key() == Qt::Key_W) {
		inputs.isUpPressed = false;
	}
	if (event->key() == Qt::Key_Up) {
		inputs.isUpPressed = false;
	}
	if (event->key() == Qt::Key_S) {
		inputs.isDownPressed = false;
	}
	if (event->key() == Qt::Key_Down) {
		inputs.isDownPressed = false;
	}
	if (event->key() == Qt::Key_Space) {
		inputs.isSpacePressed = false;
		inputs.isSelectPressed = false;
	}
	if (event->key() == Qt::Key_Q) {
		inputs.isDebugPressed = true;
	}
	if (event->key() == Qt::Key_P) {
		inputs.isPausePressed = false;
	}
}

void Game::setVolume(int volume) {
	inputs.volume = volume / 100.0f;
}

