/* 

 */
#include "Arduino.h"
#include "SwTimer.h"

void SwTimer::start() {
	value = 0;
	started = millis();
	status = status_t::RUNNING;
}

void SwTimer::pause() {
	if (status == status_t::RUNNING){
        value = value + millis() - started;
		status = status_t::PAUSED;
	}
}

void SwTimer::resume() {
	if (status == status_t::PAUSED) {
		started = millis();
		status = status_t::RUNNING;
	}
}

void SwTimer::stop() {
	if (status == status_t::RUNNING) {
	    value = millis() - started + value;
	    status = status_t::STOPPED;
    }
}

uint32_t SwTimer::elapsed() {
	if (status == status_t::RUNNING) {
        return (millis() - started + value);
    }
    else{
        return value;
    }
}

bool SwTimer::isPaused(){
    return (status == status_t::PAUSED);
}

bool SwTimer::isRunning(){
    return (status == status_t::RUNNING);
}
