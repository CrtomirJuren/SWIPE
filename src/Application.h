/**************************************
 * @file    Application.h
 * @brief   interface for Application
 * @author  Črtomir Juren
 * @version 1.0 9/4/22 
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
#include "DistanceButtonHCSR04\DistanceButtonHCSR04.h"
#include "BuzzerNonBlocking\BuzzerNonBlocking.h"

#include "SwTimer\SwTimer.h"
#include "SwPeriodicTimer\SwPeriodicTimer.h"

// #TODO AppSM spremeni v enum class Application.States
/*
enum class RadioState {Off, PlaysRadio, DoesStreaming, Broken};
*/

enum AppSM{
    STATE_IDLE,
    STATE_CONFIG,
    STATE_RUN,
    STATE_ALARM,
    STATE_END,
};

class Application
{
private:
    /* private attributes */
    Led ledShortDistance;
    Led ledLongDistance;

    LedBlinker ledBlinkerAlarm;

    PushButton buttonConfig;
    DistanceButtonHCSR04 distButton; //address of variable v being passed
    BuzzerNonBlocking buzzerNonBlocking;
    
    /* sending object to function as pointer reference
       because we are using pointer to object we must change
       calling of methods
       '.' become '->' for pointers to objects instead of objects.
    */
    LiquidCrystal_PCF8574 *lcd;

    // Stream *_streamRef;
    SwPeriodicTimer tick100ms;

    // SwTimer timeoutTimer;
    SwTimer stopwatch;

    // LOCAL CLASSES
    //  NewPing *sonar;

    // CREATING sonar = new NewPing(pinTrig, pinEcho, MAX_DISTANCE);

    /* statemachine */ 
    int state = STATE_IDLE;
    bool isEntering = true;
    bool isExiting = false;

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

    unsigned long StateAStartTime;
    unsigned long StateBStartTime;
    unsigned long StateCStartTime;

    unsigned long elapsedTime;

    /* private method */


public:
    /* constructor */
    Application() {};

    /* constructor overloads */
    // pass objects with reference
    Application(Led &ledShortDistance,
                Led &ledLongDistance,
                PushButton &button,
                DistanceButtonHCSR04 &distButton,
                BuzzerNonBlocking &buzzerNonBlocking,
                SwPeriodicTimer &tick100ms,
                LiquidCrystal_PCF8574 *lcd,
                SwTimer &stopwatch,
                LedBlinker &ledBlinkerAlarm);   
    
    /* main methods */ 
    void init();          
    void update();
    void StateMachine();
    // void setSerial(Stream *streamObject);
    // void sendText(char *someText);

    /* other methods */ 
    void initializeLcd();
    //void allLedsOn();
    //void allLedsOff();
    //void setGreen();
    //void setYellow();
    //void setRed();
};

#endif