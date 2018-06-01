// Paint example specifically for the TFTLCD breakout board.
// If using the Arduino shield, use the tftpaint_shield.pde sketch instead!
// DOES NOT CURRENTLY WORK ON ARDUINO LEONARDO

// Modified for SPFD5408 Library by Joao Lopes
// Version 0.9.2 - Rotation for Mega

// *** SPFD5408 change -- Begin
#include <SPFD5408_Adafruit_GFX.h>    // Core graphics library
#include <SPFD5408_Adafruit_TFTLCD.h> // Hardware-specific library
#include <SPFD5408_TouchScreen.h>
// *** SPFD5408 change -- End

#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif

// When using the BREAKOUT BOARD only, use these 8 data lines to the LCD:
// For the Arduino Uno, Duemilanove, Diecimila, etc.:
//   D0 connects to digital pin 8  (Notice these are
//   D1 connects to digital pin 9   NOT in order!)
//   D2 connects to digital pin 2
//   D3 connects to digital pin 3
//   D4 connects to digital pin 4
//   D5 connects to digital pin 5
//   D6 connects to digital pin 6
//   D7 connects to digital pin 7

// For the Arduino Mega, use digital pins 22 through 29
// (on the 2-row header at the end of the board).
//   D0 connects to digital pin 22
//   D1 connects to digital pin 23
//   D2 connects to digital pin 24
//   D3 connects to digital pin 25
//   D4 connects to digital pin 26
//   D5 connects to digital pin 27
//   D6 connects to digital pin 28
//   D7 connects to digital pin 29

// For the Arduino Due, use digital pins 33 through 40
// (on the 2-row header at the end of the board).
//   D0 connects to digital pin 33
//   D1 connects to digital pin 34
//   D2 connects to digital pin 35
//   D3 connects to digital pin 36
//   D4 connects to digital pin 37
//   D5 connects to digital pin 38
//   D6 connects to digital pin 39
//   D7 connects to digital pin 40

#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin
#define PIN_TRIG 26
#define PIN_ECO  24
#define VCC 28
#define GND 22
 

// Original values
//#define TS_MINX 150
//#define TS_MINY 120
//#define TS_MAXX 920
//#define TS_MAXY 940

// Calibrate values
#define TS_MINX 125
#define TS_MINY 85
#define TS_MAXX 965
#define TS_MAXY 905

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
// optional
#define LCD_RESET A4

// Assign human-readable names to some common 16-bit color values:
#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF


Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

#define BOXSIZE 40
#define PENRADIUS 2
int oldcolor, currentcolor;
boolean buttonEnabled = true;
unsigned int screen = 0;

void setup() {

  // put your setup code here, to run once:
  pinMode(VCC, OUTPUT);
  pinMode(GND, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECO, INPUT);
  digitalWrite(VCC, HIGH);
  digitalWrite(GND, LOW);


  
  Serial.begin(9600);
  Serial.println(F("Paint"));
  uint16_t identifier = tft.readID();
  if(identifier == 0x9341){
    Serial.println(F("Found ILI9431 LCD driver"));
    Serial.println(identifier, HEX);
    }
    else {
    Serial.print(F("Unknown LCD driver chip: "));
    Serial.println(identifier, HEX);
    Serial.println(F("If using the Adafruit 2.8\" TFT Arduino shield, the line:"));
    Serial.println(F("  #define USE_ADAFRUIT_SHIELD_PINOUT"));
    Serial.println(F("should appear in the library header (Adafruit_TFT.h)."));
    Serial.println(F("If using the breakout board, it should NOT be #defined!"));
    Serial.println(F("Also if using the breakout, double-check that all wiring"));
    Serial.println(F("matches the tutorial."));
    return;
  }
  tft.begin(identifier);
  tft.setRotation(0);
  drawBorder();

  tft.fillScreen(BLACK);
  tft.fillRect(45, 45, 120, 40, RED);

  tft.setCursor(55, 50);
  tft.setTextSize(2);
  tft.setTextColor(BLUE);
  tft.println("Main Menu");
  

  waitOneTouch();

  tft.fillScreen(BLACK);

  tft.fillRect(0, 0, 120, BOXSIZE, RED);
  tft.fillRect(0, 40, 120, BOXSIZE, YELLOW);
  tft.fillRect(0, 80, 120, BOXSIZE, GREEN);
  //tft.fillRect(0, 120, 80, BOXSIZE, CYAN);

  tft.setCursor(10,20 );
  tft.setTextSize(1);
  tft.setTextColor(BLUE);
  tft.println("Temp:");

  tft.setCursor(10, 50);
  tft.setTextSize(1);
  tft.setTextColor(BLUE);
  tft.println("Humid");

  tft.setCursor(10, 90);
  tft.setTextSize(1);
  tft.setTextColor(BLUE);
  tft.println("GPS");


  

}

#define MINPRESSURE 10
#define MAXPRESSURE 1000

void loop() {

 

    //declare variables for ultrasonic sensor
  
  //trigger the signal to sensor

  
  
    TSPoint p = ts.getPoint();
    buttonEnabled = true;
    screen = 0;
    

    p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
    // *** SPFD5408 change -- End
    p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
  

    if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    
    Serial.print("X = "); Serial.print(p.x);
    Serial.print("\tY = "); Serial.print(p.y);
    Serial.print("\tPressure = "); Serial.println(p.z);

    
    waitOneTouch();
    if(p.y > 80 && p.y < 120 && p.x < 120 && buttonEnabled){

      buttonEnabled = false; //Disable button
      Serial.println("Button Pressed");
      
      /*tft.fillScreen(BLACK);

      
      tft.setCursor(0, 50);
      tft.setTextSize(2);
      tft.setTextColor(BLUE);
      tft.println("Ultrasonic Sensor");
      */
      drawUltrasonic();
      }
      
      delay(10);  
}
  
}


void drawUltrasonic()
{
   long duracion, distancia;
   tft.fillScreen(BLACK);
  int steps = 1;
   //Draw frame
   tft.drawRect(0,0,239,320,WHITE);

   drawVerticalLine(50);

   drawVerticalLine(100);

      tft.setCursor(0, 50);
      tft.setTextSize(2);
      tft.setTextColor(BLUE);
      tft.println("Ultrasonic Sensor:");

      while(steps == 1){
      tft.fillRect(30, 80, 50, 20, BLACK);
    digitalWrite(PIN_TRIG, LOW);  
    delayMicroseconds(2); 
    digitalWrite(PIN_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(PIN_TRIG, LOW);
  
  //receive the value from sensor
  duracion = pulseIn(PIN_ECO, HIGH);
  distancia = (duracion/2) / 29;


      tft.setCursor(30, 80);
      //tft.print("Y: ");
      tft.print(distancia);
      delay(500);

      }

   //waitOneTouch();

   //drawHorizontalLine(80);

   //drawHorizontalLine(150);
}


void drawVerticalLine(int x)
{
  int i=0;
  for(i=0;i<7;i++)
  {
    tft.drawLine(x+i,20,x+i,220,WHITE);
  }
}




TSPoint waitOneTouch() {

  // wait 1 touch to exit function
  
  TSPoint p;
  
  do {
    p= ts.getPoint(); 
  
    pinMode(XM, OUTPUT); //Pins configures again for TFT control
    pinMode(YP, OUTPUT);
  
  } while((p.z < MINPRESSURE )|| (p.z > MAXPRESSURE));
  
  return p;
}

void drawBorder(){

    uint16_t width = tft.width() - 1;
    uint16_t height = tft.height() - 1;
    uint8_t border = 3;

    tft.fillScreen(BLACK);
    tft.fillRect(border, border, (width - border * 2), (height - border * 2 ), WHITE);
}

