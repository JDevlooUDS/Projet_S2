#include "../header/mainMenu.h"


MainMenu::MainMenu() {
	ResourceManager::getInstance().loadMenuResources();
	AudioManager::getInstance().updateMusic(MusicState::MENU);
	setSceneRect(0,0,1920,1080);

	background = ResourceManager::getInstance().getBackground();
}

MainMenu::~MainMenu() {}

void MainMenu::init() {

	qreal width = 1920;
	qreal height = 1080;
	QPointF target(width / 2, height / 2);
	QGraphicsView* view = views().first();
	view->centerOn(target);


	// Titre
	QGraphicsTextItem* title = new QGraphicsTextItem("AGE OF VOLTRON");
	title->setDefaultTextColor(Qt::yellow);
	title->setFont(QFont("Arial", 36, QFont::Bold));

	qreal titleX = (width/2) - (title->boundingRect().width() / 2);
	qreal titleY = 200;

	title->setPos(titleX, titleY);
	title->setZValue(11);
	addItem(title);

	//tutorial
	tutorial = new MenuButton("Tutoriel", 400, 100);

	qreal tutoX = (width / 2) - (tutorial->boundingRect().width() / 2);
	qreal tutoY = title->pos().y() + 75;

	tutorial->setPos(tutoX, tutoY);
	tutorial->setZValue(11);
	addItem(tutorial);

	connect(tutorial, &MenuButton::clicked, this, &MainMenu::tutoClicked);

	//play
	play = new MenuButton("Competition", 400, 100);

	qreal playX = (width / 2) - (play->boundingRect().width() / 2);
	qreal playY = tutorial->pos().y() + 125;

	play->setPos(playX, playY);
	play->setZValue(11);
	addItem(play);

	connect(play, &MenuButton::clicked, this, &MainMenu::playClicked);

	//leaderboardButton
	leaderboard = new MenuButton("Classement", 400, 100);

	qreal leaderX = (width / 2) - (leaderboard->boundingRect().width() / 2);
	qreal leaderY = play->pos().y() + 125;

	leaderboard->setPos(leaderX, leaderY);
	leaderboard->setZValue(11);
	addItem(leaderboard);

	connect(leaderboard, &MenuButton::clicked, this, &MainMenu::leaderboardClicked);

	//settings
	settings = new MenuButton("Options", 400, 100);

	qreal settingsX = (width / 2) - (settings->boundingRect().width() / 2);
	qreal settingsY = leaderboard->pos().y() + 125;

	settings->setPos(settingsX, settingsY);
	settings->setZValue(11);
	addItem(settings);

	connect(settings, &MenuButton::clicked, this, &MainMenu::settingsClicked);

	//exit
	exit = new MenuButton("Quitter", 400, 100);

	qreal exitX = (width / 2) - (exit->boundingRect().width() / 2);
	qreal exitY = settings->pos().y() + 125;

	exit->setPos(exitX, exitY);
	exit->setZValue(11);
	addItem(exit);

	connect(exit, &MenuButton::clicked, this, &MainMenu::exitClicked);

	//slider
	QSlider* volume = new QSlider(Qt::Horizontal);
	volume->setRange(0, 100);
	volume->setValue(AudioManager::getInstance().getVolume());
	slider = addWidget(volume);
	qreal volumeX = (width/2) - (slider->boundingRect().width() / 2);
	qreal volumeY = title->pos().y() + 75;
	slider->setZValue(11);
	slider->setPos(volumeX, volumeY);
	slider->setVisible(false);

	connect(volume, &QSlider::valueChanged, this, [=](int value) {
		emit setVolume(value);
	});

	//bouton retour
	back = new MenuButton("Retour", 400, 100);
	qreal backX = (width/2) - (back->boundingRect().width() / 2);
	qreal backY = slider->pos().y() + 75;
	back->setPos(backX, backY);
	back->setZValue(11);
	back->setVisible(false);
	addItem(back);

	connect(back, &MenuButton::clicked, this, &MainMenu::settingsClicked);


	buttons.push_back(tutorial);
	buttons.push_back(play);
	buttons.push_back(leaderboard);
	buttons.push_back(settings);
	buttons.push_back(exit);
	it = buttons.begin();
	selectedButton = *it;
	selectedButton->select();
}

void MainMenu::updateScene(double deltaTime, const Inputs& inputs) {
	QPointF target(1920 / 2, 1080 / 2);
	QGraphicsView* view = views().first();
	view->centerOn(target);

	changeSelectTimer += deltaTime;
	

	if (showingNamePrompt) {
		
		if (changeSelectTimer >= CHANGE_SELECT_LIMIT) {
			if (inputs.isUpPressed) {
				selectedButton->unSelect();
				selectedButton = confirmNameBtn;
				selectedButton->select();
			}
			else if (inputs.isDownPressed) {
				selectedButton->unSelect();
				selectedButton = returnButton;
				selectedButton->select();
			}
		}

		selectTimer += deltaTime;

		if (selectTimer >= SELECT_SPEED) {
			if (inputs.isSelectPressed && selectedButton != nullptr) emit selectedButton->clicked();
			selectTimer = 0.0;
		}
		return;
	}


	if (changeSelectTimer >= CHANGE_SELECT_LIMIT) {
		if (inputs.isUpPressed) {
			if (it == buttons.begin()) return;
			it--;
			selectedButton->unSelect();
			selectedButton = *it;
			selectedButton->select();
			changeSelectTimer = 0.0;
		}
		if (inputs.isDownPressed) {
			if (it == buttons.end() - 1) return;
			it++;
			selectedButton->unSelect();
			selectedButton = *it;
			selectedButton->select();
			changeSelectTimer = 0.0;
		}
	}

	selectTimer += deltaTime;

	if (selectTimer >= SELECT_SPEED) {
		if (inputs.isSelectPressed && selectedButton != nullptr) emit selectedButton->clicked();
		selectTimer = 0.0;
	}

}

void MainMenu::playClicked() {
	AudioManager::getInstance().playButtonSelectSFX();
	showingNamePrompt = true;
	showNamePrompt();
}

void MainMenu::tutoClicked() {
	AudioManager::getInstance().playButtonSelectSFX();
	emit changeScene(TUTO);
}

void MainMenu::leaderboardClicked() {
	AudioManager::getInstance().playButtonSelectSFX();
}

void MainMenu::settingsClicked() {
	AudioManager::getInstance().playButtonSelectSFX();
	toggleSettings = !toggleSettings;
	if (toggleSettings) {
		selectedButton->unSelect();
		tutorial->setVisible(false);
		play->setVisible(false);
		leaderboard->setVisible(false);
		settings->setVisible(false);
		exit->setVisible(false);
		back->setVisible(true);
		slider->setVisible(true);
		selectedButton = back;
		selectedButton->select();
	}
	else {
		selectedButton->unSelect();
		tutorial->setVisible(true);
		play->setVisible(true);
		leaderboard->setVisible(true);
		settings->setVisible(true);
		exit->setVisible(true);
		selectedButton = tutorial;
		it = buttons.begin();
		selectedButton->select();
		back->setVisible(false);
		slider->setVisible(false);
	}
}

void MainMenu::exitClicked() {
	QApplication::quit();
}

void MainMenu::drawBackground(QPainter* painter, const QRectF& rect) {
	painter->drawPixmap(sceneRect().toRect(), background);
}

void MainMenu::showNamePrompt() {
	overlay = new QGraphicsRectItem(0, 0, 1920, 1080);
	overlay->setBrush(QColor(0, 0, 0, 150));
	overlay->setZValue(20);
	addItem(overlay);

	nameEdit = new QLineEdit();
	nameEdit->setPlaceholderText("Enter your name...");
	nameEdit->setFixedWidth(400);
	nameEdit->setStyleSheet("font-size: 24px; padding: 10px;");

	nameInputProxy = addWidget(nameEdit);
	nameInputProxy->setPos(1920 / 2 - 200, 1080 / 2 - 50);
	nameInputProxy->setZValue(21);

	confirmNameBtn = new MenuButton("Confirmer", 200, 60);
	confirmNameBtn->setPos(1920 / 2 - 100, 1080 / 2 + 50);
	confirmNameBtn->setZValue(21);
	addItem(confirmNameBtn);

	connect(confirmNameBtn, &MenuButton::clicked, this, &MainMenu::onNameConfirmed);
	connect(nameEdit, &QLineEdit::returnPressed, this, &MainMenu::onNameConfirmed);

	//return button
	returnButton = new MenuButton("Annuler", 200, 60);
	qreal returnX = (1920 / 2) - 100;
	qreal returnY = confirmNameBtn->pos().y() + 85;
	returnButton->setPos(returnX, returnY);
	returnButton->setZValue(21);
	addItem(returnButton);
	connect(returnButton, &MenuButton::clicked, this, &MainMenu::cleanNamePrompt);

	selectedButton->unSelect();
	selectedButton = confirmNameBtn;
	selectedButton->select();

	nameInputProxy->setFocusPolicy(Qt::StrongFocus);
	nameEdit->setFocus();
	nameEdit->setEnabled(true);
	nameInputProxy->setFocus();
	views().first()->setInteractive(true);
}

void MainMenu::onNameConfirmed() {
	AudioManager::getInstance().playButtonSelectSFX();
	dynamic_cast<Game*>(views().first())->setPlayerName(nameEdit->text());
	emit changeScene(Prison);
}

void MainMenu::cleanNamePrompt() {
	showingNamePrompt = false;
	if (nameInputProxy != nullptr) {
		removeItem(nameInputProxy);
		nameInputProxy->deleteLater();
		nameInputProxy = nullptr;
	}
	if (nameEdit != nullptr) {
		nameEdit->deleteLater();
		nameEdit = nullptr;
	}
	if (confirmNameBtn != nullptr) {
		removeItem(confirmNameBtn);
		confirmNameBtn->deleteLater();
		confirmNameBtn = nullptr;
	}
	if (overlay != nullptr) {
		removeItem(overlay);
		delete overlay;
		overlay = nullptr;
	}
	if (returnButton != nullptr) {
		removeItem(returnButton);
		returnButton->deleteLater();
		returnButton = nullptr;
	}
	selectedButton = tutorial;
	selectedButton->select();
}

void MainMenu::keyPressEvent(QKeyEvent* event) {
	QGraphicsScene::keyPressEvent(event);
}