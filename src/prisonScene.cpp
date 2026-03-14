#include "./header/prisonScene.h"
#include <QDebug>

PrisonScene::PrisonScene() {
	setSceneRect(0, 0, 800, 600);
	QPixmap playerSprite(":/sprites/placeHolderSprite.png");
	if (playerSprite.isNull())
		qDebug() << "Failed to load sprite!";
	addPixmap(playerSprite);
}

PrisonScene::~PrisonScene() {}