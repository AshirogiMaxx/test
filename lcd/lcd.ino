
#include <SPFD5408_Adafruit_GFX.h>    // Core graphics library
#include <SPFD5408_Adafruit_TFTLCD.h> // Hardware-specific library
#include <SPFD5408_TouchScreen.h>     // Touch library

// Calibrates value
#define SENSIBILITY 300
#define MINPRESSURE 10
#define MAXPRESSURE 1000

//These are the pins for the shield!
#define YP A3 
#define XM A2 
#define YM 9 
#define XP 8 

short TS_MINX=150;
short TS_MINY=120;
short TS_MAXX=920;
short TS_MAXY=940;

// Init TouchScreen:

TouchScreen ts = TouchScreen(XP, YP, XM, YM, SENSIBILITY);

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4 // Optional : otherwise connect to Arduino's reset pin

// Assign human-readable names to some common 16-bit color values:
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

// Dimensions

uint16_t width = 0;
uint16_t height = 0;

// Buttons

#define BUTTONS 3
#define BUTTON_CLEAR 0
#define BUTTON_SHOW 1
#define BUTTON_RESET 2

Adafruit_GFX_Button buttons[BUTTONS];

uint16_t buttons_y = 0;

//-- Setup

void setup() {
  // put your setup code here, to run once:
  tft.reset();
  tft.begin(0x9341);
  tft.setRotation(0);
  width = tft.width() - 1;
  height = tft.height() - 1;
  initializeButtons();
  drawBorder();

}

void loop() {
  // put your main code here, to run repeatedly:
  tft.drawLine(0, 0, width, height, RED);
}


void drawBorder () {

  uint16_t width = tft.width() - 1;
  uint16_t height = tft.height() - 1;
  uint8_t border = 5;

  tft.fillScreen(BLUE);
  tft.fillRect(border, border, (width - border * 2), (height - border * 2), WHITE);
  
}

void initializeButtons() {

  uint16_t x = 40;
  uint16_t y = height - 20;
  uint16_t w = 75;
  uint16_t h = 20;
  
  uint8_t spacing_x = 5;
  
  uint8_t textSize = 1;

  char buttonlabels[3][20] = {"Clear", "Show", "Recalib."};
  uint16_t buttoncolors[15] = {RED, BLUE, RED};

  for (uint8_t b=0; b<3; b++) {
    buttons[b].initButton(&tft,                           // TFT object
                  x+b*(w+spacing_x),  y,                  // x, y,
                  w, h, WHITE, buttoncolors[b], WHITE,    // w, h, outline, fill, 
                  buttonlabels[b], textSize);             // text
  }

  // Save the y position to avoid draws
  
  buttons_y = y;
  
}
