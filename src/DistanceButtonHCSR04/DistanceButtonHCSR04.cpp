/**************************************
 * @file    DistanceButtonHCSR04.cpp
 * @brief   implementation for DistanceButtonHCSR04 class
 * @author  Črtomir Juren
 * @version 1.0 11/04/22
 **************************************/

#include "DistanceButtonHCSR04.h"

/* class contructor */
DistanceButtonHCSR04::DistanceButtonHCSR04(byte pinTrig, byte pinEcho){
    // HCSR ustrasonic distance sensor
    this->distSensor = new UltraSonicDistanceSensor(pinTrig, pinEcho); // 12, 13
    // buffer for avaraging distance
    this->buffer = new Smoothed <float>;
    this->buffer->begin(SMOOTHED_AVERAGE, 10);
}

/* initialization method */
void DistanceButtonHCSR04::initialize(float shortLimitSetting,
                                    float longLimitSetting,
                                    float hysteresisStepSetting,
                                    unsigned int noActivityThreshold){
    /**
     * Initializes class. Run in setup()
     * @param shortLimitSetting
     * @param longLimitSetting
     * @param hysteresisStepSetting
     * @param noActivityThreshold
     */

    this->shortLimitSetting = shortLimitSetting;
    this->longLimitSetting = longLimitSetting;
    this->hysteresisStepSetting = hysteresisStepSetting;
    this->noActivityThreshold = noActivityThreshold;

    stateSM = SM_RELEASED;
    stateOldSM = SM_RELEASED;

    lastTimeUpdated = millis();
    // defualt update delay
    updateDelay = 100; // [ms]
}

void DistanceButtonHCSR04::gesture_calculations(){
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
        gesture = GESTURE_SHORT;
        this->hysteresis = 0;
    }
    // GESTURE_LONG 6 < x < 20
    else if ((shortLimit < distanceAvg) && (distanceAvg < longLimit)){
        gesture = GESTURE_LONG;
        this->hysteresis = this->hysteresisStepSetting;
    }
    // NOT VALID
    else{
        gesture = GESTURE_NONE;
        this->hysteresis = 0;
    }
}

void DistanceButtonHCSR04::statemachine(){

    isTransition = (stateSM != stateOldSM);
    stateOldSM = stateSM;

    // reset trigger before statemachine logic
    trigger = TRIG_NONE;

    // /* statemachine */
    switch(stateSM){
        case SM_RELEASED:
            if (isTransition){
                Debug.print(DBG_DEBUG, "DistanceButton|DEBUG|SM_RELEASED");
            }
            // transition to next state logic
            if (isPressedShort()){
                stateSM = SM_SHORT;
                trigger = TRIG_SHORT_PRESS;
            }
            else if (isPressedLong()){
                stateSM = SM_LONG;
                trigger = TRIG_LONG_PRESS;
            }
            else{
                stateSM = SM_RELEASED; //stay in this state
                trigger = TRIG_NONE;
            }
            break;

        case SM_SHORT:
            if (isTransition){
                Debug.print(DBG_DEBUG, "DistanceButton|DEBUG|SM_SHORT");
            }

            // transition to next state logic
            if (isPressedShort()){
                stateSM = SM_SHORT;
                trigger = TRIG_NONE;
            }
            else if (isPressedLong()){
                stateSM = SM_LONG;
                trigger = TRIG_SHORT_TO_LONG;
            }
            else{
                stateSM = SM_RELEASED;
                trigger = TRIG_SHORT_RELEASE;
            }
            break;

        case SM_LONG:
            if (isTransition){
                Debug.print(DBG_DEBUG, "DistanceButton|DEBUG|SM_LONG");
            }

            // transition to next state logic
            if (isPressedShort()){
                stateSM = SM_SHORT;
                trigger = TRIG_LONG_TO_SHORT;
            }
            else if (isPressedLong()){
                stateSM = SM_LONG;
                trigger = TRIG_NONE;
            }
            else{
                stateSM = SM_RELEASED;
                trigger = TRIG_LONG_RELEASE;
            }

            break;
        // state not defined
        default:
            Debug.print(DBG_DEBUG, "DistanceButton|DEBUG|default");
    }
}

void DistanceButtonHCSR04::update(){
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

// public methods
bool DistanceButtonHCSR04::isPressedShort(){
    /**
     * @return bool if GESTURE_SHORT
     */
    return (gesture == GESTURE_SHORT);
}

bool DistanceButtonHCSR04::isPressedLong(){
    return (gesture == GESTURE_LONG);
}

bool DistanceButtonHCSR04::isTrigger(TriggerType triggerType){
    if(trigger != TRIG_NONE){
        return (trigger == triggerType);
    }
    else{
        return false;
    }
}

bool DistanceButtonHCSR04::isPressed(){
    return (isPressedShort() || isPressedLong());
}

bool DistanceButtonHCSR04::isAnyTrigger(){
    return (trigger != TRIG_NONE);
}

void DistanceButtonHCSR04::sendToArduinoSerialPlotter(){
    // print to serial plotter
    Serial.print(distanceRaw);
    Serial.print("\t"); 
    Serial.print(distanceAvg);
    Serial.print("\t"); 
    if(gesture == GESTURE_NONE) Serial.print(0);    
    if(gesture == GESTURE_SHORT) Serial.print(30);
    if(gesture == GESTURE_LONG) Serial.print(40);
    Serial.print("\t"); 
    Serial.print(shortLimit);
    Serial.print("\t"); 
    Serial.println(longLimit);
}