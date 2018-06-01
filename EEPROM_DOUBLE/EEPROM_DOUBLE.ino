#include <EEPROM.h>


template <class T> int EEPROM_writeAnything(int ee, const T& value)
{
   const byte* p = (const byte*)(const void*)&value;
   int i;
   for (i = 0; i < sizeof(value); i++)
       EEPROM.write(ee++, *p++);
   return i;
}

template <class T> int EEPROM_readAnything(int ee, T& value)
{
   byte* p = (byte*)(void*)&value;
   int i;
   for (i = 0; i < sizeof(value); i++)
       *p++ = EEPROM.read(ee++);
   return i;
}


double testInt[12] = { -12.5, -21.852459, -102.300160, 0, 2.45, 2.90, 3.10, 4 , 5.6, 7.9, 5.5, 4};
byte noElem = 12;
unsigned int baseAddr = 0;
unsigned int n = 0;

void setup() {
 Serial.begin(9600);

 // write data to eeprom 
 for (int i=0; i <= noElem-1; i++){
   n = EEPROM_writeAnything( (i*4)+baseAddr, testInt[i]);
 }

 // read data back
 for (int i=0; i <= noElem-1; i++){
   double val;
   int addr = (i*4)+baseAddr;
   n = EEPROM_readAnything( addr, val);
   Serial.println(val);
 }
}

void loop() {
}
