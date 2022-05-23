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
    enum class state_t{
        IDLE,
        CONFIG,
        RUN,
        ALARM,
        END,
    };

    struct{
        uint16_t s = 0;
        uint16_t m = 0;
        uint16_t h = 0;
        char buffer [18];
    } lcdClock_t;

    // struct{
    //     int32_t duration_ms = 0;
    //     int32_t elapsed_ms = 0;
    //     int32_t remaining_ms = 0;
    // } stopwatch_t;

    // static char formattedTime [50];
    // char lcdTimebuffer [18]; // a few bytes larger than your LCD line

    state_t state = state_t::IDLE;
    state_t state_old = state_t::IDLE;
    bool isEntering = true;

    /* stopwatch */
    int32_t totalDurationSec = 0;
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

    /* private methods */
    void StateMachine();

    void lcdInintialize();

    void lcdShowTime();

    //void allLedsOn();
    //void allLedsOff();
    //void setGreen();
    //void setYellow();
    //void setRed();

public:
    /* constructor default */
    Application() {};

    /* constructor overload */
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

    /* public methods */
    void init();          
    void update();

};

#endif