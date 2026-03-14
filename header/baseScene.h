#pragma once
#include <QGraphicsScene>

enum SceneType {
	Prison
};

class BaseScene : public QGraphicsScene {
	Q_OBJECT
public:
	BaseScene();
	~BaseScene();
	void changeScene(SceneType sceneType);
private:
};