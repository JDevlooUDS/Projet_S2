#include "./header/prisonScene.h"
#include <QDebug>

PrisonScene::PrisonScene() {
	ResourceManager::getInstance().loadPrisonSceneResources();
	setSceneRect(0, 0, 1920, 1080);
	player = new Player();
	player->activate();
	addItem(player);
	player->setPos(300, 0);
	loadMap();
}

PrisonScene::~PrisonScene() {
	delete player;
	player = nullptr;
}

void PrisonScene::updateScene(double deltaTime) {
	if (inputs.isLeftPressed) player->move(-1, 0, deltaTime);
	if (inputs.isRightPressed) player->move(1, 0, deltaTime);
}

void PrisonScene::loadMap() {
	vector<Tile> tiles = ResourceManager::getInstance().getTiles();
	foreach(Tile tile, tiles) {
		QGraphicsPixmapItem* item = new QGraphicsPixmapItem();
		item->setPos(tile.getXPosition(), tile.getYPosition());
		item->setPixmap(tile.getPixmap());
		item->setFlag(QGraphicsItem::ItemSendsGeometryChanges, tile.isCollide());
		addItem(item);
	}
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