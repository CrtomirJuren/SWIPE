/* 

 */
#include <Arduino.h>
#include "Stopwatch.h"

void Stopwatch::setDurationMs(uint32_t duration) {
	value = duration;
	// started = millis();
	// status = Stopwatch::status_t::RUNNING;
}

void Stopwatch::start() {
	value = 0;
	started = millis();
	status = Stopwatch::status_t::RUNNING;
}

void Stopwatch::pause() {
	if (status == Stopwatch::status_t::RUNNING){
        value = value + millis() - started;
		status = Stopwatch::status_t::PAUSED;
	}
}

void Stopwatch::resume() {
	if (status == Stopwatch::status_t::PAUSED) {
		started = millis();
		status = Stopwatch::status_t::RUNNING;
	}
}

void Stopwatch::stop() {
	if (status == Stopwatch::status_t::RUNNING) {
	    value = millis() - started + value;
	    status = Stopwatch::status_t::STOPPED;
    }
}

uint32_t Stopwatch::elapsed() {
	if (status == Stopwatch::status_t::RUNNING) {
        return (millis() - started + value);
    }
    else{
        return value;
    }
}

bool Stopwatch::isPaused(){
    return (status == Stopwatch::status_t::PAUSED);
}

bool Stopwatch::isRunning(){
    return (status == Stopwatch::status_t::RUNNING);
}
