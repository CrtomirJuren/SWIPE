// libraries
#include <Arduino.h>
#include <Arduino_DebugUtils.h>
#include "DistanceButton/DistanceButton.h"
#include <HCSR04.h>

#define DIST_SENSOR_TRIG_PIN 12
#define DIST_SENSOR_ECHO_PIN 13

// create HCSR ultrasonic distance sonsor
UltraSonicDistanceSensor ultraSonicDistanceSensor(DIST_SENSOR_TRIG_PIN,
                                                  DIST_SENSOR_ECHO_PIN);

// object to handle distance value logic
DistanceButton distButton(&ultraSonicDistanceSensor); 

void setup () {
    Serial.begin(115200);
    // Debug.setDebugLevel(DBG_VERBOSE);
    Debug.setDebugLevel(DBG_DEBUG);
    Debug.timestampOn();
    
    distButton.initialize(10, 20, 5, 5);
    
    Debug.print(DBG_DEBUG, "DEBUG | setup");
}

void loop () {
    /* must be called */
    distButton.update();     // runs every 100ms
       
    // single events
    if (distButton.isShortOnlyPress()) Serial.println("isShortOnlyPress()");
    if (distButton.isLongOnlyPress()) Serial.println("isLongOnlyPress()");

    if (distButton.isTransitionToLong()) Serial.println("isTransitionToLong()");
    if (distButton.isTransitionToShort()) Serial.println("isTransitionToShort()");
    
    if (distButton.isTransitionFromShort()) Serial.println("isTransitionFromShort()");
    if (distButton.isTransitionFromLong()) Serial.println("isTransitionFromLong()");

    // continuous events
    // if (distButton.isPressed()) Debug.print(DBG_INFO, "INFO | isPressed()");
    // if (distButton.isPressedShort()) Debug.print(DBG_INFO, "INFO | isPressedShort()");
    // if (distButton.isPressedLong()) Debug.print(DBG_INFO, "INFO | isPressedLong()");

    /* analog values checking */
    // distButton.sendToArduinoSerialPlotter();

    delay(100);
}