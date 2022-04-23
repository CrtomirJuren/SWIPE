#ifndef SWPERIODICTIMER_H
#define SWPERIODICTIMER_H

#include <Arduino.h>

// #TODO AppSM spremeni v enum class Application.States
/*
enum class RadioState {Off, PlaysRadio, DoesStreaming, Broken};
*/
// enum class timer_mode_t : unsigned char{
//     COUNTER,
//     COUNTDOWN,
//     PERIODIC,
// };

/** Ticker status
 *
 * @param STOPPED default, ticker is stopped
 * @param RUNNIBG ticker is running
 * @param PAUSED ticker is paused
 *
 */
enum status_t {
	STOPPED,
	RUNNING,
	PAUSED};

class SwPeriodicTimer {

public:

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
	void resume();

	/** stops the ticker
	 *
	 */
	void stop();

	/**
	 * @brief give the elapsed time back
	 * 
	 * @return uint32_t eleapsed time in micro or milli seconds
	 */
	uint32_t elapsed();

	/**
	 * @brief get the state of the timer
	 * 
	 * @return status_t 
	 */
	status_t state();

private:
	uint32_t started;
	uint32_t value;
	status_t status;

	uint32_t period_ms;
    // timer_mode_t mode;
};

#endif
