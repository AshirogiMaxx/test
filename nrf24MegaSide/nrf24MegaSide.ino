#include <SPI.h>
#include <RF24.h>
#include "printf.h"

RF24 radio(9,10);
byte addresses [][6] = {"1Node","2Node"};



void setup() {
  Serial.begin(115200);
  printf_begin();
  radio.begin();
  radio.setPALevel(RF24_PA_MAX);
  radio.setChannel(0x76);
  radio.openReadingPipe(0,addresses[0]);
  radio.enableDynamicPayloads();
  radio.powerUp();
  radio.printDetails();
  radio.startListening();
}

void loop() {
  /*if (radio.available())
  {
    char text[32] = {0};
    radio.read(&text, sizeof(text));
    Serial.println(text);    
  }*/
  if (radio.available())
  {
  long int e;
  radio.read(&e, sizeof(e));
  Serial.println(e);
  }
}
