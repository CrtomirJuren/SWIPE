/*
SENSORS
    - ultrasonic distance sensor

INDICATORS
    - if hand is at short distance -> LED short ON
    - if hand is at longt distance -> LED long ON

APPLICATION STATEMACHINE
- if you press on the pushbutton you trigger statemachine
- IDLE --> A --> B --> C --> END --> IDLE
 */

/*** OFFICIAL DOWNLOADED LIBRARIES ***/
#include <Arduino.h>
#include <LiquidCrystal_PCF8574.h>
#include <Wire.h>
#include <Arduino_DebugUtils.h>

/*** MY CUSTOM LIBRARIES ***/
#include "Application.h" //Application class, it contains all classes
#include "LedBlinker\Led\Led.h"    // no dependancy
#include "BuzzerNonBlocking\BuzzerNonBlocking.h"    // no dependancy
#include "BuzzerNonBlocking\Buzzer\Buzzer.h"        // no dependancy
#include "PushButton\PushButton.h" // no dependancy
#include "DistanceButtonHCSR04\DistanceButtonHCSR04.h"

/*** DEFINES ***/
// Led indicators
# define LED_SHORT_DISTANCE_PIN 2
# define LED_LONG_DISTANCE_PIN 3
//# define LED_RED_PIN 11
#define BUZZER_PIN 5
// button
#define BUTTON_CONFIG_PIN 4

/*** CLASS CONSTRUCTORS ***/
Led ledShortDistance(LED_SHORT_DISTANCE_PIN);
Led ledLongDistance(LED_LONG_DISTANCE_PIN);
//LedBlinker ledBlinkerGreen(ledGreen, 200);

LiquidCrystal_PCF8574 lcd(0x27); // set the LCD address to 0x27 for a 16 chars and 2 line display

PushButton buttonConfig(BUTTON_CONFIG_PIN, 50, BTN_INT_UP);

DistanceButtonHCSR04 distButton(12, 13); //address of variable v being passed

Buzzer buzzer(BUZZER_PIN, 3000);
BuzzerNonBlocking buzzerNonBlocking(buzzer, 500);

// Application class
Application application(ledShortDistance, 
                        ledLongDistance, 
                        buttonConfig,
                        distButton,
                        buzzerNonBlocking
                        );

/*** SETUP ***/
void setup() {
    /* initalize serial communication */
    Serial.begin(9600);
    // wait to connect. For native USB port only
    while (!Serial){;}

    /* */
    Debug.setDebugLevel(DBG_DEBUG);
    Debug.timestampOn();
    Debug.print(DBG_DEBUG, "main|DEBUG|serial.begin");

    /* initailize application */
    // application.setSerial(&Serial);
    application.init();
}

/*** MAIN LOOP ***/
void loop(){
    /* run applicatio
    n */
    application.update();
}