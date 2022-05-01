/**************************************
 * @file    DistanceButton.h
 * @brief   interface for class
 * @author  Črtomir Juren
 * @version 1.0 2/11/21
 **************************************/

#ifndef DISTANCEBUTTON_H
#define DISTANCEBUTTON_H

#include <Arduino.h>
#include <Arduino_DebugUtils.h>
#include <HCSR04.h>
#include <Smoothed.h> 	// running avarage

class DistanceButton{
private:
    /* attributes */
    UltraSonicDistanceSensor *distSensor;
    Smoothed <float> *buffer;

    unsigned long lastTimeUpdated;
    unsigned long updateDelay;    

    bool isValid = false;   
    bool isGestureActive = false;

    unsigned int noActivityCnt = 0;
    unsigned int noActivityThreshold;

    // limits for short/long control
    float shortLimitSetting;
    float longLimitSetting;
    float hysteresisStepSetting;
    float hysteresis = 0;

    /* statemachine */
    bool isStateChange;

    void gesture_calculations();
    void statemachine();

public:
    // statemachine
    enum class state_t{
        RELEASED,
        RELEASED_TO_SHORT,
        RELEASED_TO_LONG,

        SHORT,
        SHORT_TO_LONG,
        SHORT_TO_RELEASED,

        LONG,
        LONG_TO_SHORT,
        LONG_TO_RELEASED,
    };

    enum class gesture_t {
        NONE,
        SHORT,
        LONG,
    };

    // state and its history
    state_t state, state_old, state_hist[5];

    gesture_t gest = gesture_t::NONE;

    float distanceRaw;
    float distanceAvg;

    float shortLimit;
    float longLimit;

    /* constructor */
    DistanceButton() {}       // contructor, defualt, must have
    DistanceButton(UltraSonicDistanceSensor *distSensor);     // contructor, defualt, must have

    /* public methods */
    void initialize(float shortLimitSetting, 
                    float longLimitSetting,
                    float hysteresisStepSetting,
                    unsigned int noActivityThreshold);

    // call this in loop() every 100ms
    void update();

    // new
    bool isGestureShort();
    bool isGestureLong();
    bool isGestureNone();

    bool isShortOnlyPress();
    bool isLongOnlyPress();

    bool isState(state_t state);

    /*char* const is an immutable pointer 
    (it cannot point to any other location)
    but the contents of location at which
    it points are mutable.
    */
    const char* stateToString(state_t state);

    // old
    bool isPressed();
    bool isPressedShort();
    bool isPressedLong();

    bool isTransitionToShort();
    bool isTransitionFromShort();

    bool isTransitionToLong();
    bool isTransitionFromLong();

    void sendToArduinoSerialPlotter();
};

#endif
