// Buzzer with tone().
// sketch created by Črtomir Juren
#include "Buzzer.h"

#define BUZZER_PIN 5

Buzzer buzzer(BUZZER_PIN, 3000);

void setup() {
    buzzer.init();
}

void loop() {
    buzzer.toggle();
    delay(500);
}
