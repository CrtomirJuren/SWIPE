/**************************************
 * @file    Application.h
 * @brief   interface for Application
 * @author  Črtomir Juren
 * @date    5/01/22  dd/mm/yy 
 * @version 1.0
 **************************************/
#ifndef APPLICATION_H
#define APPLICATION_H

#include <Arduino.h>
#include <Stream.h>
#include <Arduino_DebugUtils.h>
#include <LiquidCrystal_PCF8574.h>
#include <Wire.h>

#include "LedBlinker\Led\Led.h"    // no dependancy
#include "LedBlinker\LedBlinker.h"    // "LedBlinker\Led\Led.h" 
#include "PushButton\PushButton.h" // no dependancy
#include "SerialManager\SerialManager.h" // no dependancy
#include "DistanceButton\DistanceButton.h"
#include "BuzzerNonBlocking\BuzzerNonBlocking.h"

#include "SwTimer\SwTimer.h"
#include "SwPeriodicTimer\SwPeriodicTimer.h"

class Application
{
private:

    /* private attributes */
    Led ledShortDistance;
    Led ledLongDistance;

    LedBlinker ledBlinkerAlarm;

    PushButton buttonConfig;
    DistanceButton distButton; //address of variable v being passed
    BuzzerNonBlocking buzzerNonBlocking;
    
    /* sending object to function as pointer reference
       because we are using pointer to object we must change
       calling of methods
       '.' become '->' for pointers to objects instead of objects.
    */
    LiquidCrystal_PCF8574 *lcd;

    SwPeriodicTimer tick100ms;

    SwTimer timer;

    /* statemachine */ 
    enum state_t{
        IDLE,
        CONFIG,
        RUN,
        ALARM,
        END,
    };

    state_t state;
    state_t state_old; 
    bool isEntering = true;

    /* stopwatch */
    int32_t durationMs = 0;
    uint32_t elapsedMs = 0;
    uint32_t remainingMs = 0;
    int8_t direction = 1; // -1 or 1

    unsigned long timerStart;
    unsigned long timeLastShortPress = 0;
    unsigned long ShortPressDuration = 0;
    unsigned long secCounter = 0;

    unsigned long timeLastLongPress = 0;
    unsigned long LongPressDuration = 0;

    unsigned long previousMillis = 0; 
    unsigned long timeNowSM = 0;
    const unsigned long StateDuration = 10000;
    const unsigned long StateTimeout = 15000;

    /* private method */


public:
    /* constructor */
    Application() {};

    /* constructor overloads */
    // pass objects with reference
    Application(Led &ledShortDistance,
                Led &ledLongDistance,
                PushButton &button,
                DistanceButton &distButton,
                BuzzerNonBlocking &buzzerNonBlocking,
                SwPeriodicTimer &tick100ms,
                LiquidCrystal_PCF8574 *lcd,
                SwTimer &stopwatch,
                LedBlinker &ledBlinkerAlarm);   
    
    /* main methods */ 
    // void setSerial(Stream *streamObject);
    // void sendText(char *someText);

    void init();          
    void update();

    void StateMachine();

    /* other methods */ 
    void initializeLcd();

    //void allLedsOn();
    //void allLedsOff();
    //void setGreen();
    //void setYellow();
    //void setRed();
};

#endif