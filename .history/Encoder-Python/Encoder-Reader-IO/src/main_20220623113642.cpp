#include <Arduino.h>
#include <ezButton.h>

// The aformention connections
int enA = 10;
int in1 = 8;
int in2 = 9;
ezButton limitSwitch(7);
// Motor encoder output pulses per 360 degree revolution (measured manually)F
#define ENC_COUNT_REV 2000 // pules per revolution

// Encoder output to Arduino Interrupt pin. Tracks the pulse count.
#define ENC_IN_RIGHT_A 2 // from encoder to Aruino
#define ENC_IN_RIGHT_B 3

// 1 = Forward; -1 = Reverse
int Direction_right = 1;

// Keep track of the number of right wheel pulses
volatile long right_wheel_pulse_count = 0;

// One-second interval for measurements
int interval = 1000; // ms

// Counters for milliseconds during interval
long previousMillis = 0;
long currentMillis = 0;

// Variable for RPM measuerment
float en_rpm_right = 0;
float motor_rpm_right = 0;

// Variable for angular velocity measurement
float ang_velocity_right = 0;
float ang_velocity_right_deg = 0;

const float rpm_to_radians = 0.10471975512;
const float rad_to_deg = 57.29578;

// Gear Ratio
// Small = 10, Large = 15
float gear_ratio = 15 / 10;

// Increment the number of pulses by 1
void right_wheel_pulse()
{
    right_wheel_pulse_count += Direction_right;
    /*
    // Read the value for the encoder for the right wheel
    int val = digitalRead(ENC_IN_RIGHT_B);

    Direction_right = true
    if(val == LOW) {
      Direction_right = false; // Reverse
    }
    else {
      Direction_right = true; // Forward
    }

    if (Direction_right) {
      right_wheel_pulse_count++;
    }
    else {
      right_wheel_pulse_count--;
    } */
}

////////////////////////////////////////////////
void setup()
{
    // Set used pins as output
    pinMode(enA, OUTPUT);
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    Serial.begin(9600);

    // Set pin states of the encoder
    pinMode(ENC_IN_RIGHT_A, INPUT_PULLUP);
    pinMode(ENC_IN_RIGHT_B, INPUT);

    limitSwitch.setDebounceTime(50);
    // Every time the pin goes high, this is a pulse
    attachInterrupt(digitalPinToInterrupt(ENC_IN_RIGHT_A), right_wheel_pulse, RISING);
    // Making sure I don't get my beard tangled
    // in the motor when I'm hanging over it
    // plugging it in AKA motor off
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
}

void loop()
{
    limitSwitch.loop();
    Serial.println("send 1 = up, send -1 = down"); // Prompt User for Input
    while (Serial.available() == 0)
    {
        // Wait for User to Input Data
    }
    float numMotorRotation = Serial.parseInt(); // Read the data the user has input
    // analogWrite affect Duty cycle
    //  255: 100%, 127: 50%, 0: 0%
    //  analogWrite(enA, 50); // set the speed

    // proud comment, this is for limit switch
    int state = limitSwitch.getState();
    // Serial.print(state);

    if (state == 1 or numMotorRotation == 1)
    {
        // turn it on
        // Serial.println(" not touch");
        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);
        analogWrite(enA, 255);
    }
    if (numMotorRotation == -1)
    {
        // turn it on
        // Serial.println(" not touch");
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);
        analogWrite(enA, 255);
    }
    if (state == 0 or numMotorRotation == 0)
    {
        Serial.println("touch");
        digitalWrite(in1, LOW);
        digitalWrite(in2, LOW);
    }

    //  // turn it on
    //  digitalWrite(in1, LOW);
    //  digitalWrite(in2, HIGH);
    //
    //  // analogWrite(enA, 100); // a little bit faster now
    //  analogWrite(enA, 70); // a little bit faster now
    //
    //  // Come on now!
    //  digitalWrite(in1, LOW);
    //  digitalWrite(in2, LOW);
    //

    // Record the time
    currentMillis = millis();
    long delta_time = currentMillis - previousMillis;
    // If one second has passed, print the number of pulses
    if (currentMillis - previousMillis >= interval)
    {

        previousMillis = currentMillis;
        Serial.print(delta_time);

        // Calculate revolutions per minute
        // Real RPM at 10.20 RPM, pulses 284 and RPM " 8.00"
        en_rpm_right = ((float)(right_wheel_pulse_count * 60.0 / (float)ENC_COUNT_REV)) / (float)(delta_time / 1000.0);
        motor_rpm_right = en_rpm_right * (float)gear_ratio;
        ang_velocity_right = en_rpm_right * rpm_to_radians;
        ang_velocity_right_deg = ang_velocity_right * rad_to_deg;

        Serial.print(" Pulses: ");
        Serial.println(right_wheel_pulse_count);
        Serial.print(" Encoder Speed: ");
        Serial.print(en_rpm_right);
        Serial.println(" RPM");
        Serial.print(" Motor Speed: ");
        Serial.print(motor_rpm_right);
        Serial.println(" RPM");
        Serial.print(" Angular Velocity: ");
        Serial.print(ang_velocity_right);
        Serial.print(" rad per second");
        Serial.print("\t");
        Serial.print(ang_velocity_right_deg);
        Serial.println(" deg per second");
        Serial.println();

        right_wheel_pulse_count = 0;
    }
}