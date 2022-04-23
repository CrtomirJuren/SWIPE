#include <Arduino.h>
#include <Arduino_DebugUtils.h>
#include <Wire.h>

// #include "SwTimer\SwTimer.h"
// #include "SwTimer/SwTimer.h"
#include "SwPeriodicTimer/SwPeriodicTimer.h"

// SwTimer swCounter(timer_mode_t::COUNTER);
SwPeriodicTimer swPeriodic1000ms(1000);

char command;

void setup() {
    Serial.begin(9600);   

    Debug.setDebugLevel(DBG_DEBUG);
    Debug.timestampOn();
    Debug.print(DBG_DEBUG, "main|DEBUG|serial.begin");
}

void loop() {

  // check if data has been sent from the computer:
  if (Serial.available()) {
    // read received command
    command = Serial.read();
    Serial.println(command);

    // Debug.print(DBG_DEBUG, "elapsed");
    // Serial.println(swCounter.elapsed());
    swPeriodic1000ms.isElapsed();

    // act upon command
    switch (command)
    {
        case 'a':
            Debug.print(DBG_DEBUG, "swCounter.start()");
            swPeriodic1000ms.start();
            break;
        
        case 'b':
            Debug.print(DBG_DEBUG, "swCounter.pause()");
            // swPeriodic1000ms.pause();
            break;
        
        case 'c':
            Debug.print(DBG_DEBUG, "swCounter.resume()");
            // swCounter.resume();
            break;
        
        case 'd':
            Debug.print(DBG_DEBUG, "swCounter.stop()");
            swPeriodic1000ms.stop();
            break;

        default:
            Debug.print(DBG_DEBUG, "unknown command");
            break;
    }
  }
}