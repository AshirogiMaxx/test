// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>


const char* ssid     = "TP-LINK_A66676";
const char* password = "04566014";

ESP8266WebServer server(80);

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            12

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      8
#define BUTTON_LOW 13
#define BUTTON_MID 14
#define BUTTON_HIGH 16

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ400);

int delayval = 1000; // delay for half a second
int color = 255;

int red, green, blue;

int r, g, b;

void handleRoot() {

  String color, finale;
  String hexstring = "#FF3Fa0";

  if (server.hasArg("chroma")) {
    Serial.print("Variable Exists: ");
    color = server.arg("chroma");
    Serial.println(color);
    //String test = color;
    //color.remove(0,1);
    //Serial.println(color);
    long number = strtol( &color[1], NULL, 16);
    Serial.println(number);
    r = number >> 16;
    g = number >> 8 & 0xFF;
    b = number & 0xFF;
    Serial.println(r);
    Serial.println(g);
    Serial.println(b);
    EEPROM.write(100,r);
    EEPROM.write(101,g);
    EEPROM.write(102,b);
    EEPROM.commit();
    
  }

  String temp = "<html>";
  temp += "<body><form action='/' method='POST'>";
  temp += "<p>Change the color of the color picker, and then click the button below.</p>";
  temp += "Color picker: <input type='color' name='chroma' id='myColor' value='#ff0080'>";
  temp += "<input type='submit' name='SUBMIT' value='Submit'></form>";
  temp += "<p>Note that the default value is not affected when you change the value of the color picker.</p>";
  temp += "<button type='button' onclick='myFunction()'>Try it</button>";
  temp += "<p><b>Note:</b> type='color' is not supported in Internet Explorer 11 and earlier versions or Safari 9.1 and earlier versions.</p>";
  temp += "<p id='demo'></p>";
  temp += "<script>";
  temp += "function myFunction() {";
  temp += "var x = document.getElementById('myColor');";
  temp += "    var defaultVal = x.defaultValue;";
  temp += "    var currentVal = x.value;";

  temp += "    if (defaultVal == currentVal) {";
  temp += "        document.getElementById(demo').innerHTML = 'Default value and current value is the same: '";
  temp += "        + x.defaultValue + ' and ' + x.value";
  temp += "        + '<br>Change the color of the color picker to see the difference!';";
  temp += "    } else {";
  temp += "        document.getElementById('demo').innerHTML = 'The default value was: ' + defaultVal";
  temp += "        + '<br>The new, current value is: ' + currentVal;";
  temp += "    }";
  temp += "}";
  temp += "</script>";
  temp += "</body>";
  temp += "</html>";



  server.send(200, "text/html", temp);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}


void setup() {

  pixels.begin(); // This initializes the NeoPixel library.
  pinMode(BUTTON_LOW, INPUT_PULLUP);
  pinMode(BUTTON_MID, INPUT_PULLUP);
  pinMode(BUTTON_HIGH, INPUT_PULLUP);
  EEPROM.begin(512);
  Serial.begin(115200); 

  red = EEPROM.read(100);
  green = EEPROM.read(101);
  blue = EEPROM.read(102);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);
  server.begin();
}
  

void loop() {

  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.
  server.handleClient();
  bool newstate1 = digitalRead(BUTTON_LOW);
  bool newstate2 = digitalRead(BUTTON_MID);
   Serial.println(red);
    Serial.println(green);
     Serial.println(blue);

  

  /*Serial.print("The status from newstate1 is: ");
  Serial.println(newstate1);
  Serial.print("The status from newstate2 is: ");
  Serial.println(newstate2);
  */
  if (newstate1 == LOW){
    for(int i=0;i<NUMPIXELS/2;i++){
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(red,green,blue)); // Moderately bright green color.
      pixels.show(); // This sends the updated pixel color to the hardware.

      //delay(delayval); // Delay for a period of time (in milliseconds).
      }
  }else{
      for(int i=0;i<NUMPIXELS/2;i++){
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(0,0,0)); // Moderately bright green color.
      pixels.show(); // This sends the updated pixel color to the hardware.

      //delay(delayval); // Delay for a period of time (in milliseconds).
      }
    }
  
  if(newstate2 == LOW){
    for(int i=NUMPIXELS/2;i<NUMPIXELS;i++){
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(0,70,30));
      pixels.show(); // This sends the updated pixel color to the hardware.

      //delay(delayval); // Delay for a period of time (in milliseconds).
     }
  }else{
      for(int i=NUMPIXELS/2;i<NUMPIXELS;i++){
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(0,0,0));
      pixels.show(); // This sends the updated pixel color to the hardware.

      //delay(delayval); // Delay for a period of time (in milliseconds).
     }
    }
}
