/**************************************
 * @file    DistanceButton.cpp
 * @brief   implementation for class
 * @author  Črtomir Juren
 * @version 1.0 11/04/22
 **************************************/
#include "DistanceButton.h"

/* class contructor */
DistanceButton::DistanceButton(UltraSonicDistanceSensor *distSensor){
    // HCSR ustrasonic distance sensor
    this->distSensor = distSensor; // 12, 13
    // buffer for avaraging distance
    this->buffer = new Smoothed <float>;
    this->buffer->begin(SMOOTHED_AVERAGE, 10);
}

/* initialization method */
void DistanceButton::initialize(float shortLimitSetting,
                                float longLimitSetting,
                                float hysteresisStepSetting,
                                unsigned int noActivityThreshold){
    /**
     * Initializes class. Run in setup()
     * @param shortLimitSetting
     * @param longLimitSetting
     * @param hysteresisStepSetting
     * @param noActivityThreshold
     **/

    this->shortLimitSetting = shortLimitSetting;
    this->longLimitSetting = longLimitSetting;
    this->hysteresisStepSetting = hysteresisStepSetting;
    this->noActivityThreshold = noActivityThreshold;

    state = state_t::RELEASED;
    state_old = state_t::RELEASED;

    // initialize transistions history array

    for (int i=0; i<5; i++){
        state_hist[i] = state_t::RELEASED;
    }

    isStateChange = true;

    lastTimeUpdated = millis();
    // defualt update delay
    updateDelay = 100; // [ms]
}

void DistanceButton::gesture_calculations(){
    /*
     * decide if gesture = GESTURE_NONE/GESTURE_SHORT/GESTURE_LONG
     */

    // 1.) Read raw value from sensor
    distanceRaw = this->distSensor->measureDistanceCm();

    // 2.) CHECK IF VALUE IS VALID
    // this part is needed to ignore values out of range
    this->isValid = (distanceRaw != -1.0 && distanceRaw < 40.0);

    // this->buffer->add(distanceRaw);

    // 3.) SMOOTH VALUES
    // IN RANGE
    if (this->isValid){
        // is hand already pressed
        if (this->isGestureActive){
            // add raw value to smoothing
            this->buffer->add(distanceRaw);
        }
        // is hand pressed for first time
        else{
            // initialize buffer with new distance
            for (int i = 0; i <= 10; i++) {
                this->buffer->add(distanceRaw);
            }
        }
        //  gesture is now active      
        this->noActivityCnt = 0;
        this->isGestureActive = true;
    }
    // OUT OF RANGE
    else{
        // start incrementing isPresentCounter until max is reached
        if(this->isGestureActive){
            this->noActivityCnt++;    
            // when CounterMax is reached, set isGestureActive to false
            if (this->noActivityCnt >= this->noActivityThreshold){
                // clear buffer with zeros
                for (int i = 0; i <= 10; i++) {
                    this->buffer->add(0);
                }
                this->isGestureActive = false;
            } //if
        } //if
    } //else

    // now we have good smoothed avarage
    distanceAvg = this->buffer->get();

    // histeresys
    shortLimit = this->shortLimitSetting - this->hysteresis;
    longLimit = this->longLimitSetting + this->hysteresis;

    // 4.) DECIDE ON gestureState switching with hysteresis
    // GESTURE_SHORT 0 < x < 6
    // if((0 < distanceAvg) && (distanceAvg < (this->shortLimitCm - this->hysteresis))){
    if((0 < distanceAvg) && (distanceAvg < shortLimit)){
        // gesture = GESTURE_SHORT;
        gest = gesture_t::SHORT;

        this->hysteresis = 0;
    }
    // GESTURE_LONG 6 < x < 20
    else if ((shortLimit < distanceAvg) && (distanceAvg < longLimit)){
        // gesture = GESTURE_LONG;
        gest = gesture_t::LONG;
        
        this->hysteresis = this->hysteresisStepSetting;
    }
    // NOT VALID
    else{
        // gesture = GESTURE_NONE;
        gest = gesture_t::NONE;

        this->hysteresis = 0;
    }
}

void DistanceButton::statemachine(){

    /* statemachine */
    switch (state){
        case state_t::RELEASED:
            /* state enter */
            if (isStateChange){
                Debug.print(DBG_DEBUG, "DEBUG|ENTER|RELEASED");
            }
            
            /* state run */
            if (isGestureShort()){
                state = state_t::RELEASED_TO_SHORT;
            }
            else if (isGestureLong()){
                state = state_t::RELEASED_TO_LONG;
            }

            break;

        case state_t::RELEASED_TO_SHORT:
            /* state enter */
            if (isStateChange){
                Debug.print(DBG_DEBUG, "DEBUG|ENTER|RELEASED_TO_SHORT");
            }
            
            /* state run */
            state = state_t::SHORT;
            break;

        case state_t::RELEASED_TO_LONG:
            /* state enter */
            if (isStateChange){
                Debug.print(DBG_DEBUG, "DEBUG|ENTER|RELEASED_TO_LONG");
            }
            
            /* state run */
            state = state_t::LONG;
            break;

        case state_t::SHORT:
            /* state enter */
            if (isStateChange){
                Debug.print(DBG_DEBUG, "DEBUG|ENTER|SHORT");
            }
            
            /* state run */
            if (isGestureLong()){
                state = state_t::SHORT_TO_LONG;
            }
            else if (isGestureNone()){
                state = state_t::SHORT_TO_RELEASED;
            }

            break;

        case state_t::SHORT_TO_LONG:
            /* state enter */
            if (isStateChange){
                Debug.print(DBG_DEBUG, "DEBUG|ENTER|SHORT_TO_LONG");
            }
            /* state run */
            state = state_t::LONG;
            break;

        case state_t::SHORT_TO_RELEASED:
            /* state enter */
            if (isStateChange){
                Debug.print(DBG_DEBUG, "DEBUG|ENTER|SHORT_TO_RELEASED");
            }
            /* state run */
            state = state_t::RELEASED;
            break;

        case state_t::LONG_TO_SHORT:
            /* state enter */
            if (isStateChange){
                Debug.print(DBG_DEBUG, "DEBUG|ENTER|LONG_TO_SHORT");
            }
            
            /* state run */
            state = state_t::SHORT;

            break;

        case state_t::LONG:
            /* state enter */
            if (isStateChange){
                Debug.print(DBG_DEBUG, "DEBUG|ENTER|LONG");
            }
            
            /* state run */
            if (isGestureShort()){
                state = state_t::LONG_TO_SHORT;
            }
            else if (isGestureNone()){
                state = state_t::LONG_TO_RELEASED;
            }
            break;

        case state_t::LONG_TO_RELEASED:
            /* state enter */
            if (isStateChange){
                Debug.print(DBG_DEBUG, "DEBUG|ENTER|LONG_TO_RELEASED");
            }
            /* state run */
            state = state_t::RELEASED;
            break;

        default:
            Debug.print(DBG_DEBUG, "DEBUG|ERROR|STATE NOT DEFINED");
            break;
    }
    // check if transition and update state history
    isStateChange = (state != state_old);
    state_old = state;

    // save tranistions
    if (isStateChange){
        // save state history [i,i-1,i-2,i-3,i-4]
        state_hist[4] = state_hist[3];
        state_hist[3] = state_hist[2];
        state_hist[2] = state_hist[1];
        state_hist[1] = state_hist[0];
        state_hist[0] = state;
    }

}

void DistanceButton::update(){
    /*
     * Main update loop, update every 100ms
     * 
     */
    unsigned long timeNow = millis();

    if ((timeNow - lastTimeUpdated) >= updateDelay){
        lastTimeUpdated = timeNow;
        // calculate distances and gestures
        gesture_calculations();
        // statemachine and trigger tranistions logic
        statemachine();
    }
}

/* public methods */

/* current gesture */
bool DistanceButton::isGestureShort(){
    /**
     * @return bool if GESTURE_SHORT
     */
    return (gest == gesture_t::SHORT);
}

bool DistanceButton::isGestureLong(){
    /**
     * @return bool if GESTURE_LONG
     */
    return (gest == gesture_t::LONG);
}

bool DistanceButton::isGestureNone(){
    /**
     * @return bool if GESTURE_LONG
     */
    return (gest == gesture_t::NONE);
}

/* specific history of states  */
bool DistanceButton::isShortOnlyPress(){
    // check if RELEASED -> SHORT -> RELEASED
    if (isStateChange){
        return (
            state_hist[4] == state_t::RELEASED &&
            // state_hist[3] == state_t::RELEASED_TO_SHORT &&
            state_hist[2] == state_t::SHORT &&
            // state_hist[1] == state_t::SHORT_TO_RELEASED &&
            state_hist[0] == state_t::RELEASED
        );
    }
    else{
        return false;
    }
}

bool DistanceButton::isLongOnlyPress(){
    // check if RELEASED -> LONG -> RELEASED
    if (isStateChange){
        return (
            state_hist[4] == state_t::RELEASED &&
            // state_hist[3] == state_t::RELEASED_TO_SHORT &&
            state_hist[2] == state_t::LONG &&
            // state_hist[1] == state_t::SHORT_TO_RELEASED &&
            state_hist[0] == state_t::RELEASED
        );
    }
    else{
        return false;
    }
}

/* current state */
bool DistanceButton::isPressedShort(){
    /**
     * @return bool if GESTURE_SHORT
     */
    return (state == state_t::SHORT);
}

bool DistanceButton::isPressedLong(){
    return (state == state_t::LONG);
}

bool DistanceButton::isState(state_t state){
    return (this->state == state);
}

bool DistanceButton::isTransitionToShort(){
    // return (trigger != TRIG_NONE);
    return (
        state == state_t::RELEASED_TO_SHORT ||
        state == state_t::LONG_TO_SHORT
        );
    // return (isTrigger(TRIG_SHORT_PRESS) || isTrigger(TRIG_LONG_TO_SHORT));
}

bool DistanceButton::isTransitionToLong(){
//     return ((isTrigger(TRIG_LONG_PRESS))||(isTrigger(TRIG_SHORT_TO_LONG)));
    return (
        state == state_t::RELEASED_TO_LONG ||
        state == state_t::SHORT_TO_LONG
        );
}

bool DistanceButton::isTransitionFromLong(){
    // return ((isTrigger(TRIG_LONG_RELEASE))||(isTrigger(TRIG_LONG_TO_SHORT)));
    return (
        state == state_t::LONG_TO_SHORT ||
        state == state_t::LONG_TO_RELEASED
        );
}

bool DistanceButton::isTransitionFromShort(){
    // return ((isTrigger(TRIG_SHORT_RELEASE))||(isTrigger(TRIG_SHORT_TO_LONG)));
    return (
        state == state_t::SHORT_TO_LONG ||
        state == state_t::SHORT_TO_RELEASED
        );
}

/* helper functions  */
const char* DistanceButton::stateToString(state_t s){

    static char buf [50] = "";

    /* statemachine */
    switch (s){
        case state_t::RELEASED:
            strcpy (buf, "RELEASED");
            break;
        case state_t::RELEASED_TO_SHORT:
            strcpy (buf, "RELEASED_TO_SHORT");
            break;
        case state_t::RELEASED_TO_LONG:
            strcpy (buf, "RELEASED_TO_LONG");
            break;
        case state_t::SHORT:
            strcpy (buf, "SHORT");
            break;
        case state_t::SHORT_TO_LONG:
            strcpy (buf, "SHORT_TO_LONG");
            break;
        case state_t::SHORT_TO_RELEASED:
            strcpy (buf, "SHORT_TO_RELEASED");
            break;
        case state_t::LONG:
            strcpy (buf, "LONG");
            break;
        case state_t::LONG_TO_SHORT:
            strcpy (buf, "LONG_TO_SHORT");
            break;
        case state_t::LONG_TO_RELEASED:
            strcpy (buf, "LONG_TO_RELEASED");
            break;
        default:
            strcpy (buf, "undefined");
            break;
    }
    return buf;
}

void DistanceButton::sendToArduinoSerialPlotter(){
    // print to serial plotter
    Serial.print(distanceRaw);
    Serial.print("\t"); 
    Serial.print(distanceAvg);
    Serial.print("\t"); 
    if(gest == gesture_t::NONE) Serial.print(0);    
    if(gest == gesture_t::SHORT) Serial.print(30);
    if(gest == gesture_t::LONG) Serial.print(40);
    // Serial.print("\t"); 
    Serial.print(shortLimit);
    Serial.print("\t"); 
    Serial.println(longLimit);
}