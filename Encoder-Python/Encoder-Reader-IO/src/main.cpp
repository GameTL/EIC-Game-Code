#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include <Encoder.h>

// init display with pinout
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Encoder
//! avoid using pins with LEDs attached
Encoder myEnc(9, 8); // myEnc(pinA, pinB); // for anticlockwise, reverse

// Calulation & Constants
int prevT; // Track previous time
int currT; // Track current time
int x;
long oldPosition = 0;
long newPosition = 0;
float deltaTime;
float RESOLUTION = 2000;
float velocity;
float angularVelocity;
float PULSE_PER_REV = RESOLUTION * 4; // pulse per revolution: 8000
float TIME_INTERVAL = 20;             // in millisecond
float DIVIDER = (TIME_INTERVAL / 1000);

// input from python / serial
String input;

void displayInit()
{
    lcd.clear();
    lcd.begin(16, 2);
    lcd.print("DISPLAY");
    lcd.setCursor(0, 1);
    lcd.print("IT WORKS!");
    delay(200);
}
void checkOutToDisplay()
{
    if (Serial.available() > 0)
    {
        // read the incoming byte:
        lcd.clear();
        input = Serial.readStringUntil('\n');
        lcd.setCursor(0, 0);
        lcd.print(input);
    }
}
void serialInputToDisplay()
{
    if (Serial.available() > 0) // if there's an input
    {
        // read the incoming byte:
        Serial.println(deltaTime);
        lcd.clear();
        input = Serial.readStringUntil('\n');
        lcd.setCursor(0, 0);
        lcd.print(input);
    }
}
void setup()
{
    Serial.begin(115200); // Change the default platform.IO & Serial Monitor
    prevT = millis();
    currT = millis();
    displayInit();
}
int counter = 0;
void loop()
{
    checkOutToDisplay();
    currT = millis();
    newPosition = myEnc.read();
    deltaTime = currT - prevT;
    if (deltaTime >= TIME_INTERVAL)
    {
        serialInputToDisplay();
        counter = counter + 1;
        angularVelocity = (float)(((newPosition - oldPosition) * (2)) / (DIVIDER * PULSE_PER_REV));
        oldPosition = newPosition;
        Serial.println(counter);       // How many loops
        Serial.println(deltaTime);     // Check time interval
        Serial.print(angularVelocity); // Check angular velocity in pi rad/s
        Serial.print("PI RAD/s");
        Serial.println();
        prevT = currT;
    }

    // send data only when you receive data:

    // delay(1000);
}
