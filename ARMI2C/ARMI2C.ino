#include <Wire.h>

#define OWN_ADDRESS 0x08
#define LED_PIN 13

int ledBlink = 0;

void receiveEvent(int bytes)
{
    //  Read received data
    ledBlink = Wire.read();
}
void requestEvent()
{
    Wire.write(ledBlink);  
}

void setup() {
  // put your setup code here, to run once:
  pinMode (LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Wire.begin(OWN_ADDRESS);
  // Attach a function to trigger when something is received
  Wire.onReceive(receiveEvent);
  // Attach a function to trigger when something is requested
  Wire.onRequest(requestEvent);

}

void loop() {
  // put your main code here, to run repeatedly:
  if (ledBlink == 1)
  {
       digitalWrite(LED_PIN, HIGH);
       delay(250);
       digitalWrite(LED_PIN, LOW);
    
   }
   else if (ledBlink == 0)
   {
      digitalWrite(LED_PIN, LOW);
   }

}
