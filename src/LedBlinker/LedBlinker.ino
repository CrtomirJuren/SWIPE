// #include <Arduino.h>
#include <Arduino_DebugUtils.h>

#include "Led\Led.h"
#include "LedBlinker.h"

Led led(4);
LedBlinker ledBlinker(led, 200);

char command;

void setup() {
    Serial.begin(115200);   

    Debug.setDebugLevel(DBG_DEBUG);
    Debug.timestampOn();
    Debug.print(DBG_DEBUG, "main|DEBUG|serial.begin");

    ledBlinker.init();
}

void loop() {
    ledBlinker.update();

    // check if char in buffer
    if (Serial.available()) {
        // read command
        command = Serial.read();

        // act upon command
        switch (command)
        {
            case 'e':
                Debug.print(DBG_DEBUG, "edBlinker::enable()");
                ledBlinker.enable();
                break;
            
            case 'd':
                Debug.print(DBG_DEBUG, "LedBlinker::disable()");
                ledBlinker.disable();
                break;

            default:
                Debug.print(DBG_DEBUG, "unknown command");
                break;
        }
    }
}