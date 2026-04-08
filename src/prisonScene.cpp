#include "./header/prisonScene.h"
#include <QDebug>
#include "debug.h"

PrisonScene::PrisonScene() {
	ResourceManager::getInstance().loadPrisonSceneResources();
	setSceneRect(0, 0, 1920, 1080);
	player = new Player();
	player->activate();
	loadMap();
	addItem(player);
	player->setPos(300, 0);
	player->setWalls(walls);

	vector<QGraphicsPixmapItem*> ghosts = player->getAfterImages();
	for (QGraphicsPixmapItem* ghost : ghosts) {
		addItem(ghost);
	}
}

PrisonScene::~PrisonScene() {

}

void PrisonScene::updateScene(double deltaTime) {
	if (debug) displayDebugInfo(deltaTime);
	else clearDebug(views().first());

	if (jon.isConnected()) {
		//inputs.reset();
		jon.RcvFromSerial(&inputs);
	}

	if (gameEndMenu) {
		if (inputs.isRightPressed) {
			selectedButton->unSelect();
			selectedButton = returnMenu;
			selectedButton->select();
		}
		if (inputs.isLeftPressed) {
			selectedButton->unSelect();
			selectedButton = replay;
			selectedButton->select();
		}

		if (inputs.isSpacePressed) emit selectedButton->clicked();

		return;
	}

	if (!player->isAlive()) {
		showDeath();
		return;
	}

	if (inputs.isDebugPressed) {
		debug = !debug;
		inputs.isDebugPressed = false;
	}

	

	timer += deltaTime;

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

	foreach(Hole* hole, holes) {
		if (hole->collidesWithItem(player)) {
			player->damage();
		}
	}

	player->update(deltaTime, inputs);
}

void PrisonScene::showEnd() {
	QGraphicsView* view = views().first();
	QPointF center = view->mapToScene(view->viewport()->rect().center());
	
	qreal width = 600;
	qreal height = 400;
	
	QGraphicsRectItem* overlay = new QGraphicsRectItem(0, 0, width, height);
	overlay->setPos(center.x() - width/2, center.y() - height/2);
	overlay->setBrush(QBrush(QColor(0, 0, 0, 180)));
	overlay->setZValue(10);
	addItem(overlay);

	// message
	QGraphicsTextItem* title = new QGraphicsTextItem("Ahh ouais beau gosse");
	title->setDefaultTextColor(Qt::yellow);
	title->setFont(QFont("Arial", 36, QFont::Bold));

	qreal titleX = overlay->pos().x() + (width / 2) - (title->boundingRect().width()/2);
	qreal titleY = overlay->pos().y() + 50;

	title->setPos(titleX, titleY);
	title->setZValue(11);
	addItem(title);

	// temps
	double finalTime = timer;
	QString timeText = QString("Temps : %1 secondes").arg(finalTime, 0, 'f', 2);
	QGraphicsTextItem* timeDisplay = new QGraphicsTextItem(timeText);
	timeDisplay->setDefaultTextColor(Qt::white);
	timeDisplay->setFont(QFont("Arial", 24));

	qreal timeX = overlay->pos().x() + (width / 2) - (timeDisplay->boundingRect().width() / 2);
	qreal timeY = titleY + 50;
	timeDisplay->setPos(timeX, timeY);
	timeDisplay->setZValue(11);
	addItem(timeDisplay);

	// bouton replay
	replay = new MenuButton("Rejouer!", 200, 50);

	qreal replayX = overlay->pos().x() + (width / 4) - (replay->boundingRect().width() / 2);
	qreal replayY = overlay->pos().y() + 200;
	replay->setPos(replayX, replayY);
	replay->setZValue(11);
	addItem(replay);

	connect(replay, &MenuButton::clicked, this, &PrisonScene::replayGame);

	// bouton retour au menu
	returnMenu = new MenuButton("retourner au menu!", 200, 50);

	qreal returnMenuX = overlay->pos().x() + ((width / 4) * 3) - (returnMenu->boundingRect().width() / 2);
	qreal returnMenuY = overlay->pos().y() + 200;
	returnMenu->setPos(returnMenuX, returnMenuY);
	returnMenu->setZValue(11);
	addItem(returnMenu);

	connect(returnMenu, &MenuButton::clicked, this, &PrisonScene::goToMenu);
	gameEndMenu = true;
	selectedButton = replay;
	selectedButton->select();
}

void PrisonScene::showDeath() {
	QGraphicsView* view = views().first();
	QPointF center = view->mapToScene(view->viewport()->rect().center());

	qreal width = 600;
	qreal height = 400;

	QGraphicsRectItem* overlay = new QGraphicsRectItem(0, 0, width, height);
	overlay->setPos(center.x() - width / 2, center.y() - height / 2);
	overlay->setBrush(QBrush(QColor(0, 0, 0, 180)));
	overlay->setZValue(10);
	addItem(overlay);

	// message
	QGraphicsTextItem* title = new QGraphicsTextItem("Womp womp skill issue");
	title->setDefaultTextColor(Qt::yellow);
	title->setFont(QFont("Arial", 36, QFont::Bold));

	qreal titleX = overlay->pos().x() + (width / 2) - (title->boundingRect().width() / 2);
	qreal titleY = overlay->pos().y() + 50;

	title->setPos(titleX, titleY);
	title->setZValue(11);
	addItem(title);

	// bouton replay
	replay = new MenuButton("Rejouer!", 200, 50);

	qreal replayX = overlay->pos().x() + (width / 4) - (replay->boundingRect().width() / 2);
	qreal replayY = overlay->pos().y() + 200;
	replay->setPos(replayX,replayY);
	replay->setZValue(11);
	addItem(replay);

	connect(replay, &MenuButton::clicked, this, &PrisonScene::replayGame);

	// bouton retour au menu
	returnMenu = new MenuButton("retourner au menu!", 200, 50);

	qreal returnMenuX = overlay->pos().x() + ((width / 4) * 3) - (returnMenu->boundingRect().width() / 2);
	qreal returnMenuY = overlay->pos().y() + 200;
	returnMenu->setPos(returnMenuX, returnMenuY);
	returnMenu->setZValue(11);
	addItem(returnMenu);

	connect(returnMenu, &MenuButton::clicked, this, &PrisonScene::goToMenu);

	gameEndMenu = true;
	selectedButton = replay;
	selectedButton->select();

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
		else if (s == "holes") {
			Hole* hole = new Hole();
			holes.push_back(hole);
			item = hole;
		}
		item->setPos(tile.getXPosition(), tile.getYPosition());
		item->setPixmap(tile.getPixmap());
		item->setFlag(QGraphicsItem::ItemSendsGeometryChanges, tile.isCollide());
		addItem(item);
	}
}

void PrisonScene::keyPressEvent(QKeyEvent* event) {
	if (!KEYBOARD_INPUT) return;
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
	}
}

void PrisonScene::keyReleaseEvent(QKeyEvent* event) {
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
	}
	if (event->key() == Qt::Key_Q) {
		inputs.isDebugPressed = true;
	}
}


QGraphicsItem* PrisonScene::getPlayer() {
	return player;
}

void PrisonScene::replayGame() {
	emit changeScene(Prison);
}

void PrisonScene::goToMenu() {
	emit changeScene(Menu);
}

void PrisonScene::displayDebugInfo(double deltaTime) {
	// section debug
	QGraphicsView* view = views().first();  // récupère la Game view
	clearDebug(view);
	debugText(view, "fps: " + QString::number(1.0 / deltaTime, 'f', 0), 10);
	debugText(view, "pos_x: " + QString::number(player->x()), 30);
	QString s = player->isOnGround() ? "true" : "false";
	debugText(view, "Grounded: " + s, 50);
	float velocity = player->getXVelocity();
	QString speed = QString::number(velocity);
	debugText(view, "X Speed :" + speed, 70);
	int hp = player->getHp();
	QString health = QString::number(hp);
	debugText(view, "Health : " + health, 90);
}