/**************************************
 * @file    Application.cpp
 * @brief   implementation for Application class
 * @author  Črtomir Juren
 * @date    5/01/22  dd/mm/yy 
 * @version 1.0
 **************************************/
#include "Application.h"

//TODO ALL serial.print to Debug.print()

/* class contructor */
Application::Application(Led &ledShortDistance, 
                         Led &ledLongDistance, 
                         PushButton &button, 
                         DistanceButton &distButton,
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

    lcdInintialize();

    // /* init statemachine */
    // state = Application::state_t::IDLE;
    // isEntering = true;

    /* at application init end, check wait for safety*/
    ledShortDistance.on();
    ledLongDistance.on();
    delay(500);
    ledShortDistance.off();
    ledLongDistance.off();

    // start periodic timers
    tick100ms.start();
}

/* returns true if error */
void Application::lcdInintialize(){
    // lcd variables
    // int show = -1;
    int error;// lcd variables

    /* check if LCD connected */
    // See http://playground.arduino.cc/Main/I2cScanner how to test for a I2C device.
    Wire.begin();
    Wire.beginTransmission(0x27);
    error = Wire.endTransmission();
    Serial.print("Error: ");
    // Debug.print("Error: ");
    Serial.print(error);

    if (error == 0) {
        // Serial.println(": LCD found.");
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
    /* clas updates */
    distButton.update();         // executes every 100ms
    buzzerNonBlocking.update();  // executes every 100ms
    tick100ms.update();          // executes every 100ms
    ledBlinkerAlarm.update();    // executes every 100ms

    /* leds to show distances */
    if(distButton.isTransitionToShort()){ledShortDistance.on();}
    if(distButton.isTransitionFromShort()){ledShortDistance.off();}
    if(distButton.isTransitionToLong()){ledLongDistance.on();}
    if(distButton.isTransitionFromLong()){ledLongDistance.off();}    

    /* statemachine update every 100ms */
    if (tick100ms.isElapsed){
        StateMachine();
    }
}

void  Application::lcdShowTime(){

    // time conversion
    // declare variables
    // int time = 0;
 
    // // obtain data from user
    // time_seconds = 9630
 
    // // using the time from ^ above, convert
    // // secs to HH:MM:SS format using division
    // // and modulus
    // lcdClock_t.h = time / 3600;
    // time = time % 3600; 
    // lcdClock_t.m = time / 60;
    // time = time % 60;
    // lcdClock_t.s = time;

    // format into string
    sprintf (lcdClock_t.buffer,
            "%02u:%02u:%02u",
            lcdClock_t.h, 
            lcdClock_t.m,
            lcdClock_t.s);

    // show time on line second and offset 4 columns
    lcd->setCursor(4, 1);
    lcd->print(lcdClock_t.buffer);
}

void Application::StateMachine(){
    timeNowSM = millis();

    /* STATEMACHINE */
    switch(state){

        /* STATE IDLE */
        case Application::state_t::IDLE :{
            /* enter */
            if (isEntering){
                isEntering = false;
                // timerStart = millis();
                Debug.print(DBG_DEBUG, "app|DEBUG|IDLE: State Enter");
                // secCounter = 0;
                // lcd for this state
                lcd->clear();
                lcd->setCursor(0, 0);
                lcd->print("*** IDLE ***");
                
                lcdShowTime();
            }

            /* run */

            /* transition to CONFIG */
            if (distButton.isState(DistanceButton::state_t::SHORT_TO_LONG)){
                Debug.print(DBG_DEBUG, "app|DEBUG|IDLE: TRIG_SHORT_TO_LONG");
                // next state                
                state = Application::state_t::CONFIG;
            }

            /* transition to RUNNING */
            if (distButton.isShortOnlyPress()){
                if(totalDurationSec != 0 ){                    
                    state = Application::state_t::RUN;             
                }
                else{
                    Debug.print(DBG_DEBUG, "app|DEBUG|IDLE: totalDurationSec is NOT SET");   
                }
            }

            /* exit */
            if (state_old != state){
                /* state cleanup */           
            }
            break;
        }

        /* STATE CONFIG */
        case Application::state_t::CONFIG :{
            /* enter */
            if (isEntering){
                isEntering = false;
                // timerStart = millis();
                Debug.print(DBG_DEBUG, "app|DEBUG|CONFIG: State Enter");
                // lcd->clear();
                lcd->setCursor(0, 0);
                lcd->print("*** CONFIGURE ***");
            }

            /* run */     
            /* transition back to IDLE state */
            if(distButton.isState(DistanceButton::state_t::LONG_TO_SHORT)){
                Debug.print(DBG_DEBUG, "app|DEBUG|CONFIG: Exiting CONFIG state");          
                state = Application::state_t::IDLE;
            }

            // DIRECTION OF INCREMENT
            if((distButton.isState(DistanceButton::state_t::SHORT_TO_LONG))){
                Debug.print(DBG_DEBUG, "app|DEBUG|CONFIG: direction increment change");
                direction *= -1;
            }

            // SINGLE INCREMENTS
            // IF TRIIGER NONE -> SHORT PRESS RELEASE DETECTED
            // INCREMENT SECONDS
            if(distButton.isShortOnlyPress()){ 
                Debug.print(DBG_DEBUG, "app|DEBUG|CONFIG: seconds increment");

                totalDurationSec = totalDurationSec + direction;

                if (totalDurationSec < 0){
                    totalDurationSec = 0;
                }     
            }

            // increment minutes
            if(distButton.isLongOnlyPress()){ 
            // if(distButton.isTrigger(TRIG_LONG_RELEASE)){
                Debug.print(DBG_DEBUG, "app|DEBUG|CONFIG: minutes increment");
                
                totalDurationSec = totalDurationSec + direction * 60;
                
                if (totalDurationSec < 0){
                    totalDurationSec = 0;
                }   
            }

            // FAST INCREMENT SECONDS

            //FAST INCREMENT MINUTES

            // SHOW ON LCD NEW TIME
            // lcd->clear();
            lcd->setCursor(0, 0);
            lcd->print("*** CONFIGURE ***");
            lcd->setCursor(0, 1);
            lcd->print(totalDurationSec);

            /* exit */
            if (state_old != state){
                /* state cleanup */
            
            }
            break;
        }

        /* STATE RUN */
        case Application::state_t::RUN :{
            /* enter */
            if (isEntering){
                isEntering = false;
                timerStart = millis();
                Debug.print(DBG_DEBUG, "app|DEBUG|RUNNING: State Enter");
                secCounter = 0;

                // lcd->clear();
                lcd->setCursor(0, 0);
                lcd->print("*** RUNNING ***");
                // lcd->setCursor(0, 1);
                // lcd->print("*** second line.");

                // start stopwatch
                // timer.start();
            }
            
            /* run */
            // TODO: update only every 1 seconds
            /* update stopwatch on LCD */
            // elapsedMs = timer.elapsed();
            // remainingMs = totalDurationSec - elapsedMs;

            // print remaining time to lcd
            // lcd->clear();
            lcd->setCursor(0, 1);
            lcd->print(remainingMs);

            if (elapsedMs >= (uint32_t) totalDurationSec){
                // next state
                state = Application::state_t::ALARM;
            }

            /* timer control */ 
            if(distButton.isShortOnlyPress()){
                if (timer.isRunning()) {
                    Debug.print(DBG_DEBUG, "app|DEBUG|PAUSING");   
                    timer.pause();
                }
                else if (timer.isPaused()) {
                    Debug.print(DBG_DEBUG, "app|DEBUG|RESUMING");   
                    timer.resume();
                }
            }

            /* transition to IDLE state */            
            if(distButton.isLongOnlyPress()){ 
                // Debug.print(DBG_DEBUG, "app|DEBUG|RUN: SHORT PRESSED - STOPPING");  
                timer.stop();        
                // next state
                state = Application::state_t::IDLE;
            }

            /* exit */
            if (state_old != state){               
                /* state cleanup */
            }
            break;
        }

        /* STATE ALARM */
        case Application::state_t::ALARM :{
            /* enter */
            if (isEntering){
                isEntering = false;
                timerStart = millis();
                Debug.print(DBG_DEBUG, "app|DEBUG|ALARM: State Enter");
                // turn on buzzer alarm
                buzzerNonBlocking.enable();
                // start blinking
                ledBlinkerAlarm.enable();

                secCounter = 0;
                // lcd->clear();
                lcd->setCursor(0, 0);
                lcd->print("*** ALARM ***");
                lcd->setCursor(0, 1);
                lcd->print("SHORT 2s STOP");
            }
            
            /* run */
            // if (timeNowSM - timerStart > StateDuration){
            //     Debug.print(DBG_DEBUG, "app|DEBUG|ALARM: State Exit");    
            //     // isExiting = true;
            //     state = Application::IDLE;
            // }

            /* transition to IDLE state is with 2 seconds of short distance */
            if(distButton.isShortOnlyPress()){
                Debug.print(DBG_DEBUG, "app|DEBUG|ALARM: SHORT PRESSED FOR 2s");          
                // next state
                state = Application::state_t::IDLE;
            }

            /* exit */
            if (state_old != state){
                /* state cleanup */

                // stop buzzing and blinking
                buzzerNonBlocking.disable();
                ledBlinkerAlarm.disable();
            }
            break;
        }
        
        /* DEFAULT */
        default: {
            Debug.print(DBG_DEBUG, "app|DEBUG|State not defined");
            break;
        }
    }
    if (state_old != state){
        isEntering = true;
    }
    state_old = state;
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

/* TRANSITION TO RUN STATE IS POSSIBLE ONLY OF TIME IS SET */
// if (distButton.isPressedShort()){
//     secCounter++;             
//     if (secCounter > 40){
//         Debug.print(DBG_DEBUG, "app|DEBUG|IDLE: SHORT PRESSED FOR 4s");          
//         // next state
//         if(totalDurationSec != 0 ){                    
//             state = Application::RUN;             
//         }
//         else{
//             Debug.print(DBG_DEBUG, "app|DEBUG|IDLE: totalDurationSec is NOT SET");   
//             secCounter = 0;
//         }
//     }
// }
// else{
//     secCounter = 0;
// } 