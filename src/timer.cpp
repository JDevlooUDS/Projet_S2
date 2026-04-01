#include "../header/timer.h"

using namespace std;


Timer::Timer() {
	elapsedTime = 0.0;
	startTime = 0.0;
	active = false;
}

Timer::~Timer() {

}

void Timer::activate() {
	elapsedTime = 0.0;
	startTime = clock();
	active = true;
}

float Timer::deactivate() {
	active = false;
	return elapsedTime;
}

void Timer::update() {
	if (active) {
		clock_t currentTime = clock();
		elapsedTime = (double)(currentTime - startTime)/CLOCKS_PER_SEC;
	}
}

bool Timer::isActive() {
	return active;
}

double Timer::getTime() {
	return elapsedTime;
}
