/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the Uno and
  Leonardo, it is attached to digital pin 13. If you're unsure what
  pin the on-board LED is connected to on your Arduino model, check
  the documentation at http://arduino.cc

  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
 */
const int buttonPin = 4;
const int ledPin = 2;

int ledState = HIGH;
int buttonState;
int lastButtonState = LOW;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;



// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
   // SET initial state TO THE LED
  digitalWrite(ledPin, ledState);
}

// the loop function runs over and over again forever
void loop() {
  int reading = digitalRead(buttonPin);// wait for a second

  if(reading != lastButtonState){

    lastDebounceTime = millis();
    
  }
  if((millis() - lastDebounceTime) > debounceDelay){
      if(reading != buttonState){
         buttonState = reading;
         if(buttonState == HIGH){
          ledState = !ledState;
         }
      }
  }

  // set the LED:
  digitalWrite(ledPin, ledState);

  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastButtonState = reading;

    
}

