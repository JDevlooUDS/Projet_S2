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

	// pieges et boosts
	bool touchingTrap = false;
	foreach(Trap * trap, traps) {
		if (trap->collidesWithItem(player)) {
			trap->applyEffect(player);
			touchingTrap = true;
		}
	}

	bool touchingBoost = false;
	foreach(Boost* boost, boosts) {
		if (boost->collidesWithItem(player)) {
			boost->applyEffect(player);
			touchingBoost = true;
		}
	}

	if (!touchingTrap && !touchingBoost) {
		player->setSpeedMultiplier(1.0f);
		player->enableJump();
	}

	foreach(End* end, endZones) {
		if (end->collidesWithItem(player)) {
			// arret timer?
			showEnd();
			return;
		}
	}

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

void PrisonScene::showEnd() {
	QGraphicsRectItem* overlay = new QGraphicsRectItem(660, 340, 600, 400);
	overlay->setBrush(QBrush(QColor(0, 0, 0, 180)));
	overlay->setZValue(10);
	addItem(overlay);

	// message
	QGraphicsTextItem* title = new QGraphicsTextItem("Ahh ouais beau gosse");
	title->setDefaultTextColor(Qt::yellow);
	title->setFont(QFont("Arial", 36, QFont::Bold));
	title->setPos(700, 370);
	title->setZValue(11);
	addItem(title);

	// temps
	double finalTime = 6.7; // pour l'exemple
	QString timeText = QString("Temps : %1 secondes").arg(finalTime, 0, 'f', 2);
	QGraphicsTextItem* timeDisplay = new QGraphicsTextItem(timeText);
	timeDisplay->setDefaultTextColor(Qt::white);
	timeDisplay->setFont(QFont("Arial", 24));
	timeDisplay->setPos(730, 460);
	timeDisplay->setZValue(11);
	addItem(timeDisplay);

	// bouton rejouer
	// ...
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
		else if (s == "trap") {
			Trap* trap = new Trap(0.4f);
			traps.push_back(trap);
			item = trap;
		}
		else if (s == "boost") {
			Boost* boost = new Boost(2.5f);
			boosts.push_back(boost);
			item = boost;
		}
		else if (s == "end") {
			End* end = new End();
			endZones.push_back(end);
			item = end;
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