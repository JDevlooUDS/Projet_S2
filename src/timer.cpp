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
		double elapsedTime = (double)(currentTime - startTime)/CLOCKS_PER_SEC;
		cout << "elapsed time: " << elapsedTime << " seconds" << endl;
	}
}
