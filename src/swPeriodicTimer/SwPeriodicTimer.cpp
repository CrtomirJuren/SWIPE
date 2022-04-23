/* 

 */
#include "Arduino.h"
#include "SwPeriodicTimer.h"

SwPeriodicTimer::SwPeriodicTimer(uint32_t period_ms) {

}

void SwPeriodicTimer::start() {
	value = 0;
	started = millis();
	status = RUNNING;
}

// void SwPeriodicTimer::pause() {
// 	if (status == RUNNING) {
// 		value = value + millis() - started;
// 		status = PAUSED;
// 	}
// }

void SwPeriodicTimer::resume() {
	if (status == PAUSED) {
		started = millis();
		status = RUNNING;
	}
}

void SwPeriodicTimer::stop() {
	if (status == RUNNING) {
	    value = millis() - started + value;
	}
	status = STOPPED;
}

uint32_t SwPeriodicTimer::elapsed() {
	if (status == RUNNING) {
	
    return millis() - started + value;
	
    }
	return value;
}

status_t SwPeriodicTimer::state() {
	return status;
}