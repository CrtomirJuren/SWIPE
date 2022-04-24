// #include "LedBlinker\Led\Led.h"
#include "Led.h"

Led led(4);

void setup() {
  led.init();
}

void loop() {
    delay(500);
    led.on();
    delay(500);
    led.off();
}
