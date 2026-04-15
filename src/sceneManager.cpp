#include "../header/sceneManager.h"

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
			connect(prisonScene, &PrisonScene::setVolume, game, &Game::setVolume);
			game->setScene(prisonScene);
			break;
		case Menu:
			menuScene = new MainMenu();
			connect(menuScene, &BaseScene::changeScene, this, &SceneManager::setScene);
			connect(menuScene, &MainMenu::setVolume, game, &Game::setVolume);
			game->setScene(menuScene);
			menuScene->init();
			break;
		case TUTO:
			tutoScene = new TutoScene();
			connect(tutoScene, &BaseScene::changeScene, this, &SceneManager::setScene);
			connect(tutoScene, &TutoScene::setVolume, game, &Game::setVolume);
			game->setScene(tutoScene);
			break;
		case Leaderboard:
			leaderboardScene = new LeaderboardScene();
			connect(leaderboardScene, &BaseScene::changeScene, this, &SceneManager::setScene);
			game->setScene(leaderboardScene);
			break;
	}
}