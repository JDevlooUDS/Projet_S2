#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include "inputs.h"
#include <windows.h>
#include <cstdlib>
#include "player.h"
#include <chrono>

using namespace std;

class Game {
public:
	Game();
	~Game();
	void run();
private:
	void getInputs();
	void update();
	void draw();
	int loadMap();

	Inputs inputs;
	bool isRunning = false;
	bool isFinished = false;
	bool isMapLoaded = false;
	int mapSize = 0;
	int mapLenght = 0;
	int mapHeight = 0;
	char* map = nullptr;
	Player player;

	bool wasAKeyPressedLastFrame = false;
	bool wasDKeyPressedLastFrame = false;
	bool wasSpaceKeyPressedLastFrame = false;
	chrono::milliseconds delay = chrono::milliseconds(1000);
	chrono::system_clock::time_point jumpTimer;

};