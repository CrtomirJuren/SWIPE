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

#include "LedBlinker\Led\Led.h"    // no dependancy
#include "PushButton\PushButton.h" // no dependancy
#include "SerialManager\SerialManager.h" // no dependancy
#include "DistanceButtonHCSR04\DistanceButtonHCSR04.h"
#include "BuzzerNonBlocking\BuzzerNonBlocking.h"

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

    PushButton buttonConfig;
    DistanceButtonHCSR04 distButton; //address of variable v being passed
    BuzzerNonBlocking buzzerNonBlocking;

    Stream *_streamRef;

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

    /* private method */


public:
    /* constructor */
    Application() {}

    /* constructor overloads */
    // pass objects with reference
    Application(Led &ledShortDistance,
                Led &ledLongDistance,
                PushButton &button,
                DistanceButtonHCSR04 &distButton,
                BuzzerNonBlocking &buzzerNonBlocking);   
    
    /* main methods */ 
    void init();          
    void update();

    // void setSerial(Stream *streamObject);
    // void sendText(char *someText);

    /* other methods */ 
    //void allLedsOn();
    //void allLedsOff();
    //void setGreen();
    //void setYellow();
    //void setRed();
};

#endif