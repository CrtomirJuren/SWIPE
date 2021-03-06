#include <Arduino.h>
#include <Arduino_DebugUtils.h>

#include "Stopwatch\Stopwatch.h"


Stopwatch stopwatch; // default constructor called

char command;

void setup() {
    Serial.begin(115200);   

    Debug.setDebugLevel(DBG_DEBUG);
    Debug.timestampOn();
    Debug.print(DBG_DEBUG, "setup");
}

void loop() {

  // check if data has been sent from the computer:
  if (Serial.available()) {
    // read received command
    command = Serial.read();
    Serial.println(command);

    Debug.print(DBG_DEBUG, "elapsed");
    // Serial.println(stopwatch.elapsed());

    // act upon command
    switch (command)
    {
        case 'a':
            Debug.print(DBG_DEBUG, "start");
            stopwatch.start();
            break;
        
        case 'b':
            Debug.print(DBG_DEBUG, "pause");
            // stopwatch.pause();
            break;
        
        case 'c':
            Debug.print(DBG_DEBUG, "resume");
            // stopwatch.resume();
            break;
        
        case 'd':
            Debug.print(DBG_DEBUG, "stop");
            // stopwatch.stop();
            break;

        default:
            Debug.print(DBG_DEBUG, "unknown");
            break;
    }
  }
}
