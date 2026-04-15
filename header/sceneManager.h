#pragma once
#include "game.h"
#include "prisonScene.h"
#include "mainMenu.h"
#include "tutoScene.h"
#include "leaderboardScene.h"

class SceneManager : public QObject{

	Q_OBJECT

public:
	SceneManager(Game* game);
	~SceneManager();

public slots:
	void setScene(SceneType sceneType);

private:
	Game* game;
	PrisonScene* prisonScene = nullptr;
	MainMenu* menuScene = nullptr;
	TutoScene* tutoScene = nullptr;
	LeaderboardScene* leaderboardScene = nullptr;
};