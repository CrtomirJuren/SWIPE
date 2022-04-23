/**************************************
 * @file    BuzzerNonBlocking.cpp
 * @brief   implementation for Buzzer
 * @author  Črtomir Juren
 * @version 1.0 18/4/22 
 **************************************/
#include "BuzzerNonBlocking.h"

/* class contructor */
BuzzerNonBlocking::BuzzerNonBlocking(Buzzer &buzzer, unsigned long delay){
    this->buzzer = buzzer;
    lastTimeBuzzed = millis();
    this->delay = delay; // [ms]
    isEnabled = false;
}

/* initialization method */
void BuzzerNonBlocking::init(){
    buzzer.init();
}

void BuzzerNonBlocking::toggleLed(){
    buzzer.toggle();
}

void BuzzerNonBlocking::enable(){
    isEnabled = true;
}

void BuzzerNonBlocking::disable(){
    isEnabled = false;
    buzzer.off();
}

void BuzzerNonBlocking::setFrequency(unsigned int frequency){
    buzzer.setFrequency(frequency);
}

void BuzzerNonBlocking::update(){
    unsigned long timeNow = millis();

    if ((timeNow - lastTimeBuzzed) >= delay){
        lastTimeBuzzed = timeNow;
        
        // only beep if enabled
        if (isEnabled){
            buzzer.toggle();
        }
    }
}

void BuzzerNonBlocking::setDelay(unsigned long delay){
    this->delay = delay;    
}