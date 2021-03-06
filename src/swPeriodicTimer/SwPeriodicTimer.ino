#include <Arduino.h>
#include <Arduino_DebugUtils.h>
#include "SwPeriodicTimer.h"

// SwTimer swCounter(timer_mode_t::COUNTER);
SwPeriodicTimer period500ms(500);
SwPeriodicTimer period1000ms(1000);
SwPeriodicTimer period3000ms(3000);

// char command;
uint8_t period500msCnt;
uint8_t period1000msCnt;
uint8_t period3000msCnt;

void setup() {
    Serial.begin(115200);   

    Debug.setDebugLevel(DBG_DEBUG);
    Debug.timestampOn();
    Debug.print(DBG_DEBUG, "serial initialized");

    period500ms.start();
    period1000ms.start();
    period3000ms.start();
}

void loop() {
    period500ms.update();
    period1000ms.update();
    period3000ms.update();

    if(period500ms.isElapsed){
        Debug.print(DBG_DEBUG, "_|period500ms|isElapsed");  
        period500msCnt++;

        if (period500msCnt >= 10){
            period500ms.stop();
        }
    }
    
    if(period1000ms.isElapsed){
        Debug.print(DBG_DEBUG, "__|period1000ms|isElapsed");        
        period1000msCnt++;

        if (period1000msCnt >= 10){
            period1000ms.stop();
        }
    }
    
    if(period3000ms.isElapsed){
        Debug.print(DBG_DEBUG, "___|period3000ms|isElapsed");        
        period3000msCnt++;

        if (period3000msCnt >= 10){
            period3000ms.stop();
        }
    }
}
