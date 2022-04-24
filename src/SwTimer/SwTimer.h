#ifndef SWTIMER_H
#define SWTIMER_H

#include <Arduino.h>

// #TODO AppSM spremeni v enum class Application.States
/*
enum class RadioState {Off, PlaysRadio, DoesStreaming, Broken};
*/

/** Ticker status
 *
 * @param STOPPED default, ticker is stopped
 * @param RUNNIBG ticker is running
 * @param PAUSED ticker is paused
 *
 */
class SwTimer {
public:
  enum status_t {
    STOPPED,
    RUNNING,
    PAUSED};
  
    enum mode_t{
        COUNTER,
        COUNTDOWN
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
	SwTimer(){}; 

    // implementation
	SwTimer(int i); 
    
	/** destructor
	 *
	 */
    // default    
    ~SwTimer(){};

	/** started the ticker
	 *
	 */
	void start();

	/**
	 * @brief pause the timer
	 * 
	 */
	void pause();

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
	// status_t state();

private:
	uint32_t started;
	uint32_t value;
	status_t status;

	uint32_t period_ms;
    // SwTimer::mode_t mode;
};

#endif
