## TODO LIST
- timer formatting 00:00:00
- slowly make pretty lcd gui

## STUDY

- https://community.platformio.org/t/tutorial-for-creating-multi-cpp-file-arduino-project/5830
- https://www.youtube.com/watch?v=sm6QxJkWcSc

set serial
-https://www.youtube.com/watch?v=fhEermOCrMo

SPRINTF() teorija
- https://www.programmingelectronics.com/sprintf-arduino/

### Study
- using enum classes in private/public object scope
- using c++ short hand one line if else ternary operator
variable = (condition) ? expressionTrue : expressionFalse;
- statemachine with enum classes
- distance button statemachine with array history
- distance button with histeresys

### Lcd formatting and printing
'''code
    // convert time struct to string
    char buffer [18]; // a few bytes larger than your LCD line
    uint16_t rpm = 1234; // data read from instrument
    sprintf (buffer, "RPM = %4u", rpm); // send data to the buffer

    // SHOW ON LCD NEW TIME
    lcd->setCursor(0, 1);
    lcd->print(buffer);
'''