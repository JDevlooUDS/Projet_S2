#pragma once
#include <QGraphicsScene>
#include <qdebug.h>
#include "inputs.h"
#include <string>

enum SceneType {
	Prison,
	Menu
};

class BaseScene : public QGraphicsScene {
	Q_OBJECT
public:
	BaseScene();
	~BaseScene();
	virtual void updateScene(double deltaTime, const Inputs& inputs);
signals:
	void changeScene(SceneType sceneType);
protected:

private:
};