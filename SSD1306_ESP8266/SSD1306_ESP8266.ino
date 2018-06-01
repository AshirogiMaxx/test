#include "ESP8266WiFi.h"
#include <Wire.h>
#include <ACROBOTIC_SSD1306.h>

void setup()
{
  Wire.begin();  
  oled.init();                      // Initialze SSD1306 OLED display
  oled.clearDisplay();              // Clear screen
  oled.setTextXY(0,0);              // Set cursor position, start of line 0
  oled.putString("  MJRoBot.org");
  oled.setTextXY(4,0);              // Set cursor position, start of line 4
  oled.putString("  HELLO, WORLD");
}

void loop()
{
}
