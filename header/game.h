#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include "inputs.h"
#include <windows.h>
#pragma once
#include <cstdlib>
#include "player.h"
#include <chrono>
#include "timer.h"
#include <QGraphicsView>
#include <QGraphicsScene>

using namespace std;

class Game : public QGraphicsView {
public:
	Game(QGraphicsScene *scene = nullptr);
	~Game();
	void run();
private:
	void getInputs();
	void update();
	void draw();
	int loadMap();
	void managePlayerMovement();
	void killPlayer();
	void updateMap();

	Inputs inputs;
	bool isRunning = false;
	bool isFinished = false;
	bool isMapLoaded = false;
	bool isPlayerDead = false;
	int mapSize = 0;
	int mapLenght = 0;
	int mapHeight = 0;
	char* map = nullptr;
	Player player;

	bool wasAKeyPressedLastFrame = false;
	bool wasDKeyPressedLastFrame = false;
	bool wasSpaceKeyPressedLastFrame = false;
	bool wasDashKeyPressedLastFrame = false;
	bool isMapUpdated = true;

	Timer timer;

	chrono::milliseconds delay = chrono::milliseconds(1000);
	chrono::system_clock::time_point jumpTimer;

};