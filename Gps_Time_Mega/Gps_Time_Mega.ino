/*
 * TimeGPS.pde
 * example code illustrating time synced from a GPS
 * 
 */
 
#include <Time.h>
#include <TinyGPS.h>       //http://arduiniana.org/libraries/TinyGPS/
#include <SoftwareSerial.h> 
// TinyGPS and SoftwareSerial libraries are the work of Mikal Hart
 
#define ECHO_NMEA false  // Set to false if you don't want to see the raw GPS data
 
TinyGPS gps; 
HardwareSerial serial_gps = Serial1;  // receive on pin 3
 
const int offset = 1;   // offset hours from gps time (UTC)
time_t prevDisplay = 0; // when the digital clock was displayed
 
void setup()
{
  Serial.begin(9600);
  serial_gps.begin(9600);
  Serial.println("Waiting for GPS time ... ");
  setSyncProvider(gpsTimeSync);
}
 
void loop()
{
  char c;
  while (serial_gps.available()) 
  {
    // Echo the raw NMEA data, if required
    c = serial_gps.read();
    if (ECHO_NMEA) {
      Serial.print(c);
    }
    if (gps.encode(c)) {
      if (ECHO_NMEA) {
        Serial.println();
      }
      // If the time hasn't been set, retry now that we've received valid data from the GPS
      if (timeStatus() == timeNotSet) { 
        now();
      }
    }
  }
  if(timeStatus()!= timeNotSet) 
  {
     if( now() != prevDisplay) //update the display only if the time has changed
     {
       prevDisplay = now();
       digitalClockDisplay();  
     }
  }  
}
 
void digitalClockDisplay() {
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year()); 
  Serial.println(); 
}
 
void printDigits(int digits) {
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
     Serial.print('0');   
  Serial.print(digits); 
}
 
time_t gpsTimeSync() {
  //  returns time if avail from gps, else returns 0
  unsigned long fix_age = 0 ;   
  unsigned long date, time;
  gps.get_datetime(&date, &time, &fix_age);
  // Ignore if fix age is invalid or stale
  if (fix_age == TinyGPS::GPS_INVALID_AGE || fix_age > 1000) {
    Serial.print("TimeSync failed due to "); Serial.print(fix_age == TinyGPS::GPS_INVALID_AGE ? "invalid" : "stale"); Serial.println(" fix age");
    return 0;
  }
  // Ignore if no valid date or time yet received, such as when GPGGA is received before a GPRMC, because there's no date in a GPGGA
  if (date == TinyGPS::GPS_INVALID_DATE || time == TinyGPS::GPS_INVALID_TIME) {
    Serial.print("TimeSync failed due to invalid "); Serial.println(date == TinyGPS::GPS_INVALID_DATE ? "date" : "time");
    return 0;
  }
  return gpsTimeToArduinoTime(); // return time only if updated recently by gps 
}
 
time_t gpsTimeToArduinoTime() {
  // returns time_t from gps date and time with the given offset hours
  tmElements_t tm;
  int year;
  gps.crack_datetime(&year, &tm.Month, &tm.Day, &tm.Hour, &tm.Minute, &tm.Second, NULL, NULL);
  tm.Year = year - 1970; 
  time_t time = makeTime(tm);
  return time + (offset * SECS_PER_HOUR);
}
