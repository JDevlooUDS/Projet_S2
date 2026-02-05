#include "../header/game.h"

Game::Game() {
	
}

Game::~Game() {}

void Game::run() {
	while (isRunning) {
		getInputs();
		update();
		draw();
	}
}

void Game::getInputs() {

}

void Game::update() {

}

void Game::draw() {
	
}