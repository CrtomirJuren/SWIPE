/* 

 */
#include "Arduino.h"
#include "SwTimer.h"

SwTimer::SwTimer(timer_mode_t mode) {

}

SwTimer::SwTimer(timer_mode_t mode, uint32_t period_ms) {

}

void SwTimer::start() {
	value = 0;
	started = millis();
	status = RUNNING;
}

void SwTimer::pause() {
	if (status == RUNNING) {
		value = value + millis() - started;
		status = PAUSED;
	}
}

void SwTimer::resume() {
	if (status == PAUSED) {
		started = millis();
		status = RUNNING;
	}
}

void SwTimer::stop() {
	if (status == RUNNING) {
	    value = millis() - started + value;
	}
	status = STOPPED;
}

uint32_t SwTimer::elapsed() {
	if (status == RUNNING) {
	
    return millis() - started + value;
	
    }
	return value;
}

status_t SwTimer::state() {
	return status;
}