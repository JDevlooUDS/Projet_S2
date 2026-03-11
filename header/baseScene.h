#pragma once
#include <QGraphicsScene>
class BaseScene : public QGraphicsScene {
public:
	BaseScene();
	~BaseScene();
	void changeScene(SceneType sceneType);
};