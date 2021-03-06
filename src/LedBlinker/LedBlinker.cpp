/**************************************
 * @file    LedBlinker.cpp
 * @brief   implementation for LedBlinker class.
 *          LedBlinker Class is higher level software logic,
 *          Led Class is for low level hardware control.
 * @author  Črtomir Juren
 * @version 1.0 4/10/20 
 **************************************/
#include "LedBlinker.h"

/* class contructor */
LedBlinker::LedBlinker(Led &led){
    this->led = led;
    lastTimeBlinked = millis();
    // defualt blink delay
    blinkDelay = 500; // [ms]
    state = state_t::DISABLED;
}

/* class contructor, overload */
LedBlinker::LedBlinker(Led &led, unsigned long blinkDelay){
    this->led = led;
    lastTimeBlinked = millis();

    this->blinkDelay = blinkDelay; // [ms]
    state = state_t::DISABLED;
}

// /* initialization method */
void LedBlinker::init(){
    led.init();
}

void LedBlinker::toggleLed(){
    led.toggle();
}

void LedBlinker::setBrightness(unsigned int brightness){
    led.setBrightness(brightness);
}

void LedBlinker::update(){
    unsigned long timeNow = millis();

    if ((timeNow - lastTimeBlinked) >= blinkDelay){
        lastTimeBlinked = timeNow;
        if(state == state_t::ENABLED){
            led.toggle();
        }
        else{
           led.off();
        }
    }
}

unsigned long LedBlinker::getBlinkDelay(){
    return blinkDelay;
}

void LedBlinker::setBlinkDelay(unsigned long blinkDelay){
    this->blinkDelay = blinkDelay;    
}

void LedBlinker::enable(){
    state = state_t::ENABLED;   
}

void LedBlinker::disable(){
    state = state_t::DISABLED;      
}
