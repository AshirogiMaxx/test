String APN = "Internet";
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
//#include <TinyGPS++.h>
static const uint32_t GPSBaud = 9600;
//TinyGPSPlus gps;
double Lat;
double Long;
HardwareSerial fonaSS = Serial1;
HardwareSerial mySerial = Serial2;
Adafruit_GPS GPS(&mySerial);
#define GPSECHO  false 

// this keeps track of whether we're using the interrupt
// off by default!
boolean usingInterrupt = false;
void useInterrupt(boolean); // Func prototype keeps Arduino 0023 happy

char lat_print[10]= "sdfsdfsd";
char Long_print[10] ="sdklf";
int8_t answer;
String response;
int KeyTime;
unsigned long ATtimeOut = 10000; // How long we will give an AT command to comeplete
int onModulePin = 13;

//Sparkfun URL Building
const String publicKey = "zDK1GXOpGZTVJNlpM8ZN"; //Public Key for data stream
const String privateKey = "YypvwzoNwgcXyvGKbjlv"; //Private Key for data stream
const byte NUM_FIELDS = 4; //number of fields in data stream
const String fieldNames[NUM_FIELDS] = {"latitude", "longitude", "date", "time"}; //actual data fields
String fieldData[NUM_FIELDS]; //holder for the data values

//Holders for Position Data

String Date = "8.68";
String Time = "693.8";
//String lat_print = "25555";
//String Long_print = "10000";


void setup(){
    Serial.begin(9600);
    GPS.begin(9600);
    fonaSS.begin(9600);
    GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
    GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
    GPS.sendCommand(PGCMD_ANTENNA);
    useInterrupt(true);
    delay(1000);
    mySerial.println(PMTK_Q_RELEASE);
    Serial.println("Starting...");
    sendATcommand("ATE0", "OK", 2000);
    power_on();
    Serial.print("Mensajes ON...");sendATcommand("AT+CMGF=1", "OK", 2000);
    Serial.print("HTTP ON...");sendATcommand("AT+CGATT=1", "OK", 2000);
    Serial.print("GPRS ON...");sendATcommand("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"", "OK", 2000);
    Serial.print("APN ON...");sendATcommand("AT+SAPBR=3,1,\"APN\",\"HOLA\"","OK",5000);
    Serial.print("INTENRNET ON...");if(sendATcommand("AT+SAPBR=1,1","OK",10000)){
      if(response == "OK") {
            Serial.println("Engaged GPRS");
        } else {
            Serial.println("GPRS Already on");
        }
    
    }
   Serial.print("HTTTTTTP ON..."); sendATcommand("AT+HTTPINIT","OK",2000);
   Serial.print("READY ON...");sendATcommand("AT+HTTPPARA=\"CID\",1","OK",2000);
    
    
    //Serial.println("ok");
}

SIGNAL(TIMER0_COMPA_vect) {
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
#ifdef UDR0
  if (GPSECHO)
    if (c) UDR0 = c;  
    // writing direct to UDR0 is much much faster than Serial.print 
    // but only one character can be written at a time. 
#endif
}


void useInterrupt(boolean v) {
  if (v) {
    // Timer0 is already used for millis() - we'll just interrupt somewhere
    // in the middle and call the "Compare A" function above
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
    usingInterrupt = true;
  } else {
    // do not call the interrupt function COMPA anymore
    TIMSK0 &= ~_BV(OCIE0A);
    usingInterrupt = false;
  }
}

uint32_t timer = millis();

void loop(){
  
  // in case you are not using the interrupt above, you'll
  // need to 'hand query' the GPS, not suggested :(
  if (! usingInterrupt) {
    // read data from the GPS in the 'main loop'
    char c = GPS.read();
    // if you want to debug, this is a good time to do it!
    if (GPSECHO)
      if (c) Serial.print(c);
  }
  
  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences! 
    // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
    //Serial.println(GPS.lastNMEA());   // this also sets the newNMEAreceived() flag to false
  
    if (!GPS.parse(GPS.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
      return;  // we can fail to parse a sentence in which case we should just wait for another
  }

  dtostrf(GPS.latitudeDegrees, 5, 6, lat_print);
  dtostrf(GPS.longitudeDegrees, 5, 6, Long_print);
  
        fieldData[0] = lat_print; 
        fieldData[1] = Long_print;
        fieldData[2] = Date;
        fieldData[3] = Time;
  Serial.println("Enviando a La Pagina");
    if(sendURL()){
      Serial.println(response);
    }
  Serial.print("Make GET Request: ");
    if(sendATcommand("AT+HTTPACTION=0","OK",3000)){ //make get request =0 - GET, =1 - POST, =2 - HEAD
        Serial.println(response);
        
    }
    delay(30000);
    Serial.println("xiclo");
    
}



boolean sendURL()
{
  int complete = 0;
  char c;
  String content;
  unsigned long commandClock = millis();                      // Start the timeout clock
  //Print all of the URL components out into the Serial Port
  fonaSS.print("AT+HTTPPARA=\"URL\",\"");
  fonaSS.print("http://data.sparkfun.com/input/");
  fonaSS.print(publicKey);
  fonaSS.print("?private_key=");
  fonaSS.print(privateKey);

  for (int i_url=0; i_url<NUM_FIELDS; i_url++) {
    fonaSS.print("&");
    fonaSS.print(fieldNames[i_url]);
    fonaSS.print("=");
    fonaSS.print(fieldData[i_url]);
  }
  fonaSS.print("\"");
  fonaSS.println(); 
  /*
  //>>>>>DEBUG<<<<<<
  Serial.print("AT+HTTPPARA=\"URL\",\"");
  Serial.print("http://data.sparkfun.com/input/");
  Serial.print(publicKey);
  Serial.print("?private_key=");
  Serial.print(privateKey);
  for (int i_url=0; i_url<NUM_FIELDS; i_url++) {
    Serial.print("&");
    Serial.print(fieldNames[i_url]);
    Serial.print("=");
    Serial.print(fieldData[i_url]);
  }
  Serial.print("\"");
  Serial.println();
  //>>>>>>>>>>>>>>>>>>>>
  */
  while(!complete && commandClock <= millis() + ATtimeOut) {
    while(!fonaSS.available() && commandClock <= millis()+ATtimeOut);
    while(fonaSS.available()) {
      c = fonaSS.read();
      if(c == 0x0A || c == 0x0D);
      else content.concat(c);
    }
    response = content;
    complete = 1; 
  }
  if (complete ==1) return 1;
  else return 0;
}


void flushFONA() { //if there is anything is the fonaSS serial Buffer, clear it out and print it in the Serial Monitor.
    char inChar;
    while (fonaSS.available()){
        inChar = fonaSS.read();
        Serial.write(inChar);
        delay(20);
    }
}

void power_on(){
    uint8_t answer=0;
    answer = sendATcommand("AT", "OK", 2000);
    if (answer == 0)
    {
        // power on pulse
        digitalWrite(onModulePin,HIGH);
        delay(3000);
        digitalWrite(onModulePin,LOW);
    
        // waits for an answer from the module
        while(answer == 0){     // Send AT every two seconds and wait for the answer
            answer = sendATcommand("AT", "OK", 2000);    
        }
    }

}

int8_t sendATcommand(char* ATcommand, char* expected_answer, unsigned int timeout){
    uint8_t x=0, answer = 0;
    char response[100];
    unsigned long previous;
    
    memset(response, '\0',100);
    delay(100);
    
    while(fonaSS.available()>0) fonaSS.read();
    fonaSS.println(ATcommand);
    
    x=0;
    previous = millis();
        do{
        // if there are data in the UART input buffer, reads it and checks for the asnwer
        if(fonaSS.available() != 0){    
            response[x] = fonaSS.read();
            x++;
            // check if the desired answer is in the response of the module
            if (strstr(response, expected_answer) != NULL)    
            {
                answer = 1;
            }
        }
    // Waits for the asnwer with time out
    }while((answer == 0) && ((millis() - previous) < timeout));  
    Serial.println(response);  

    return answer;
}
/*int8_t sendATcommand(char* ATcommand, char* expected_answer, unsigned int timeout){
    uint8_t x=0, answer = 0;
    char response[100];
    unsigned long previous;
    char c;
    String content;
    
    memset(response, '\0',100);
    delay(100);
    
    while(fonaSS.available()>0) fonaSS.read();
    fonaSS.println(ATcommand);
    
    x=0;
    previous = millis();
        do{
        // if there are data in the UART input buffer, reads it and checks for the asnwer
        if(fonaSS.available()){    
            c = fonaSS.read();
            if(c == 0x0A || c ==   0x0D);
            else content.concat(c);
        }
         
        //response = content;
    // Waits for the asnwer with time out
    }while((answer == 0) && ((millis() - previous) < timeout));    
    //Serial.println(response);
    Serial.println(content);
    return answer;
}*/
/*
static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (gpshs.available())
      gps.encode(gpshs.read());
  } while (millis() - start < ms);
}*/
