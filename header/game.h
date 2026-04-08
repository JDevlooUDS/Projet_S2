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
#include "prisonScene.h"
#include "baseScene.h"
#include <QTimer>
#include <QElapsedTimer>

#include "audioManager.h"

using namespace std;

class Game : public QGraphicsView {
	Q_OBJECT
public:
	Game(QGraphicsScene *scene = nullptr);
	~Game();

public slots:
	void gameLoop();
private:
	QTimer* fps = nullptr;
	QElapsedTimer elapsedTimer;
	QRect cameraPos;
};