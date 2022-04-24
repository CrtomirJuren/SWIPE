/**************************************
 * @file    TrafficLight.cpp
 * @brief   implementation for TrafficLight class
 * @author  Črtomir Juren
 * @version 1.0 4/10/20 
 **************************************/
#include "Application.h"
// #include "swPeriodicTimer\SwPeriodicTimer.h"
// #include "SwTimer\SwTimer.h"
// #include "SwPeriodicTimer\SwPeriodicTimer.h"

/* class contructor overloads */
Application::Application(Led &ledShortDistance, 
                         Led &ledLongDistance, 
                         PushButton &button, 
                         DistanceButtonHCSR04 &distButton,
                         BuzzerNonBlocking &buzzerNonBlocking,
                         SwPeriodicTimer &tick100ms,
                         LiquidCrystal_PCF8574 *lcd,
                         SwTimer &timer,
                         LedBlinker &ledBlinkerAlarm){    

    this->ledShortDistance = ledShortDistance;
    this->ledLongDistance = ledLongDistance;
    this->ledBlinkerAlarm = ledBlinkerAlarm;

    this->buttonConfig = button;
    this->distButton = distButton;
    this->buzzerNonBlocking = buzzerNonBlocking;
    this->tick100ms = tick100ms;
    this->lcd = lcd;
    this->timer = timer;
} 

/* initialization method, default */
void Application::init(){

    // init indicators
    ledShortDistance.init();
    ledLongDistance.init();
    ledBlinkerAlarm.init();

    // init sensors
    buttonConfig.init();
    distButton.initialize(10, 20, 5, 5);

    buzzerNonBlocking.init();

    initializeLcd();

    /* init statemachine */
    state = STATE_IDLE;
    isEntering = true;

    /* after init end, wait for safety*/
    // delay(100);

    // start periodic timers
    tick100ms.start();
}

/* returns true if error */
void Application::initializeLcd(){
    // lcd variables
    // int show = -1;
    int error;// lcd variables

    /* check if LCD connected */
    // See http://playground.arduino.cc/Main/I2cScanner how to test for a I2C device.
    Wire.begin();
    Wire.beginTransmission(0x27);
    error = Wire.endTransmission();
    Serial.print("Error: ");
    Serial.print(error);

    if (error == 0) {
        Serial.println(": LCD found.");
        // show = 0;
        lcd->begin(16, 2); // initialize the lcd
        // turn on backlight
        lcd->setBacklight(255);
        // go to first character
        lcd->home();
        // clear display
        lcd->clear();
        // print to display
        lcd->print("   SWIPE v1.00   ");
    } 
    else {
        Serial.println(": LCD not found.");
    } // if

    // return error;
}

// void Application::setSerial(Stream *streamObject){
//     // save pointer to Serial object
//     _streamRef = streamObject;
// }

// void Application::sendText(char *someText)
// {
//     _streamRef->println(someText);
// }

// void Application::updateLedsBrightness(){
//     unsigned int percent = potentiometer.readPercentage();
  
//     set led dimming value  
//       ledGreen.setBrightness(percent);
//       ledYellow.setBrightness(percent);
//       ledRed.setBrightness(percent);
// }

// void Application::fadeLeds(){
    // change the brightness for next time through the loop:
    //   brightness = brightness + fadeAmount;

    // reverse the direction of the fading at the ends of the fade:
    //   if (brightness <= 0 || brightness >= 255) {
    // fadeAmount = -fadeAmount;
    //   }
// }


/* run */ 
void Application::update(){
    /* update classes here */
    distButton.update();         // executes every 100ms
    buzzerNonBlocking.update();  // executes every 100ms
    tick100ms.update();          // executes every 100ms
    ledBlinkerAlarm.update();

    if(distButton.isTransitionToShort()){ledShortDistance.on();}
    if(distButton.isTransitionFromShort()){ledShortDistance.off();}
    if(distButton.isTransitionToLong()){ledLongDistance.on();}
    if(distButton.isTransitionFromLong()){ledLongDistance.off();}    

    if (tick100ms.isElapsed){
        StateMachine();
    }
}

void Application::StateMachine(){
    timeNowSM = millis();

    /* statemachine */
    switch(state){
        /*******************************/
        case STATE_IDLE:{
            /* entering state */
            if (isEntering){
                isEntering = false;
                timerStart = millis();
                Debug.print(DBG_DEBUG, "app|DEBUG|IDLE: State Enter");
                secCounter = 0;
                // lcd for this state
                lcd->clear();
                lcd->setCursor(0, 0);
                lcd->print("*** IDLE ***");
                lcd->setCursor(0, 1);
                lcd->print("*** second line.");
            }

            /* running state */

            /* transition to CONFIG state */
            if (distButton.isTrigger(TRIG_SHORT_TO_LONG)){
                isExiting = true;
                // sendText("IDLE: TRIG_SHORT_TO_LONG");
                Debug.print(DBG_DEBUG, "app|DEBUG|IDLE: TRIG_SHORT_TO_LONG");
                // next state                
                state = STATE_CONFIG;
            }

            /* transition to RUNNING state is with 5 seconds of short distance */
            /* TRANSITION TO RUN STATE IS POSSIBLE ONLY OF TIME IS SET */
            if (distButton.isPressedShort()){
                secCounter++;             
                if (secCounter > 40){
                    isExiting = true;
                    Debug.print(DBG_DEBUG, "app|DEBUG|IDLE: SHORT PRESSED FOR 4s");          
                    // next state
                    if(durationMs != 0 ){
                        state = STATE_RUN;                        
                    }
                    else{
                        Debug.print(DBG_DEBUG, "app|DEBUG|IDLE: durationMs is NOT SET");   
                        secCounter = 0;
                    }
                }
            }
            else{
                secCounter = 0;
            }         

            /* exiting state */
            if (isExiting){
                isExiting = false;
                isEntering = true;
            }
            break;
        }
        /*******************************/
        case STATE_CONFIG:{
            /* entering */
            if (isEntering){
                isEntering = false;
                timerStart = millis();
                Debug.print(DBG_DEBUG, "app|DEBUG|CONFIG: State Enter");
                lcd->clear();
                lcd->setCursor(0, 0);
                lcd->print("*** CONFIGURE ***");
                lcd->setCursor(0, 1);
                lcd->print("*** second line.");
            }

            /* running state */
            // here durationMs is set
            durationMs = 10000;

            /* STATE TIMEOUT */
            if (timeNowSM - timerStart > StateTimeout){
                isExiting = true;
                Debug.print(DBG_DEBUG, "app|DEBUG|CONFIG: Timeout exit");
            }

            /* transition to CONFIG state */
            if (distButton.isTrigger(TRIG_LONG_TO_SHORT)){
                isExiting = true;
                Debug.print(DBG_DEBUG, "app|DEBUG|CONFIG: TRIG_LONG_TO_SHORT");          
            }

            /* exiting */
            if (isExiting){
                state = STATE_IDLE;
                isExiting = false;
                isEntering = true;
            }
            break;
        }
        /*******************************/
        case STATE_RUN: {
            /* entering state */
            if (isEntering){
                isEntering = false;
                timerStart = millis();
                Debug.print(DBG_DEBUG, "app|DEBUG|RUNNING: State Enter");
                secCounter = 0;

                lcd->clear();
                lcd->setCursor(0, 0);
                lcd->print("*** RUNNING ***");
                // lcd->setCursor(0, 1);
                // lcd->print("*** second line.");

                // start stopwatch
                timer.start();
            }
            
            // TODO: update only every 1 seconds
            /* update stopwatch on LCD */
            elapsedMs = timer.elapsed();
            remainingMs = durationMs - elapsedMs;
            lcd->clear();
            lcd->setCursor(0, 1);
            lcd->print(remainingMs);

            /* running */
            if (elapsedMs >= durationMs){
                // next state
                state = STATE_ALARM;
                isExiting = true;
            }

            /* transition to IDLE state is with 3 seconds of short distance */
            /* transition to RUNNING state is with 5 seconds of short distance */
            if (distButton.isPressedShort()){
                // if button is pressed every 100ms increment counter
                secCounter++;             
                // Serial.println(PressCounter);  
                if (secCounter > 40){
                    isExiting = true;
                    Debug.print(DBG_DEBUG, "app|DEBUG|RUN: SHORT PRESSED FOR 4s");          
                    // next state
                    state = STATE_IDLE;
                }
            }
            else{
                secCounter = 0;
            }

            /* exiting state */
            if (isExiting){
                // stop stopwatch
                timer.stop();
               
                isExiting = false;
                isEntering = true;
            }
            break;
        }
        /*******************************/
        case STATE_ALARM: {
            /* entering state */
            if (isEntering){
                isEntering = false;
                timerStart = millis();
                Debug.print(DBG_DEBUG, "app|DEBUG|ALARM: State Enter");
                // turn on buzzer alarm
                buzzerNonBlocking.enable();
                // start blinking
                ledBlinkerAlarm.enable();

                secCounter = 0;
                lcd->clear();
                lcd->setCursor(0, 0);
                lcd->print("*** ALARM ***");
                lcd->setCursor(0, 1);
                lcd->print("SHORT 2s STOP");
            }
            
            /* running */
            if (timeNowSM - timerStart > StateDuration){
                Debug.print(DBG_DEBUG, "app|DEBUG|ALARM: State Exit");    
                isExiting = true;
            }

            /* transition to IDLE state is with 2 seconds of short distance */
            if (distButton.isPressedShort()){
                secCounter++;             
                if (secCounter > 20){
                    isExiting = true;
                    Debug.print(DBG_DEBUG, "app|DEBUG|ALARM: SHORT PRESSED FOR 2s");          
                    // next state
                    state = STATE_IDLE;
                }
            }
            else{
                secCounter = 0;
            }

            /* exiting state */
            if (isExiting){
                // stop buzzing
                buzzerNonBlocking.disable();
                // stop blinking
                ledBlinkerAlarm.disable();

                isExiting = false;
                isEntering = true;
                // next state
                state = STATE_IDLE;
            }
            break;
        }
        default: {
            Debug.print(DBG_DEBUG, "app|DEBUG|State not defined");
            break;
        }
    }
}

// void Application::allLedsOn(){
//     ledGreen.on();
//     ledYellow.on();
//     ledRed.on();
// }

// void Application::setGreen(){
//     ledGreen.on();
//     ledYellow.off();
//     ledRed.off();
// }

// void Application::setYellow(){
//     ledGreen.off();
//     ledYellow.on();
//     ledRed.off();
// }

// void Application::setRed(){
//     ledGreen.off();
//     ledYellow.off();
//     ledRed.on();
// }

