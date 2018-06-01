// rf95_client.pde
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messageing client
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example rf95_server
// Tested with Anarduino MiniWirelessLoRa, Rocket Scream Mini Ultra Pro with the RFM95W 

#include <SPI.h>
#include <RH_RF95.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN      3
#define DHTTYPE     DHT22

String id_node= "ONE";
// Singleton instance of the radio driver
//RH_RF95 rf95;
RH_RF95 rf95(10, 2);
//RH_RF95 driver(5, 2); // Rocket Scream Mini Ultra Pro with the RFM95W

// Need this on Arduino Zero with SerialUSB port (eg RocketScream Mini Ultra Pro)
//#define Serial SerialUSB

// Initialice the dht sensor
DHT_Unified dht(DHTPIN, DHTTYPE);
String Chunk;
 
void setup() 
{
  // Rocket Scream Mini Ultra Pro with the RFM95W only:
  // Ensure serial flash is not interfering with radio communication on SPI bus
//  pinMode(4, OUTPUT);
//  digitalWrite(4, HIGH);

  Serial.begin(9600);
  dht.begin();
  
  Serial.println("DHTxx Unified Sensor Example");
  sensor_t sensor;
  while (!Serial) ; // Wait for serial port to be available

  Serial.println("Arduino LoRa TX");
  
  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }
  Serial.println("LoRa radio init OK!");
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
  // you can set transmitter powers from 5 to 23 dBm:
   //rf95.setTxPower(23, true);
  // If you are using Modtronix inAir4 or inAir9,or any other module which uses the
  // transmitter RFO pins and not the PA_BOOST pins
  // then you can configure the power transmitter power for -1 to 14 dBm and with useRFO true. 
  // Failure to do that will result in extremely low transmit powers.
//rf95.setTxPower(14, true);
}

int16_t packetnum = 0;

void loop()
{
  Chunk += id_node;
  Chunk += ",";
  sensors_event_t event;   
  Serial.println("Sending to rf95_server");
  // Send a message to rf95_server
  dht.temperature().getEvent(&event);

  if (isnan(event.temperature)) {
    Serial.println("Error reading temperature!");
  }
  else {
    Serial.print("Temperature: ");
    Serial.print(event.temperature);
    Serial.println(" *C");
    Chunk += event.temperature;
    Chunk += ",";
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println("Error reading humidity!");
  }
  else {
    Serial.print("Humidity: ");
    Serial.print(event.relative_humidity);
    Serial.println("%");
    Chunk += event.relative_humidity;
    Chunk += ",";
  }
  char sendch[Chunk.length()];
  Chunk.toCharArray(sendch,Chunk.length());  
  Serial.print("Sending "); Serial.println(sendch);
  
  Serial.println("Sending..."); delay(10);
  rf95.send((uint8_t *)sendch, Chunk.length());
  Chunk = "";
  
  Serial.println("Waiting for packet to complete..."); delay(10);
  rf95.waitPacketSent();
  
  // Now wait for a reply
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  if (rf95.waitAvailableTimeout(3000))
  { 
    // Should be a reply message for us now   
    if (rf95.recv(buf, &len))
   {
      Serial.print("got reply: ");
      Serial.println((char*)buf);
      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);    
    }
    else
    {
      Serial.println("recv failed");
    }
  }
  else
  {
    Serial.println("No reply, is rf95_server running?");
  }
  delay(1000);
}


