#include <PWM.h>


/*
 Stepper Motor Control - speed control

 This program drives a unipolar or bipolar stepper motor.
 The motor is attached to digital pins 8 - 11 of the Arduino.
 A potentiometer is connected to analog input 0.

 The motor will rotate in a clockwise direction. The higher the potentiometer value,
 the faster the motor speed. Because setSpeed() sets the delay between steps,
 you may notice the motor is less responsive to changes in the sensor value at
 low speeds.

 Created 30 Nov. 2009
 Modified 28 Oct 2010
 by Tom Igoe

 */

#include <Stepper.h>

#define motorSteps 200     // change this depending on the number of steps
                           // per revolution of your motor
#define motorPin1 8
#define motorPin2 9
#define motorPin3 10
#define motorPin4 11
#define ledPin 13
int PWMA =12;                // the pin that the LED is attached to
int PWMB = 3; 
int brightness = 90;         // how bright the LED is
int32_t frequency = 100000;

// initialize of the Stepper library:
Stepper myStepper(motorSteps, motorPin1,motorPin2, motorPin3, motorPin4); 

void setup() {
  InitTimersSafe(); 
  // set the motor speed at 60 RPMS:
  myStepper.setSpeed(160);
  bool success = SetPinFrequencySafe(PWMB, frequency);
  // Initialize the Serial port:
  Serial.begin(9600);
  // set up the LED pin:
  pinMode(ledPin, OUTPUT);
  // blink the LED:
  blink(3);
}

void loop() {
  // Step forward 100 steps:  
  pwmWrite(PWMA, brightness);
  Serial.println("Forward");
  myStepper.step(200);
  delay(500);
  // Step backward 100 steps:
  Serial.println("Backward");
  myStepper.step(-200);
  delay(500); 

}
// Blink the reset LED:
void blink(int howManyTimes) {
  int i;
  for (i=0; i< howManyTimes; i++) {
    digitalWrite(ledPin, HIGH);
    delay(200);
    digitalWrite(ledPin, LOW);
    delay(200);
  }
}

