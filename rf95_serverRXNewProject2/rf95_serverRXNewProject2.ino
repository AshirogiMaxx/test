// rf95_server.pde
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messageing server
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95  if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example rf95_client
// Tested with Anarduino MiniWirelessLoRa, Rocket Scream Mini Ultra Pro with the RFM95W 


#include <SPI.h>
#include <RH_RF95.h>
#include <Wire.h>

#define PIN            5
#define OLED_RESET 4

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      8

//Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ400);// change the NEO_KHZ TO 400 TO WORK PROPERLY IN 8MHZ

int delayval = 3; // delay for half a second
// Singleton instance of the radio driver
//RH_RF95 rf95;
RH_RF95 rf95(10, 2);
//RH_RF95 rf95(5, 2); // Rocket Scream Mini Ultra Pro with the RFM95W
#define RFM95_RST 9
#define RFM95_FREQ 915.0

// Need this on Arduino Zero with SerialUSB port (eg RocketScream Mini Ultra Pro)
//#define Serial SerialUSB

//int led = 13;
//int color = 10;

void setup() 
{
  // Rocket Scream Mini Ultra Pro with the RFM95W only:
  // Ensure serial flash is not interfering with radio communication on SPI bus   
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
  delay(2000);

  //pinMode(led, OUTPUT);
       
  while (!Serial);
  Serial.begin(115200);
  //delay(100);

  Serial.println("Arduino LoRa RX");
  
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);
  
  // long range configuration requires for on-air time
  /*boolean longRange = false;
  if (longRange) {
    RH_RF95::ModemConfig modem_config = {
      0x78, // Reg 0x1D: BW=125kHz, Coding=4/8, Header=explicit
      0xC4, // Reg 0x1E: Spread=4096chips/symbol, CRC=enable
      0x08  // Reg 0x26: LowDataRate=On, Agc=Off.  0x0C is LowDataRate=ON, ACG=ON
    };
    rf95.setModemRegisters(&modem_config);
    if (!rf95.setModemConfig(RH_RF95::Bw125Cr48Sf4096)) {
      Serial.println("set config failed");
    }
  } */ 

  



  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }
  Serial.println("LoRa radio init OK!");
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
  if (!rf95.setFrequency(RFM95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RFM95_FREQ);
  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
  // you can set transmitter powers from 5 to 23 dBm
  rf95.setTxPower(23, true);
  // If you are using Modtronix inAir4 or inAir9,or any other module which uses the
  // transmitter RFO pins and not the PA_BOOST pins
  // then you can configure the power transmitter power for -1 to 14 dBm and with useRFO true. 
  // Failure to do that will result in extremely low transmit powers.
  //rf95.setTxPower(14, true);
  /*if (!rf95.setModemConfig(RH_RF95::Bw125Cr48Sf4096)) {
    Serial.println("set config failed");
  }*/
}

void loop()
{
  
  if (rf95.available())
  {
    // Should be a message for us now   
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    if (rf95.recv(buf, &len))
    {
      //digitalWrite(led, HIGH);
      RH_RF95::printBuffer("request: ", buf, len);

      Serial.print("got request: ");
      Serial.println((char*)buf);
      
      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);
      
      // Send a reply
      uint8_t data[] = "And hello back to you number 2";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent();
      Serial.println("Sent a reply");
      //digitalWrite(led, LOW);
    
     }
     else
     {
        Serial.println("recv failed");
     }

  }
}


