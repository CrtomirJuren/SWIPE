// Buzzer with tone().
// sketch created by Črtomir Juren
#include "Buzzer\Buzzer.h"
#include "BuzzerNonBlocking.h"

#define BUZZER_PIN 5

Buzzer buzzer(BUZZER_PIN, 3000);
BuzzerNonBlocking buzzerNonBlocking(buzzer, 500);

void setup() {
    buzzerNonBlocking.init();
}

void loop() {
    buzzerNonBlocking.update();
}
