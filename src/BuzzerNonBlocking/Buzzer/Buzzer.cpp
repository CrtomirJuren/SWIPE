/**************************************
 * @file    Buzzer.cpp
 * @brief   implementation for Buzzer class
 * @author  Črtomir Juren
 * @version 1.0 18/4/22 
 **************************************/

# include "Buzzer.h"

/* class contructor */
Buzzer::Buzzer(byte pin, unsigned int frequency){
    this->pin = pin; //_pin = pin;
    this->frequency = frequency; //_pin = pin;
    state = LOW;
}

/* initialization method */
void Buzzer::init(){
    pinMode(pin, OUTPUT);
}

void Buzzer::setFrequency(unsigned int frequency){
    // convert user input 0-100[%] -> 0-255
    // brightness = map(brightness, 0, 100, 0, 255);

    // store to class attribute
    this->frequency = frequency;
}

/* on method */ 
void Buzzer::on(){
    state = HIGH;
    tone(pin, frequency);
}

/* off method */ 
void Buzzer::off(){
    state = LOW;
    noTone(pin);
}

//  abstracting, so code is written in higher language
bool Buzzer::isOn(){
    // if state is HIGH, than return true
    return (state == HIGH);
}

void Buzzer::toggle(){
    if (isOn()){
        off();
    }
    else{
        on();
    }
}
