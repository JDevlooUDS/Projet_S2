#pragma once
#include <QGraphicsScene>
#include <qdebug.h>
#include "inputs.h"
#include <string>

enum SceneType {
	Prison
};

class BaseScene : public QGraphicsScene {
	Q_OBJECT
public:
	BaseScene();
	~BaseScene();
	virtual void updateScene(double deltaTime);
signals:
	void changeScene(SceneType sceneType);
protected:
	Inputs inputs;

private:
};