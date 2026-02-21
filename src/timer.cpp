#include "timer.h"

using namespace std;


Timer::Timer() {
	elapsedTime = 0.0;
	startTime = 0.0;
	active = false;
}

Timer::~Timer() {

}

Timer::activate() {
	startTime = clock();
	active = true;
}

Timer::deactivate() {
	active = false;
	return elapsedTime;
}

Timer::update() {
	if (active) {
		clock_t currentTime = clock();
		double elapsedTime = (double)(currentTime - startTime)/CLOCKS_PER_SEC;
		cout << "elapsed time: " << elapsedTime << " seconds" << endl;
	}
}
