#include "../header/game.h"

Game::Game() {
	if (loadMap() == 0) isMapLoaded = true;
	if (isMapLoaded) isRunning = true;
	player.activate();
	player.setPosition(0,2);

	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}

Game::~Game() {
	if (map != nullptr) delete[] map;
}

void Game::run() {
	while (isRunning) {
		getInputs();
		update();
		draw();
	}
}

void Game::getInputs() {
	inputs.reset(); 
	if (GetAsyncKeyState('A') & 0x8000) {

		if (!wasAKeyPressedLastFrame) inputs.isLeftPressed = true;
		wasAKeyPressedLastFrame = true;
	}
	else wasAKeyPressedLastFrame = false;

	if (GetAsyncKeyState('D') & 0x8000) {
		if (!wasDKeyPressedLastFrame) inputs.isRightPressed = true;
		wasDKeyPressedLastFrame = true;
	}
	else wasDKeyPressedLastFrame = false;

	if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
		if (!wasSpaceKeyPressedLastFrame) inputs.isSpacePressed = true;
		wasSpaceKeyPressedLastFrame = true;
	}
	else wasSpaceKeyPressedLastFrame = false;

	if (GetAsyncKeyState('E') & 0x8000) {
		if (!wasDashKeyPressedLastFrame) inputs.isDashPressed = true;
		wasDashKeyPressedLastFrame = true;
	}
	else wasDashKeyPressedLastFrame = false;

	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) inputs.isEscapePressed = true;
}

void Game::update() {
	managePlayerMovement();
}

void Game::draw() {
	if (!isMapUpdated) return;
	system("cls");
	for (int i = 0; i < mapSize; i++) {
		cout << map[i];
	}
	isMapUpdated = false;
}

int Game::loadMap() {
	uintmax_t size = filesystem::file_size("C:/Users/jerem/Documents/Universite/Session2/Projet_info/Projet_S2/map/map.txt");
	ifstream mapFile("C:/Users/jerem/Documents/Universite/Session2/Projet_info/Projet_S2/map/map.txt");
	if (!mapFile.is_open()) return 1;
	
	map = new char[size];
	for (int i = 0; i < size; i++) {
		map[i] = mapFile.get();
	}
	string line;
	mapFile.clear();
	mapFile.seekg(0,ios::beg);
	while (getline(mapFile, line)) {
		mapLenght = line.length() + 1;
		mapHeight++;
	}
	mapHeight--;
	mapSize = size;
	

	mapFile.close();
	return 0;
}

void Game::managePlayerMovement() {
	Position playerPos = player.getPosition();
	Position lastPlayerPos = player.getLastPosition();
	int mapLastPosY = lastPlayerPos.y * mapLenght;
	int mapPosY = playerPos.y * mapLenght;

	if (inputs.isEscapePressed) isRunning = false;
	if (inputs.isLeftPressed) {
		if (map[mapPosY + playerPos.x - 1] == ' ' && playerPos.x > 0) {
			player.move(-1, 0);
			isMapUpdated = true;
		}
	}
	if (inputs.isRightPressed) {
		if (map[mapPosY + playerPos.x + 1] == ' ' && playerPos.x < mapLenght) {
			player.move(1, 0);
			isMapUpdated = true;
		}
	}

	if (!player.isJumping() && map[mapPosY + mapLenght + playerPos.x] == ' ') {
		player.fall();
		isMapUpdated = true;
	}

	if (inputs.isSpacePressed) {
		if (!player.isJumping()) {
			player.jump();
			isMapUpdated = true;
			jumpTimer = chrono::system_clock::now();
		}
	}

	if (chrono::system_clock::now() - jumpTimer >= delay) {
		if (player.isJumping() && map[mapPosY + mapLenght + playerPos.x] == ' ') {
			player.fall();
			isMapUpdated = true;
		}
		else player.cancelJump();

	}

	if (inputs.isDashPressed) {
		if (inputs.isLeftPressed || wasAKeyPressedLastFrame) player.move(-3, 0);
		else player.move(3, 0);
		isMapUpdated = true;
	}


	playerPos = player.getPosition();
	lastPlayerPos = player.getLastPosition();
	mapLastPosY = lastPlayerPos.y * mapLenght;
	mapPosY = playerPos.y * mapLenght;
	map[mapPosY + playerPos.x] = player.getSprite();
	map[mapLastPosY + lastPlayerPos.x] = ' ';

}