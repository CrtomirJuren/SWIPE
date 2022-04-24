#ifndef SWPERIODICTIMER_H
#define SWPERIODICTIMER_H

#include <Arduino.h>

/** Ticker status
 *
 * @param STOPPED default, ticker is stopped
 * @param RUNNING ticker is running
 *
 */

class SwPeriodicTimer {

public:
    enum status_t {
        RUNNING,
        STOPPED,
    };
	/** create a Ticker object
	 *
	 * @param callback the name of the function to call
	 * @param timer interval length in ms or us
	 * @param repeat default 0 -> endless, repeat > 0 -> number of repeats
	 * @param resolution default MICROS for tickers under 70min, use MILLIS for tickers over 70 min
	 *
	 */

	/** constructors
	 *
	 */
    // default
  SwPeriodicTimer(){}; 

    // implementation
    SwPeriodicTimer(uint32_t period_ms);
    
	/** destructor
	 *
	 */
    // default    
    ~SwPeriodicTimer(){};

	/** started the ticker
	 *
	 */
	void start();

	/**
	 * @brief pause the timer
	 * 
	 */
	// void pause();

	/**
	 * @brief resume the timer after pause
	 * 
	 */
	// void resume();

	/** stops the ticker
	 *
	 */
	void stop();

	void update();

	/**
	 * @brief give the elapsed time back
	 * 
	 * @return uint32_t eleapsed time in micro or milli seconds
	 */
	// uint32_t elapsed();

	/**
	 * @brief get the state of the timer
	 * 
	 * @return status_t 
	 */
	SwPeriodicTimer::status_t state();

	bool isElapsed;

private:
	// uint32_t started;
	uint32_t lastUpdate;
	// uint32_t value;
	status_t status;
	uint32_t period_ms;
	uint32_t now;

    // timer_mode_t mode;
};

#endif
