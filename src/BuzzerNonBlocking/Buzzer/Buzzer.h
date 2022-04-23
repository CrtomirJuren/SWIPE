/**************************************
 * @file    Buzzer.h
 * @brief   interface for Led class
 * @author  Črtomir Juren
 * @version 1.0 18/4/22 
 **************************************/

#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>

class Buzzer
{
private:
    /* attributes */
    byte pin;      
    bool state;
    unsigned int frequency;

    /* private methods */
    bool isOn();

public:
    /* constructor */
    Buzzer() {}       
    
    /* constructor ovverides */
    Buzzer(byte pin, unsigned int frequency); 

    /* methods */ 
    // init the pin for LED, call this in setup()
    void init();

    // power on
    void on();
    //power off
    void off();
    //power on/off, depending on previous state
    void toggle();

    // set frequency
    void setFrequency(unsigned int frequency);
};

#endif
