#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


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
  unsigned long M;
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
  
  radio.begin();
  radio.setPALevel(RF24_PA_MAX);
  radio.setChannel(0x76);
  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1,addresses[0]);
  radio.startListening();
  radio.enableDynamicPayloads();
  radio.powerUp();
  radio.printDetails(); 
}


void loop(void)
{ 
  // we need data to sent...
  duino1.A = analogRead(A0);
  duino1.B = analogRead(A1);
  duino1.C = analogRead(A0)/102.3;
  duino1.D = analogRead(A1)/102.3;
  // end of analog reading

  // radio stuff
  if ( radio.available() )
  {
    bool done = false;
    radio.read( &duino2, sizeof(duino2) );
    radio.stopListening();
    radio.write( &duino1, sizeof(duino1) );
    radio.startListening();
  }
  // end of radio stuff

  // serial print received data 
  Serial.print("duino2.W = ");
  Serial.println(duino2.W);
  Serial.print("duino2.X = ");
  Serial.println(duino2.X);
  Serial.print("duino2.Y = ");
  Serial.println(duino2.Y);
  Serial.print("duino2.Z = ");
  Serial.println(duino2.Z);
  Serial.print("duino2.M = ");
  Serial.println(duino2.M);
  // end of serial printing
}
