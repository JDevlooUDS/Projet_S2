#include "./header/prisonScene.h"
#include <QDebug>

PrisonScene::PrisonScene() {
	ResourceManager::getInstance().loadPrisonSceneResources();
	setSceneRect(0, 0, 1920, 1080);
	player = new Player();
	player->activate();
	loadMap();
	addItem(player);
	player->setPos(300, 0);
}

PrisonScene::~PrisonScene() {
	delete player;
	player = nullptr;
}

void PrisonScene::updateScene(double deltaTime) {
	int dx = 0;
	int dy = 1;
	if (inputs.isLeftPressed) dx += -1;
	if (inputs.isRightPressed) dx += 1;
	if (inputs.isSpacePressed) player->jump();

	if (player->isJumping()) player->updateJump(deltaTime);

	player->move(0, dy, deltaTime);

	foreach(GameObject * wall, walls) {
		if (wall->collidesWithItem(player)) {
			player->setY(player->getLastPosition().y());
			player->ground();
		}
	}

	player->move(dx, 0, deltaTime);

	foreach(GameObject * wall, walls) {
		if (wall->collidesWithItem(player)) {
			player->setX(player->getLastPosition().x());
		}
	}
}

void PrisonScene::loadMap() {
	vector<Tile> tiles = ResourceManager::getInstance().getTiles();
	foreach(Tile tile, tiles) {
		GameObject* item = nullptr;
		std::string s = tile.getName().toStdString();
		if (s == "walls") {
			item = new Wall();
			walls.push_back(item);
		}
		else if (s == "grass") {
			item = new GameObject();
			item->setCollision(false);
			grass.push_back(item);
		}
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
		inputs.isSpacePressed = true;
	}
}

void PrisonScene::keyReleaseEvent(QKeyEvent* event) {
	if (event->key() == Qt::Key_A) {
		inputs.isLeftPressed = false;
	}
	if (event->key() == Qt::Key_D) {
		inputs.isRightPressed = false;
	}
	if (event->key() == Qt::Key_Space) {
		inputs.isSpacePressed = false;
	}
}