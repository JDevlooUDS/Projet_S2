#include "../header/mainMenu.h"


MainMenu::MainMenu() {
	setSceneRect(0,0,1920,1080);
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
}

void MainMenu::updateScene(double deltaTime, const Inputs& inputs) {
	QPointF target(1920 / 2, 1080 / 2);
	QGraphicsView* view = views().first();
	view->centerOn(target);
}

void MainMenu::playClicked() {
	emit changeScene(Prison);
}

void MainMenu::tutoClicked() {}

void MainMenu::leaderboardClicked() {}

void MainMenu::settingsClicked() {}

void MainMenu::exitClicked() {
	QApplication::quit();
}