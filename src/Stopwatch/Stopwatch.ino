#include <Arduino.h>
#include <Arduino_DebugUtils.h>
// #include <Wire.h>

// #include "SwTimer\SwTimer.h"
#include "Stopwatch.h"

Stopwatch stopwatch();

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
    Serial.println(stopwatch.elapsed());

    // act upon command
    switch (command)
    {
        case 'a':
            Debug.print(DBG_DEBUG, "stopwatch.start()");
            stopwatch.start();
            break;
        
        case 'b':
            Debug.print(DBG_DEBUG, "stopwatch.pause()");
            stopwatch.pause();
            break;
        
        case 'c':
            Debug.print(DBG_DEBUG, "stopwatch.resume()");
            stopwatch.resume();
            break;
        
        case 'd':
            Debug.print(DBG_DEBUG, "stopwatch.stop()");
            stopwatch.stop();
            break;

        default:
            Debug.print(DBG_DEBUG, "unknown command");
            break;
    }
  }
}
