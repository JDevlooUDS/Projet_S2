#include "./header/prisonScene.h"
#include <QDebug>

PrisonScene::PrisonScene() {
	setSceneRect(0, 0, 800, 600);
	player = new Player();
	player->activate();
	addItem(player);
}

PrisonScene::~PrisonScene() {
	delete player;
	player = nullptr;
}

void PrisonScene::updateScene(double deltaTime) {
	if (inputs.isLeftPressed) player->move(-1, 0, deltaTime);
	if (inputs.isRightPressed) player->move(1, 0, deltaTime);
}

void PrisonScene::keyPressEvent(QKeyEvent* event) {
	if (event->key() == Qt::Key_A) {
		inputs.isLeftPressed = true;
	}
	if (event->key() == Qt::Key_D) {
		inputs.isRightPressed = true;
	}
	if (event->key() == Qt::Key_Space) {
		if (player->isActive()) player->deactivate();
		else player->activate();
	}
}

void PrisonScene::keyReleaseEvent(QKeyEvent* event) {
	if (event->key() == Qt::Key_A) {
		inputs.isLeftPressed = false;
	}
	if (event->key() == Qt::Key_D) {
		inputs.isRightPressed = false;
	}
}