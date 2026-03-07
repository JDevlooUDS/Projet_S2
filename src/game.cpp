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
	timer.activate();
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
	if (isFinished) {
		isRunning = false;
		return;
	}
	if (timer.isActive()) timer.update();
	managePlayerMovement();
}

void Game::draw() {
	if (!isMapUpdated) return;
	system("cls");


	if (isFinished) {
		cout << "Travail termine..." << endl;
		cout << "temps (s) : " << timer.getTime() << endl;
		return;
	}

	if (isPlayerDead) {
		cout << "Joueur mort" << endl;
		return;
	}

	cout << "Temps (s) : " << timer.getTime() << endl;

	cout << "Hp : " << player.getHp() << endl;

	for (int i = 0; i < mapSize; i++) {
		cout << map[i];
	}



	
	isMapUpdated = false;
}

int Game::loadMap() {
	uintmax_t size = filesystem::file_size("map/map.txt");
	ifstream mapFile("map/map.txt");
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

	if (inputs.isEscapePressed) {
		isRunning = false;
		timer.deactivate();
	}
	if (inputs.isLeftPressed) {
		if (map[mapPosY + playerPos.x - 1] == ' ' && playerPos.x > 0) {
			player.move(-1, 0);
			isMapUpdated = true;
			updateMap();
		}
		else if (map[mapPosY + playerPos.x - 1] == 'x' && playerPos.x > 0) {
			killPlayer();
			updateMap();
		}
		else if (map[mapPosY + playerPos.x - 1] == 'w') {
			player.move(1, 0);
			timer.deactivate();
			isFinished = true;
			isMapUpdated = true;
			updateMap();
		}
	}
	if (inputs.isRightPressed) {
		if (map[mapPosY + playerPos.x + 1] == ' ' && playerPos.x < mapLenght) {
			player.move(1, 0);
			isMapUpdated = true;
			updateMap();
		}
		else if (map[mapPosY + playerPos.x + 1] == 'x' && playerPos.x < mapLenght) {
			killPlayer();
			updateMap();
		}
		else if (map[mapPosY + playerPos.x + 1] == 'w') {
			player.move(1, 0);
			isFinished = true;
			isMapUpdated = true;
			updateMap();
		}
	}

	if (!player.isJumping() && map[mapPosY + mapLenght + playerPos.x] == ' ') {
		player.fall();
		isMapUpdated = true;
		updateMap();
	}
	else if (!player.isJumping() && map[mapPosY + mapLenght + playerPos.x] == 'x') {
		killPlayer();
		updateMap();
	}

	if (inputs.isSpacePressed && map[mapPosY - mapLenght + playerPos.x] == ' ') {
		if (!player.isJumping()) {
			player.jump();
			isMapUpdated = true;
			jumpTimer = chrono::system_clock::now();
			updateMap();
		}
	}

	if (chrono::system_clock::now() - jumpTimer >= delay) {
		if (player.isJumping() && map[mapPosY + mapLenght + playerPos.x] == ' ') {
			player.fall();
			isMapUpdated = true;
		}
		else player.cancelJump();
		updateMap();
	}

	if (inputs.isDashPressed) {
		char b1 = map[mapPosY + playerPos.x - 1];
		char b2 = map[mapPosY + playerPos.x - 2];
		char b3 = map[mapPosY + playerPos.x - 3];
		char f1 = map[mapPosY + playerPos.x + 1];
		char f2 = map[mapPosY + playerPos.x + 2];
		char f3 = map[mapPosY + playerPos.x + 3];
		if (inputs.isLeftPressed || wasAKeyPressedLastFrame) {
			int move = 0;
			if (b1 == 'x' || b2 == 'x' || b3 == 'x') {
				killPlayer();
				isMapUpdated = true;
				updateMap();
			}
			else if (b1 == 'w' || b2 == 'w' || b3 == 'w') {
				isFinished = true;
				isMapUpdated = true;
			}
			else {
			
				if (b1 == '-' && playerPos.x - 1 >= 0) move = 0;
				else if (b2 == '-' && playerPos.x - 2 >= 0) move = -1;
				else if (b3 == '-' && playerPos.x - 3 >= 0) move = -2;
				else move = -3;
				player.move(move, 0);
				updateMap();
			}
		}
		else {
			int move = 0;
			if (f1 == 'x' || f2 == 'x' || f3 == 'x') {
				killPlayer();
				isMapUpdated = true;
				updateMap();
			}
			else if (f1 == 'w' || f2 == 'w' || f3 == 'w') {
				isFinished = true;
				isMapUpdated = true;
			}
			else {

				if (f1 == '-' && playerPos.x + 1 >= 0) move = 0;
				else if (f2 == '-' && playerPos.x + 2 >= 0) move = 1;
				else if (f3 == '-' && playerPos.x + 3 >= 0) move = 2;
				else move = 3;
				player.move(move, 0);
				updateMap();
			}
		}
		isMapUpdated = true;
	}
}

void Game::updateMap() {
	Position playerPos = player.getPosition();
	Position lastPlayerPos = player.getLastPosition();
	int mapLastPosY = lastPlayerPos.y * mapLenght;
	int mapPosY = playerPos.y * mapLenght;
	map[mapLastPosY + lastPlayerPos.x] = ' ';
	map[mapPosY + playerPos.x] = player.getSprite();
}

void Game::killPlayer() {
	player.damage(1);
	if (player.getHp() == 0) {
		player.deactivate();
		isPlayerDead = true;
		isRunning = false;
	}
	Position playerPos = player.getPosition();
	int mapPlayerPosy = playerPos.y * mapLenght;
	map[mapPlayerPosy + playerPos.x] = ' ';
	player.setPosition(0,2);
	isMapUpdated = true;
}