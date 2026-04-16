#include "./header/prisonScene.h"
#include <QDebug>
#include "debug.h"

PrisonScene::PrisonScene() {
	ResourceManager::getInstance().loadPrisonSceneResources(":/map/compet.json");
	setSceneRect(0, 0, 16000, 1600);
	player = new Player();
	player->activate();
	loadMap();
	addItem(player);
	lastFrameHp = player->getHp();
	player->setPos(ResourceManager::getInstance().playerSpawnPoint);
	if (!Jon::getInstance().isConnected()) {
		//while (!Jon::getInstance().openPort());
	}
	
	Jon::getInstance().SendToSerial(false, false, false, true ,true, true);

	vector<QGraphicsPixmapItem*> ghosts = player->getAfterImages();
	for (QGraphicsPixmapItem* ghost : ghosts) {
		ghost->setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
		ghost->setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
		ghost->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
		ghost->setEnabled(false);
		addItem(ghost);
 	}

	QPixmap filled = ResourceManager::getInstance().getFilledhealth();
	for (int i = 0; i < 3; i++) {
		healths.push_back(filled);
	}

	for (int i = 0; i < 3; i++) {
		FallingStar* star = new FallingStar();
		star->deactivate();
		addItem(star);
		fallingStars.push_back(star);
	}
	setItemIndexMethod(QGraphicsScene::BspTreeIndex);

	AudioManager::getInstance().updateMusic(MusicState::GAMEPLAY);
	background = ResourceManager::getInstance().getBackground();

	rankMessages['S'] = "Légende!";
	rankMessages['A'] = "Éclair!";
	rankMessages['B'] = "Bonne vitesse!";
	rankMessages['C'] = "Pas mal!";
	rankMessages['D'] = "T'as ton diplôme!";
	rankMessages['E'] = "Trop lent...";

	tipMessage['S'] = "";
	tipMessage['A'] = "Technique ultime: Dash depuis le sol permet de buffer un jump et le dash est conservé";
	tipMessage['B'] = "Technique 3: Dasher puis sauter d’un bloc de vitesse conserve le momentum, continuer de sauter permet de garder cette vitesse.";
	tipMessage['C'] = "Technique 2: Aller sur des piques permet de passer par des endroits autrement inaccessibles.";
	tipMessage['D'] = "Technique 1: Être dans un ralentisseur redonne le dash.";
	tipMessage['E'] = "";

}

PrisonScene::~PrisonScene() {}

void PrisonScene::updateScene(double deltaTime, const Inputs& inputs) {
	if (deltaTime > 0.5f) return;
	if (debug) displayDebugInfo(deltaTime);
	else clearDebug(views().first());

	QPointF target = player->pos();
	QGraphicsView* view = views().first();
	QPointF current = view->mapToScene(view->viewport()->rect().center());

	double smoothing = 8.0f;

	QPointF newCenter = current + (target - current) * smoothing * deltaTime;
	view->centerOn(newCenter);

	if (Jon::getInstance().isConnected()) {
		Jon::getInstance().SendTime(timer);
		Jon::getInstance().SendBar(player->getAccelerationMapped());
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

		selectTimer += deltaTime;

		if (selectTimer >= SELECT_SPEED) {
			if (inputs.isSelectPressed && selectedButton != nullptr) emit selectedButton->clicked();
			selectTimer = 0.0;
		}

		return;
	}

	

	if ((inputs.isPausePressed || cancelPause) && pauseTimer >= TOGGLE_PAUSE_LIMIT) {
		pause = !pause;
		cancelPause = false;
		pauseTimer = 0.0;
		if (pause) {
			showPause(inputs);
		}
		else {
			cleanPause();
		}
	}
	pauseTimer += deltaTime;

	if (pause) {
		if (changeSelectTimer < CHANGE_SELECT_LIMIT) {
			changeSelectTimer += deltaTime;
			return;
		}
		if (inputs.isUpPressed) {
			if (it == pauseButtons.begin()) return;
			it--;
			selectedButton->unSelect();
			selectedButton = *it;
			selectedButton->select();
			changeSelectTimer = 0.0;
		}
		if (inputs.isDownPressed) {
			if (it == pauseButtons.end() - 1) return;
			it++;
			selectedButton->unSelect();
			selectedButton = *it;
			selectedButton->select();
			changeSelectTimer = 0.0;
		}

		selectTimer += deltaTime;

		if (selectTimer >= SELECT_SPEED) {
			if (inputs.isSelectPressed && selectedButton != nullptr) emit selectedButton->clicked();
			selectTimer = 0.0;
		}

		return;
	}

	if (lastFrameHp > player->getHp()) {
		healths[player->getHp()] = ResourceManager::getInstance().getEmptyHealth();
	}

	if (!player->isAlive()) {
		showDeath();
		player->deactivate();
		AudioManager::getInstance().playGameOverSFX();
		return;
	}

	if (inputs.isDebugPressed) {
		debug = !debug;
	}

	AudioManager::getInstance().setVolume(inputs.volume);
	timer += deltaTime;

	foreach(End * end, endZones) {
		if (end->collidesWithItem(player)) {
			rank = computeRank();
			LeaderboardManager::getInstance().saveScore(static_cast<Game*>(view)->getPlayerName(), timer);
			showEnd();
			return;
		}
	}
	
	if (inputs.muon) {
		foreach(FallingStar* star, fallingStars) {
			if (!star->isActive()) {
				QGraphicsView* view = views().first();
				int height = view->viewport()->rect().height();
				int width = view->viewport()->rect().width();

				qreal starY = rand() % height;
				qreal starX = rand() % width;

				QPointF starPos = view->mapToScene(starX, starY);

				star->activate();
				star->setVisible(true);
				star->setPos(starPos);
				break;
			}
		}
	}

	foreach(FallingStar* star, fallingStars) {
		if (star->isActive()) {
			star->update(deltaTime, inputs);
		}
	}
	

	player->update(deltaTime, inputs);
}

void PrisonScene::showEnd() {
	QGraphicsView* view = views().first();
	QPointF center = view->mapToScene(view->viewport()->rect().center());
	
	qreal width = 600;
	qreal height = 400;
	
	overlay = new QGraphicsRectItem(0, 0, width, height);
	overlay->setPos(center.x() - width/2, center.y() - height/2);
	overlay->setBrush(QBrush(QColor(0, 0, 0, 180)));
	overlay->setZValue(10);
	addItem(overlay);

	// message
	title = new QGraphicsTextItem(rankMessages[rank]);
	title->setDefaultTextColor(Qt::yellow);
	title->setFont(QFont("Arial", 36, QFont::Bold));

	qreal titleX = overlay->pos().x() + (width / 2) - (title->boundingRect().width()/2);
	qreal titleY = overlay->pos().y() + 50;

	title->setPos(titleX, titleY);
	title->setZValue(11);
	addItem(title);
	// message
	tip = new QGraphicsTextItem(tipMessage[rank]);
	tip->setDefaultTextColor(Qt::yellow);
	tip->setFont(QFont("Arial", 16, QFont::Bold));

	qreal tipX = overlay->pos().x() + (width / 2) - (tip->boundingRect().width() / 2);
	qreal tipY = title->pos().y() + 50;

	tip->setPos(tipX, tipY);
	tip->setZValue(11);
	addItem(tip);

	// temps
	double finalTime = timer;
	QString timeText = QString("Temps : %1 secondes").arg(finalTime, 0, 'f', 2);
	timeDisplay = new QGraphicsTextItem(timeText);
	timeDisplay->setDefaultTextColor(Qt::white);
	timeDisplay->setFont(QFont("Arial", 24));

	qreal timeX = overlay->pos().x() + (width / 2) - (timeDisplay->boundingRect().width() / 2);
	qreal timeY = tipY + 50;
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

	overlay = new QGraphicsRectItem(0, 0, width, height);
	overlay->setPos(center.x() - width / 2, center.y() - height / 2);
	overlay->setBrush(QBrush(QColor(0, 0, 0, 180)));
	overlay->setZValue(10);
	addItem(overlay);

	// message
	title = new QGraphicsTextItem("Womp womp skill issue");
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

void PrisonScene::showPause(const Inputs& inputs) {
	QGraphicsView* view = views().first();
	QPointF center = view->mapToScene(view->viewport()->rect().center());

	qreal width = 600;
	qreal height = 400;

	overlay = new QGraphicsRectItem(0, 0, width, height);
	overlay->setPos(center.x() - width / 2, center.y() - height / 2);
	overlay->setBrush(QBrush(QColor(0, 0, 0, 180)));
	overlay->setZValue(10);
	addItem(overlay);

	// message
	title = new QGraphicsTextItem("Pause");
	title->setDefaultTextColor(Qt::yellow);
	title->setFont(QFont("Arial", 36, QFont::Bold));

	qreal titleX = overlay->pos().x() + (width / 2) - (title->boundingRect().width() / 2);
	qreal titleY = overlay->pos().y() + 50;

	title->setPos(titleX, titleY);
	title->setZValue(11);
	addItem(title);

	//bouton continuer

	continueButton = new MenuButton("Continuer", 200, 50);

	qreal continueX = overlay->pos().x() + (width / 2) - (continueButton->boundingRect().width() / 2);
	qreal continueY = title->pos().y() + 75;

	continueButton->setPos(continueX, continueY);
	continueButton->setZValue(11);
	addItem(continueButton);
	
	connect(continueButton, &MenuButton::clicked, this, &PrisonScene::clickContinue);

	// bouton replay
	replay = new MenuButton("Rejouer!", 200, 50);

	qreal replayX = overlay->pos().x() + (width / 2) - (replay->boundingRect().width() / 2);
	qreal replayY = continueButton->pos().y() + 75;
	replay->setPos(replayX, replayY);
	replay->setZValue(11);
	addItem(replay);

	connect(replay, &MenuButton::clicked, this, &PrisonScene::replayGame);

	// bouton settings
	settings = new MenuButton("Options", 200, 50);

	qreal settingsX = overlay->pos().x() + (width / 2) - (settings->boundingRect().width() / 2);
	qreal settingsY = replay->pos().y() + 75;
	settings->setPos(settingsX, settingsY);
	settings->setZValue(11);
	addItem(settings);

	connect(settings, &MenuButton::clicked, this, &PrisonScene::clickSettings);

	// bouton retour au menu
	returnMenu = new MenuButton("retourner au menu!", 200, 50);

	qreal returnMenuX = overlay->pos().x() + (width / 2) - (returnMenu->boundingRect().width() / 2);
	qreal returnMenuY = settings->pos().y() + 75;
	returnMenu->setPos(returnMenuX, returnMenuY);
	returnMenu->setZValue(11);
	addItem(returnMenu);

	connect(returnMenu, &MenuButton::clicked, this, &PrisonScene::goToMenu);

	//slider de son
	QSlider* volume = new QSlider(Qt::Horizontal);
	volume->setRange(0,100);
	volume->setValue(inputs.volume * 100);
	slider = addWidget(volume);
	qreal volumeX = overlay->pos().x() + (width / 2) - (slider->boundingRect().width() / 2);
	qreal volumeY = title->pos().y() + 75;
	slider->setZValue(11);
	slider->setPos(volumeX, volumeY);
	slider->setVisible(false);
	
	connect(volume, &QSlider::valueChanged, this, [=](int value) {
		emit setVolume(value);
	});

	//bouton retour
	backButton = new MenuButton("Retour", 200, 50);
	qreal backX = overlay->pos().x() + (width / 2) - (backButton->boundingRect().width() / 2);
	qreal backY = slider->pos().y() + 75;
	backButton->setPos(backX, backY);
	backButton->setZValue(11);
	backButton->setVisible(false);
	addItem(backButton);

	connect(backButton, &MenuButton::clicked, this, &PrisonScene::clickSettings);

	pauseButtons.push_back(continueButton);
	pauseButtons.push_back(replay);
	pauseButtons.push_back(settings);
	pauseButtons.push_back(returnMenu);
	it = pauseButtons.begin();
	selectedButton = *it;
	selectedButton->select();
}

void PrisonScene::cleanPause() {
	if (continueButton != nullptr) {
		removeItem(continueButton);
		delete continueButton;
		continueButton = nullptr;
	}
	if (replay != nullptr) {
		removeItem(replay);
		delete replay;
		replay = nullptr;
	}
	if (returnMenu != nullptr) {
		removeItem(returnMenu);
		delete returnMenu;
		returnMenu = nullptr;
	}
	if (settings != nullptr) {
		removeItem(settings);
		delete settings;
		settings = nullptr;
	}
	if (backButton != nullptr) {
		removeItem(backButton);
		delete backButton;
		backButton = nullptr;
	}
	if (slider != nullptr) {
		removeItem(slider);
		slider->deleteLater();
		slider = nullptr;
	}

	selectedButton = nullptr;

	if (overlay != nullptr) {
		removeItem(overlay);
		delete overlay;
		overlay = nullptr;
	}
	if (title != nullptr) {
		removeItem(title);
		delete title;
		title = nullptr;
	}
	pauseButtons.clear();
	toggleSettings = false;
}

void PrisonScene::loadMap() {
	vector<Tile> tiles = ResourceManager::getInstance().getTiles();
	foreach(Tile tile, tiles) {
		GameObject* item = nullptr;
		std::string s = tile.getName().toStdString();
		if (s == "walls") {
			item = new Wall();
			item->setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
			item->setFlag(QGraphicsItem::ItemIsSelectable, false);
			item->setAcceptedMouseButtons(Qt::NoButton);
			item->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
		}
		else if (s == "trap") {
			Trap* trap = new Trap(0.4f);
			traps.push_back(trap);
			item = trap;
		}
		else if (s == "boost") {
			Boost* boost = new Boost(1.8f);
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
			hole->setVisible(false);
			holes.push_back(hole);
			item = hole;
		}
		else if (s == "spikes") {
			Spike* spike = new Spike();
			spikes.push_back(spike);
			item = spike;
		}
		else if (s == "deco") {
			item = new Deco();
			item->setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
			item->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
		}
		else continue;
		item->setPos(tile.getXPosition(), tile.getYPosition());
		item->setPixmap(tile.getPixmap());
		addItem(item);
	}
}





void PrisonScene::drawForeground(QPainter* painter, const QRectF& rect) {
	painter->save();

	painter->setWorldTransform(QTransform());

	for (int i = 0; i < 3; i++) {
		painter->drawPixmap(i*50 + 12, 12, healths[i]);
	}

	painter->setFont(QFont("Arial", 10));
	painter->drawText(20,60,QString("Temps (s) : %1").arg(timer));

	painter->restore();
}

void PrisonScene::drawBackground(QPainter* painter, const QRectF& rect) {
	QRect viewportRect = views().first()->viewport()->rect();
	QSize currentSize = viewportRect.size();

	if (currentSize != lastSize) {
		scaledBackground = background.scaled(currentSize,
			Qt::KeepAspectRatioByExpanding,
			Qt::SmoothTransformation);
		lastSize = currentSize;
	}

	painter->setWorldMatrixEnabled(false);

	painter->drawPixmap(0, 0, scaledBackground);

	painter->setWorldMatrixEnabled(true);
}

void PrisonScene::replayGame() {
	AudioManager::getInstance().playButtonSelectSFX();
	emit changeScene(Prison);
}

void PrisonScene::goToMenu() {
	AudioManager::getInstance().playButtonSelectSFX();
	emit changeScene(Menu);
}

void PrisonScene::clickContinue() {
	cancelPause = true;
}

void PrisonScene::clickSettings() {
	toggleSettings = !toggleSettings;
	if (toggleSettings) {
		replay->setVisible(false);
		settings->setVisible(false);
		returnMenu->setVisible(false);
		continueButton->setVisible(false);
		slider->setVisible(true);
		backButton->setVisible(true);
		selectedButton = backButton;
		selectedButton->select();
	}
	else {
		replay->setVisible(true);
		settings->setVisible(true);
		returnMenu->setVisible(true);
		continueButton->setVisible(true);
		selectedButton = continueButton;
		selectedButton->select();
		slider->setVisible(false);
		backButton->setVisible(false);
	}
}

void PrisonScene::displayDebugInfo(double deltaTime) {
	// section debug
	QGraphicsView* view = views().first();  // r�cup�re la Game view
	clearDebug(view);
	debugText(view, "fps: " + QString::number(1.0 / deltaTime, 'f', 0), 10, 500);
	debugText(view, "pos_x: " + QString::number(player->x()), 30, 500);
	QString s = player->isOnGround() ? "true" : "false";
	debugText(view, "Grounded: " + s, 50, 500);
	float velocity = player->getXVelocity();
	QString speed = QString::number(velocity);
	debugText(view, "X Speed :" + speed, 70, 500);
	int hp = player->getHp();
	QString health = QString::number(hp);
	debugText(view, "Health : " + health, 90, 500);
}

char PrisonScene::computeRank() {
	return 'D';
}