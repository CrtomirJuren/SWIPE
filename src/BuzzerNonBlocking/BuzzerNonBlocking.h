/**************************************
 * @file    BuzzerNonBlocking.h
 * @brief   interface for BuzzerNonBlocking class
 * @author  Črtomir Juren
 * @version 1.0 4/10/20 
 **************************************/

#ifndef BUZZERNONBLOCKING_H
#define BUZZERNONBLOCKING_H

# include <Arduino.h>
# include "Buzzer\Buzzer.h"

class BuzzerNonBlocking
{
private:
    /* private attributes */
    Buzzer buzzer; // reference to the object

    bool isEnabled;
    unsigned long lastTimeBuzzed;
    unsigned long delay;      
        
    /* private methods */
    void toggleLed();

public:
    /* constructor */
    BuzzerNonBlocking() {}       // defualt, must have

    /* constructor overloads */
    BuzzerNonBlocking(Buzzer &buzzer, unsigned long delay);  // custom blink delay

    /* methods: initialization, call this in setup() */ 
    void init();
    void enable();
    void disable();
    /* methods: setter, getter*/
    // unsigned long getBlinkDelay();
    void setDelay(unsigned long delay);

    /* methods */ 
    void update();
    void setFrequency(unsigned int frequency);
};

#endif