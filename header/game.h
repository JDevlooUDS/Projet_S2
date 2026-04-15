#pragma once
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include "inputs.h"
#include <windows.h>
#include <cstdlib>
#include "player.h"
#include <chrono>
#include "timer.h"
#include <QGraphicsView>
#include "baseScene.h"
#include <QTimer>
#include <QElapsedTimer>
#include <QtOpenGLWidgets/QOpenGLWidget>
#include "audioManager.h"
#include <QKeyEvent>

using namespace std;

class Game : public QGraphicsView {
	Q_OBJECT
public:
	Game(QGraphicsScene *scene = nullptr);
	~Game();

	void setPlayerName(QString name);
	QString getPlayerName();

public slots:
	void gameLoop();
	void setVolume(int volume);

protected:
	void keyPressEvent(QKeyEvent* event) override;
	void keyReleaseEvent(QKeyEvent* event) override;
private:
	QTimer* fps = nullptr;
	QElapsedTimer elapsedTimer;
	QRect cameraPos;
	Inputs inputs;

	QString playerName;
};