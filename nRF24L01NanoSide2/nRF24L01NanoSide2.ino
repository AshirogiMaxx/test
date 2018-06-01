#include <SPI.h>
//#include <nRF24L01.h>
#include <RF24.h>
#include "printf.h"


const int led = 0;

typedef struct{
  int A;
  int B;
  float C;
  float D;
}
A_t;

typedef struct{
  int W;
  int X;
  float Y;
  float Z;
  unsigned long A;
}
B_t;

A_t duino1;  
B_t duino2; 

RF24 radio(9, 10);
byte addresses[][6] = {"1Node","2Node"};
unsigned long e = 0;

void setup()
{
  Serial.begin(115200);
  printf_begin();
  radio.begin();
  radio.setPALevel(RF24_PA_MAX);
  radio.setChannel(0x76);
  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1,addresses[1]);
  radio.enableDynamicPayloads();
  radio.powerUp();
  radio.printDetails(); 
}

void loop(void)
{
  // we need data to sent...
  duino2.W = analogRead(A0);
  duino2.X = analogRead(A1);
  duino2.Y = analogRead(A0)/102.3;
  duino2.Z = analogRead(A1)/102.3;
  duino2.A = e;
  
  // end of analog reading

  // radio stuff
  radio.stopListening();
  radio.write( &duino2, sizeof(duino2) );
  radio.startListening();

  unsigned long started_waiting_at = millis();
  bool timeout = false;
  while (radio.available())
  radio.read( &duino1, sizeof(duino1) );

  // end of radio stuff

  // serial print received data
  Serial.print("duino1.A = ");
  Serial.println(duino1.A);
  Serial.print("duino1.B = ");
  Serial.println(duino1.B);
  Serial.print("duino1.C = ");
  Serial.println(duino1.C);
  Serial.print("duino1.D = ");
  Serial.println(duino1.D);
  Serial.print("size of the packet = ");
  Serial.println(sizeof(duino2.A));
  e++;
  // end of serial printing
}
