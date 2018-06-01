#include "SIM900.h"
#include <TinyGPS++.h>

#include <SoftwareSerial.h>
//We don't need the http functions. So we can disable the next line.
//#include "inetGSM.h"
#include "sms.h"
#include "call.h"

//To change pins for Software Serial, use the two lines in GSM.cpp.

//GSM Shield for Arduino
//www.open-electronics.org
//this code is based on the example of Arduino Labs.

//Simple sketch to check if an incoming call is from an authorized
//number and in this case, send to this number an SMS with the value
//of a digital input.
static const uint32_t GPSBaud = 9600;
//We have to create the classes for SMSs and calls.
TinyGPSPlus gps;
double Lat;
double Long;
CallGSM call;
SMSGSM sms;
SoftwareSerial ss(9, 8);

char number[20];
byte stat=0;
int value=0;
int pin=1;
char value_str[5];
char lat_print[10];
char Long_print[10];
const char *googlePrefix = "http://maps.google.com/maps?q=";


void setup() 
{
  pinMode(pin,INPUT);
  //Serial connection.
  Serial.begin(9600);
  ss.begin(GPSBaud);
  Serial.println("GSM Shield testing.");
  //Start configuration of shield with baudrate.
  //For http uses is raccomanded to use 4800 or slower.
  if (gsm.begin(9600))
    Serial.println("\nstatus=READY");
  else Serial.println("\nstatus=IDLE");
};

void loop() 
{
  
  
  dtostrf(Lat, 5, 6, lat_print);

  dtostrf(Long, 5, 6, Long_print);
  char sms_OK[160];
  sprintf(sms_OK, "Hey asshole your car is now: %s,%s", lat_print, Long_print);
  Serial.println(sms_OK);

  
  Lat = gps.location.lat();
  Long = gps.location.lng();
  //Chekcs status of call
  stat=call.CallStatusWithAuth(number,1,3);
  //If the incoming call is from an authorized number
  //saved on SIM in the positions range from 1 to 3.
  if(stat==CALL_INCOM_VOICE_AUTH){
    //Hang up the call.
    call.HangUp();
    delay(2000);
    sprintf(sms_OK, "Hey asshole your car is now: %s,%s", lat_print, Long_print);
    //Serial.println(sms_OK);
    //Check the value of the input.
    //value=digitalRead(1);
    //Convert the int to a string.
    //itoa(value,value_str,10);
    //Send an SMS to the previous number with
    //the value read previously.
    sms.SendSMS(number,sms_OK);
  }
  smartDelay(1000);
};

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}
