#include <Arduino.h>
#include <Arduino_DebugUtils.h>
// #include <Wire.h>

// #include "SwTimer\SwTimer.h"
#include "SwTimer/SwTimer.h"

SwTimer swCounter(timer_mode_t::COUNTER);

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

    Debug.print(DBG_DEBUG, "elapsed");
    Serial.println(swCounter.elapsed());

    // act upon command
    switch (command)
    {
        case 'a':
            Debug.print(DBG_DEBUG, "swCounter.start()");
            swCounter.start();
            break;
        
        case 'b':
            Debug.print(DBG_DEBUG, "swCounter.pause()");
            swCounter.pause();
            break;
        
        case 'c':
            Debug.print(DBG_DEBUG, "swCounter.resume()");
            swCounter.resume();
            break;
        
        case 'd':
            Debug.print(DBG_DEBUG, "swCounter.stop()");
            swCounter.stop();
            break;

        default:
            Debug.print(DBG_DEBUG, "unknown command");
            break;
    }
  }
}