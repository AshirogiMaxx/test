#include "SIM900.h"
#include <SoftwareSerial.h>
//We don't need the http functions. So we can disable the next line.
//#include "inetGSM.h"
#include "sms.h"
#include "call.h"

SMSGSM sms;
 
char number[]="4499800433";
 
char message[180];
 
char pos;
 
char *p;
 
void setup()
 
{
 
 Serial.begin(9600);
 
 if (gsm.begin(9600))
 
   Serial.println("\nstatus=READY");
 
 else Serial.println("\nstatus=IDLE");
 
};
 
void loop()
 
{
 
 pos=sms.IsSMSPresent(SMS_UNREAD);
 
 Serial.println((int)pos);
 
 if((int)pos>0&&(int)pos<=20){
 
   Serial.print("NUOVO MESSAGGIO, POS=");
 
   Serial.println((int)pos);
 
   message[0]='\0';
 
   sms.GetSMS((int)pos,number,message,180);
 
   p=strstr(message,"testpwd");
 
   if(p){
 
     Serial.println("PSWD OK");
 
     p=strstr(message,"LEDON");
 
     if(p){
 
       Serial.println("LED ON");
 
       digitalWrite(13,HIGH);
 
     }
 
     else{
 
       p=strstr(message,"LEDOFF");
 
       if(p){
 
         Serial.println("LED OFF");
 
         digitalWrite(13,LOW);
 
       }
 
     }
 
   }
 
   sms.DeleteSMS((int)pos);
 
 }
 
 delay(5000);
 
};
