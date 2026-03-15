#include "../header/sceneManager.h"
#include "sceneManager.h"

SceneManager::SceneManager(Game* game) {
	this->game = game;
	connect(&prisonScene, &BaseScene::changeScene, this, &SceneManager::setScene);
}
SceneManager::~SceneManager() {}

void SceneManager::setScene(SceneType sceneType) {
	switch (sceneType) {
		case Prison:
			game->setScene(&prisonScene);
			break;
	}
}