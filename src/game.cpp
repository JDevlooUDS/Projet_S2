#include "../header/game.h"

Game::Game() {
	if (loadMap() == 0) isMapLoaded = true;
	if (isMapLoaded) isRunning = true;
	player.setPosition(0,2);
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
	//Valider seulement quand la touche est appuyé et non maintenue. 
	if (GetKeyState('A') & 0x8000) inputs.isLeftPressed = true;
	if (GetKeyState('D') & 0x8000) inputs.isLeftPressed = true;
	if (GetKeyState(VK_ESCAPE) & 0x8000) inputs.isEscapePressed = true;
}

void Game::update() {
	if (inputs.isEscapePressed) isRunning = false;
	if (inputs.isLeftPressed) player.move(-1,0);
	if (inputs.isRightPressed) player.move(1,0);

	int playerPosX = player.getPosition().x;
	int playerPosY = player.getPosition().y * mapLenght;

	map[playerPosY + playerPosX] = player.getSprite();
}

void Game::draw() {
	system("cls");
	for (int i = 0; i < mapSize; i++) {
		cout << map[i];
	}
}

int Game::loadMap() {
	uintmax_t size = filesystem::file_size("C:/Users/jerem/Documents/Coding/Universite/Session 2/Projet/Projet_S2/map/map.txt");
	ifstream mapFile("C:/Users/jerem/Documents/Coding/Universite/Session 2/Projet/Projet_S2/map/map.txt");
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