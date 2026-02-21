#ifndef TIMER_H
#define TIMER_H

#include <iostream>
#include <ctime>


class Timer {
public:
    Timer();
    ~Timer();

	void activate(); // start timer a 0
	float deactivate(); // arrete timer et retourne le temps ecoule
	void update(); // appeller chaque frame quand timer on
	bool isActive();
	double getTime();
private:
	double elapsedTime;
	float startTime;
	bool active;
};

#endif