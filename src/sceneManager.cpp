#include "../header/sceneManager.h"
#include "sceneManager.h"

SceneManager::SceneManager(Game* game) {
	this->game = game;
}
SceneManager::~SceneManager() {
	if (prisonScene != nullptr) delete prisonScene;
	if (menuScene != nullptr) delete menuScene;
}

void SceneManager::setScene(SceneType sceneType) {
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