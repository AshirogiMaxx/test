#include <SPI.h>
//#include <nRF24L01.h>
#include <RF24.h>
#include "printf.h"


RF24 radio(9, 10);
byte addresses[][6] = {"1Node","2Node"};
int e = 0;

void setup()
{
  Serial.begin(115200);
  printf_begin();
  radio.begin();
  radio.setPALevel(RF24_PA_MAX);
  radio.setChannel(0x76);
  radio.openWritingPipe(addresses[0]);
  radio.enableDynamicPayloads();
  radio.powerUp();
  radio.printDetails(); 
}

void loop()
{
  
  
  /*const char text[] = "Hello World";
  radio.write(&text, sizeof(text));
  
  delay(1000);*/
  radio.stopListening();
  radio.write(&e, sizeof(e));
  Serial.println(sizeof(e));
  delay(10);
  e++;
}
