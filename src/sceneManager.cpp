#include "../header/sceneManager.h"
#include "sceneManager.h"

SceneManager::SceneManager(Game* game) {
	this->game = game;
}
SceneManager::~SceneManager() {
}

void SceneManager::setScene(SceneType sceneType) {

	QGraphicsScene* oldScene = game->scene();
	if (oldScene) oldScene->deleteLater();

	switch (sceneType) {
		case Prison:
			prisonScene = new PrisonScene();
			connect(prisonScene, &BaseScene::changeScene, this, &SceneManager::setScene);
			game->setScene(prisonScene);
			break;
		case Menu:
			menuScene = new BaseScene(); // changer pour MenuScene quand créer
			connect(menuScene, &BaseScene::changeScene, this, &SceneManager::setScene);
			game->setScene(menuScene);
			break;
	}
}