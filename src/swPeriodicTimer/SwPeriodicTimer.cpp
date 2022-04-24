/* 

 */
//#include "Arduino.h"
#include "SwPeriodicTimer.h"

SwPeriodicTimer::SwPeriodicTimer(uint32_t period_ms) {
    this->period_ms = period_ms;
    status == status_t::STOPPED;
}

void SwPeriodicTimer::update() {
    now = millis(); 
    isElapsed = false;

    if (status == status_t::RUNNING){
        if(now - lastUpdate > period_ms){
            // save the last time    
            lastUpdate = now;
            isElapsed = true;   
        }
    }
}

void SwPeriodicTimer::start() {
	lastUpdate = millis();
	status = status_t::RUNNING;
}

void SwPeriodicTimer::stop() {
	if (status == status_t::RUNNING) {
	    status = status_t::STOPPED;
	}
}

SwPeriodicTimer::status_t SwPeriodicTimer::state() {
	return status;
}
