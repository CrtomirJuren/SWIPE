/**************************************
 * @file    DistanceButtonHCSR04.h
 * @brief   interface for DistanceButtonHCSR04 class
 * @author  Črtomir Juren
 * @version 1.0 2/11/21
 **************************************/

#ifndef DISTANCEBUTTONHCR04_H
#define DISTANCEBUTTONHCR04_H

#include <Arduino.h>
#include <Arduino_DebugUtils.h>
#include <HCSR04.h>
#include <Smoothed.h> 	// running avarage

// statemachine
enum DistBtnSM{
    SM_RELEASED,
    SM_SHORT,
    SM_LONG,
};

// triggers
enum TriggerType{
    TRIG_NONE,
    TRIG_SHORT_PRESS,
    TRIG_SHORT_RELEASE,
    TRIG_LONG_PRESS,
    TRIG_LONG_RELEASE,
    TRIG_SHORT_TO_LONG,
    TRIG_LONG_TO_SHORT,
};
  
enum Gesture{
    GESTURE_NONE, 
    GESTURE_SHORT,
    GESTURE_LONG,
};

class DistanceButtonHCSR04{
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
    DistBtnSM stateSM;
    DistBtnSM stateOldSM;
    bool isTransition;

    void gesture_calculations();
    void statemachine();

public:
    /* public vars*/
    Gesture gesture = GESTURE_NONE;
    TriggerType trigger = TRIG_NONE;

    float distanceRaw;
    float distanceAvg;

    float shortLimit;
    float longLimit;

    /* constructor */
    DistanceButtonHCSR04() {}       // contructor, defualt, must have
    DistanceButtonHCSR04(byte pinTrig, byte pinEcho);     // contructor, defualt, must have

    /* public methods */
    void initialize(float shortLimitSetting, 
                    float longLimitSetting,
                    float hysteresisStepSetting,
                    unsigned int noActivityThreshold);

    // call this in loop() every 100ms
    void update();

    bool isPressed();
    bool isPressedShort();
    bool isPressedLong();

    bool isAnyTrigger();
    bool isTrigger(TriggerType trigger);

    void sendToArduinoSerialPlotter();
};

#endif
