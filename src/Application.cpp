/**************************************
 * @file    TrafficLight.cpp
 * @brief   implementation for TrafficLight class
 * @author  Črtomir Juren
 * @version 1.0 4/10/20 
 **************************************/
#include "Application.h"

/* class contructor overloads */
Application::Application(Led &ledShortDistance, 
                         Led &ledLongDistance, 
                         PushButton &button, 
                         DistanceButtonHCSR04 &distButton,
                         BuzzerNonBlocking &buzzerNonBlocking)
{                               
    this->ledShortDistance = ledShortDistance;
    this->ledLongDistance = ledLongDistance;
    this->buttonConfig = button;
    this->distButton = distButton;
    this->buzzerNonBlocking = buzzerNonBlocking;
    // this->potentiometer = potentiometer;
} 

/* initialization method, default */
void Application::init(){
    // init indicators
    ledShortDistance.init();
    ledLongDistance.init();

    // init sensors
    buttonConfig.init();
    distButton.initialize(10, 20, 5, 5);

    buzzerNonBlocking.init();

    /* init statemachine */
    state = STATE_IDLE;
    isEntering = true;

    /* after init end, wait for safety*/
    delay(100);
}

// void Application::setSerial(Stream *streamObject){
//     // save pointer to Serial object
//     _streamRef = streamObject;
// }

// void Application::sendText(char *someText)
// {
//     _streamRef->println(someText);
// }

// void Application::updateLedsBrightness(){
//     unsigned int percent = potentiometer.readPercentage();
  
//     set led dimming value  
//       ledGreen.setBrightness(percent);
//       ledYellow.setBrightness(percent);
//       ledRed.setBrightness(percent);
// }

// void Application::fadeLeds(){
    // change the brightness for next time through the loop:
    //   brightness = brightness + fadeAmount;

    // reverse the direction of the fading at the ends of the fade:
    //   if (brightness <= 0 || brightness >= 255) {
    // fadeAmount = -fadeAmount;
    //   }
// }

/* run */ 
void Application::update(){
    timeNowSM = millis();
    
    // unsigned long timeLastShortPress = 0;
    // unsigned long timeLastLongPress = 0;

    /* update classes here */
    // updateLedsBrightness();
    distButton.update();

    buzzerNonBlocking.update();

    // if(distButton.isPressedShort()){
    // if(distButton.isTrigger(TRIG_SHORT_PRESS)){
    if((distButton.isTrigger(TRIG_SHORT_PRESS))||(distButton.isTrigger(TRIG_LONG_TO_SHORT))){
        ledShortDistance.on();
    }
    if((distButton.isTrigger(TRIG_SHORT_RELEASE))||(distButton.isTrigger(TRIG_SHORT_TO_LONG))){
        ledShortDistance.off();
    }
    // else if(distButton.isPressedLong()){
    if((distButton.isTrigger(TRIG_LONG_PRESS))||(distButton.isTrigger(TRIG_SHORT_TO_LONG))){
    // if(distButton.isTrigger(TRIG_LONG_PRESS)){
        ledLongDistance.on();
    }
    // else if(distButton.isPressedLong()){
    if((distButton.isTrigger(TRIG_LONG_RELEASE))||(distButton.isTrigger(TRIG_LONG_TO_SHORT))){
    // if(distButton.isTrigger(TRIG_LONG_RELEASE)){
        ledLongDistance.off();
    }

    /* statemachine */
    switch(state){
        /*******************************/
        case STATE_IDLE:{
            /* entering state */
            if (isEntering){
                isEntering = false;
                timerStart = millis();
                // sendText("IDLE: State Enter");
                Debug.print(DBG_DEBUG, "app|DEBUG|IDLE: State Enter");
                secCounter = 0;
            }

            /* running state */

            
            /* transition to CONFIG state */
            if (distButton.isTrigger(TRIG_SHORT_TO_LONG)){
                isExiting = true;
                // sendText("IDLE: TRIG_SHORT_TO_LONG");
                Debug.print(DBG_DEBUG, "app|DEBUG|IDLE: TRIG_SHORT_TO_LONG");
                // next state                
                state = STATE_CONFIG;
            }

            /* transition to RUNNING state is with 5 seconds of short distance */
            if (distButton.isPressedShort()){
                if(timeNowSM - previousMillis > 100){
                    // save the last time 
                    previousMillis = timeNowSM;   
                    
                    secCounter++;             
                    if (secCounter > 40){
                        isExiting = true;
                        Debug.print(DBG_DEBUG, "app|DEBUG|IDLE: SHORT PRESSED FOR 4s");          
                        // next state
                        state = STATE_RUN;
                    }
                }
            }
            else{
                secCounter = 0;
            }         

            /* exiting state */
            if (isExiting){
                isExiting = false;
                isEntering = true;
            }
            break;
        }
        /*******************************/
        case STATE_CONFIG:{
            /* entering */
            if (isEntering){
                isEntering = false;
                timerStart = millis();
                // sendText("CONFIG: State Enter");
                Debug.print(DBG_DEBUG, "app|DEBUG|CONFIG: State Enter");
                // setGreen();
            }

            /* STATE TIMEOUT */
            if (timeNowSM - timerStart > StateTimeout){
                isExiting = true;
                Debug.print(DBG_DEBUG, "app|DEBUG|CONFIG: Timeout exit");
            }

            /* transition to CONFIG state */
            if (distButton.isTrigger(TRIG_LONG_TO_SHORT)){
                isExiting = true;
                Debug.print(DBG_DEBUG, "app|DEBUG|CONFIG: TRIG_LONG_TO_SHORT");          
            }

            /* exiting */
            if (isExiting){
                state = STATE_IDLE;
                isExiting = false;
                isEntering = true;
            }
            break;
        }
        /*******************************/
        case STATE_RUN: {
            /* entering state */
            if (isEntering){
                isEntering = false;
                timerStart = millis();
                // sendText("B: State Enter");
                Debug.print(DBG_DEBUG, "app|DEBUG|RUNNING: State Enter");
                secCounter = 0;
                // setYellow();
            }
            
            /* running */
            if (timeNowSM - timerStart > StateDuration){
                // next state
                state = STATE_ALARM;
                isExiting = true;
            }

            /* transition to IDLE state is with 3 seconds of short distance */
            /* transition to RUNNING state is with 5 seconds of short distance */
            if (distButton.isPressedShort()){
                // if button is pressed every 100ms increment counter
                if(timeNowSM - previousMillis > 100){
                    // save the last time you blinked the LED 
                    previousMillis = timeNowSM;   
                    
                    secCounter++;             
                    // Serial.println(PressCounter);  
                    if (secCounter > 40){
                        isExiting = true;
                        Debug.print(DBG_DEBUG, "app|DEBUG|RUN: SHORT PRESSED FOR 4s");          
                        // next state
                        state = STATE_IDLE;
                    }
                }
            }
            else{
                secCounter = 0;
            }

            /* exiting state */
            if (isExiting){
                isExiting = false;
                isEntering = true;
            }
            break;
        }
        /*******************************/
        case STATE_ALARM: {
            /* entering state */
            if (isEntering){
                isEntering = false;
                timerStart = millis();
                Debug.print(DBG_DEBUG, "app|DEBUG|ALARM: State Enter");
                // turn on buzzer alarm
                buzzerNonBlocking.enable();
                secCounter = 0;
                // setRed();
            }
            
            /* running */
            if (timeNowSM - timerStart > StateDuration){
                Debug.print(DBG_DEBUG, "app|DEBUG|ALARM: State Exit");    
                isExiting = true;
            }

            /* transition to IDLE state is with 3 seconds of short distance */
            /* transition to RUNNING state is with 5 seconds of short distance */
            if (distButton.isPressedShort()){
                // if button is pressed every 100ms increment counter
                if(timeNowSM - previousMillis > 1000){
                    // save the last time you blinked the LED 
                    previousMillis = timeNowSM;   
                    
                    secCounter++;             
                    // Serial.println(PressCounter);  
                    if (secCounter > 2){
                        isExiting = true;
                        Debug.print(DBG_DEBUG, "app|DEBUG|ALARM: SHORT PRESSED FOR 2s");          
                        // next state
                        state = STATE_IDLE;
                    }
                }
            }
            else{
                secCounter = 0;
            }

            /* exiting state */
            if (isExiting){
                buzzerNonBlocking.disable();
                isExiting = false;
                isEntering = true;
                // next state
                state = STATE_IDLE;
            }
            break;
        }
        /*******************************/
        // case STATE_END: {
        //     /* entering state */
        //     if (isEntering){
        //         isEntering = false;
        //         timerStart = millis();
        //         sendText("END: State Enter");
        //         // functions on enter
        //         // allLedsOff();
        //     }
            
        //     /* running */
        //     isExiting = true;
            
        //     /* exiting state */
        //     if (isExiting){
        //         isExiting = false;
        //         isEntering = true;
        //         // next state
        //         state = STATE_IDLE;
        //         sendText("END: State Exit");
        //     }
        //     break;
        // }
        /*******************************/
        default: {
            Debug.print(DBG_DEBUG, "app|DEBUG|State not defined");
        }
    }
}

// void Application::allLedsOff(){
//     ledGreen.off();
//     ledYellow.off();
//     ledRed.off();
// }

// void Application::allLedsOn(){
//     ledGreen.on();
//     ledYellow.on();
//     ledRed.on();
// }

// void Application::setGreen(){
//     ledGreen.on();
//     ledYellow.off();
//     ledRed.off();
// }

// void Application::setYellow(){
//     ledGreen.off();
//     ledYellow.on();
//     ledRed.off();
// }

// void Application::setRed(){
//     ledGreen.off();
//     ledYellow.off();
//     ledRed.on();
// }

