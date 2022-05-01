// libraries
#include <Arduino.h>
#include <HCSR04.h>
#include <Arduino_DebugUtils.h>
#include <Smoothed.h> 	// Include the library
#include "DistanceButton/DistanceButton.h"

DistanceButton distButton(12, 13); //address of variable v being passed

void setup () {
    Serial.begin(9600);  // We initialize serial connection so that we could print values from sensor.
    // Debug.setDebugLevel(DBG_VERBOSE);
    Debug.setDebugLevel(DBG_WARNING);
    Debug.timestampOn();
    
    distButton.initialize(10, 20, 5, 5);
    
    Debug.print(DBG_INFO, "main|DEBUG|setup done");
}

void loop () {
    // run this to update every 100ms
    distButton.update();

    // main functionality
    distButton.isPressed();
    distButton.isPressedShort();
    distButton.isPressedLong();

    // check for any change
    // if(distButton.isAnyTrigger()){
    //     Debug.print(DBG_INFO, "main|DEBUG|Trigger detected");
    // }

    // // get custom trigger status
    // if(distButton.isTrigger(TRIG_SHORT_PRESS)){
    //     Debug.print(DBG_INFO, "main|DEBUG|TRIG_SHORT_PRESS Detected");
    // }

    distButton.sendToArduinoSerialPlotter();

    delay(100);
}