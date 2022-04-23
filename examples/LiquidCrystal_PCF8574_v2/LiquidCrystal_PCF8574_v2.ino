#include <LiquidCrystal_PCF8574.h>
#include <Wire.h>

LiquidCrystal_PCF8574 lcd(0x27); // set the LCD address to 0x27 for a 16 chars and 2 line display

int show = -1;

int counter = 0;
int h_print, min_print, sec_print = 0;

// t is time in seconds = millis()/1000;
/*
char * TimeToString(unsigned long t)
{
  static char str[12];

  byte hours = seconds / 3600;
  seconds %= 3600;
  byte minutes = seconds / 60;
  seconds %= 60;
  
  long h = t / 3600;
  t = t % 3600;
  int m = t / 60;
  int s = t % 60;
  sprintf(str, "%02d:%02d:%02d", h, m, s);
  return str;
}
*/

// t is time in seconds = millis()/1000;
void timeToString(char* string, unsigned long seconds)
{
  static char str[12];
    
  //unsigned long seconds = millis() / 1000;
  byte hours = seconds / 3600;
  seconds %= 3600;
  byte minutes = seconds / 60;
  seconds %= 60;
  sprintf(string, "%02d:%02d:%02d", hours, minutes, seconds);
}


void setup()
{
  int error;

  Serial.begin(9600);
  Serial.println("LCD...");

  Serial.println("Checking i2c for LCD");

  // See http://playground.arduino.cc/Main/I2cScanner how to test for a I2C device.
  Wire.begin();
  Wire.beginTransmission(0x27);
  error = Wire.endTransmission();
  Serial.print("Error: ");
  Serial.print(error);
  // ---

  if (error == 0) {
    Serial.println(": LCD found.");
    show = 0;
    lcd.begin(16, 2); // initialize the lcd

  } else {
    Serial.println(": LCD not found.");
  } // if

} // setup()

void loop()
{
  //char *s;
  //s = TimeToString(millis()/1000);
  //Serial.println(s);
  //Serial.println(counter); 

  char str[12] = "";
  unsigned long seconds = millis() / 1000;
  timeToString(str, seconds);
  Serial.println(str);

  lcd.setBacklight(128);
  lcd.home();
  lcd.clear();
  lcd.print(str);
    
  delay(10);
} // loop()

  /*
  if (show == 0) {
    lcd.setBacklight(255);
    lcd.home();
    lcd.clear();
    lcd.print("Hello LCD");
    delay(1000);

    lcd.setBacklight(0);
    delay(400);
    lcd.setBacklight(255);

  } else if (show == 1) {
    lcd.clear();
    lcd.print("Cursor On");
    lcd.cursor();

  } else if (show == 2) {
    lcd.clear();
    lcd.print("Cursor Blink");
    lcd.blink();

  } else if (show == 3) {
    lcd.clear();
    lcd.print("Cursor OFF");
    lcd.noBlink();
    lcd.noCursor();

  } else if (show == 4) {
    lcd.clear();
    lcd.print("Display Off");
    lcd.noDisplay();

  } else if (show == 5) {
    lcd.clear();
    lcd.print("Display On");
    lcd.display();

  } else if (show == 7) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("*** first line.");
    lcd.setCursor(0, 1);
    lcd.print("*** second line.");

  } else if (show == 8) {
    lcd.scrollDisplayLeft();
  } else if (show == 9) {
    lcd.scrollDisplayLeft();
  } else if (show == 10) {
    lcd.scrollDisplayLeft();
  } else if (show == 11) {
    lcd.scrollDisplayRight();

  } else if (show == 12) {
    lcd.clear();
    lcd.print("write-");

  } else if (show > 12) {
    lcd.print(show - 13);
  } // if

  delay(1400);
  show = (show + 1) % 16;
  */
