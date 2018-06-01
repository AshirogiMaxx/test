
 /*
 Modified version of the original library Stepper and implementing PWM library To set a 100 khz frecuency 
 to control the TB6612FNG and a stepper Motor 17HS5425 
 */



#include <Stepper.h>
#include <PWM.h>


#define motorSteps 200     // change this depending on the number of steps
                           // per revolution of your motor 360/1.8 Degrees
#define motorPin1 8 // AIN1
#define motorPin2 9 // AIN2
#define motorPin3 10 // BIN1
#define motorPin4 11 // BIN2
#define ledPin 13
#define STBY 3 // SBTY
int PWMA = 12;                // the pin that the PWMA and PWMB Is conected 
int Cycle = 120;         // 
int32_t frequency = 100000;

// initialize of the Stepper library:
Stepper myStepper(motorSteps, motorPin1,motorPin2, motorPin3, motorPin4); 
int previous = 0;

void setup() {
  InitTimersSafe(); 
  // set the motor speed at 60 RPMS:
  myStepper.setSpeed(60);
  bool success = SetPinFrequencySafe(PWMA, frequency);
  // Initialize the Serial port:
  Serial.begin(9600);
  // set up the LED pin:
  pinMode(ledPin, OUTPUT);
  pinMode(STBY, OUTPUT);
  digitalWrite(STBY, LOW);
  // blink the LED:
  blink(3);
}

void loop() {
  // Step forward 100 steps:  
  int val = analogRead(0);
  pwmWrite(PWMA, Cycle);
  digitalWrite(STBY, HIGH);
  // move a number of steps equal to the change in the
  // sensor reading
  myStepper.step(val - previous);

  // remember the previous value of the sensor
  previous = val;

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

