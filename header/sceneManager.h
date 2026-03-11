#pragma once
#include "game.h"
#include "prisonScene.h"
#include <QObject>

enum SceneType {
	Prison
};

class SceneManager : public QObject{

	Q_OBJECT

public:
	SceneManager(Game* game);
	~SceneManager();

public slots:
	void setScene(SceneType sceneType);

private:
	Game* game = nullptr;
	PrisonScene prisonScene;
};