/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 by ThingPulse, Daniel Eichhorn
 * Copyright (c) 2018 by Fabrice Weinberg
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * ThingPulse invests considerable time and money to develop these open source libraries.
 * Please support us by buying our products (and not the clones) from
 * https://thingpulse.com
 *
 */

// Include the correct display library
// For a connection via I2C using Wire include
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`
// or #include "SH1106Wire.h", legacy include: `#include "SH1106.h"`
// For a connection via I2C using brzo_i2c (must be installed) include
// #include <brzo_i2c.h> // Only needed for Arduino 1.6.5 and earlier
// #include "SSD1306Brzo.h"
// #include "SH1106Brzo.h"
// For a connection via SPI include
// #include <SPI.h> // Only needed for Arduino 1.6.5 and earlier
// #include "SSD1306Spi.h"
// #include "SH1106SPi.h"

// Include custom images
#include "images.h"

SSD1306Wire  display(0x3c, 5, 4);

unsigned long prev, interval = 100; //Variables for display/clock update rate
byte flash = 0; //Flag for display flashing - toggle once per update interval
byte mode = 6; //Mode for time and date setting
int tempset; //Temporary variable for setting time/date
int framecount = 3; //Framecounter for animation. Initialized to last frame to start animation at first frame
int framecount2 = 0; //Counter for number of display update periods - for timing display image changes
int imagecounter = 4; //Counter for display of new static image - Initialized to 4 to start static image display at beginning
uint8_t secset = 0; //Index for second RTC setting
uint8_t minset = 1; //Index for minute RTC setting
uint8_t hourset = 2; //Index for hour RTC setting
uint8_t wdayset = 3; //Index for weekday RTC setting
uint8_t mdayset = 4; //Index for date RTC setting
uint8_t monset = 5; //Index for month RTC setting
uint8_t yearset = 6; //Index for year RTC setting

//Alarm time variables
uint8_t wake_HOUR = 0;
uint8_t wake_MINUTE = 0;
uint8_t wake_SECOND = 0;
uint8_t wake_SET = 1; //Default alarm to ON in case of power failure or reset
int beepcount = 0; //Variable for number of 100ms intervals since alarm started sounding
const int alarmEE = 0; //EEPROM alarm status storage location


//First animation frame image
static const unsigned char PROGMEM frame_000[] =
{
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x70, 0x00, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xD8, 0x01, 0xCC,
0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xC0, 0x00, 0x00, 0x00, 0xCE, 0x0F, 0x8C, 0x00, 0x00, 0x00,
0x00, 0x00, 0x07, 0xC0, 0x00, 0x00, 0x00, 0xC7, 0xFF, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07,
0xC0, 0x00, 0x00, 0x1D, 0x83, 0xFF, 0x06, 0x38, 0x00, 0x00, 0x00, 0x00, 0x06, 0xC0, 0x00, 0x0F,
0xFF, 0x81, 0xFB, 0x03, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x05, 0xC0, 0x00, 0xFF, 0xFF, 0xC0, 0x00,
0x33, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x07, 0xC0, 0x07, 0xFE, 0x03, 0xE2, 0x00, 0x33, 0x80, 0x00,
0x00, 0x00, 0x00, 0x07, 0xC0, 0x1F, 0x0E, 0x00, 0x63, 0x80, 0xE1, 0x80, 0x00, 0x00, 0x00, 0x00,
0x07, 0x40, 0x7F, 0x8E, 0x00, 0x23, 0x80, 0x79, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x06, 0x40, 0xE7,
0x84, 0x00, 0x27, 0x04, 0x39, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x06, 0xE1, 0xC7, 0x00, 0x1F, 0xE7,
0x0E, 0x38, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x07, 0xE3, 0x80, 0x00, 0x1E, 0x07, 0x1E, 0x10, 0xC0,
0x00, 0x00, 0x00, 0x00, 0x03, 0xF3, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0xC0, 0x00, 0x00, 0x00,
0x00, 0x03, 0xDE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x01, 0xBE,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFC, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00,
0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00,
0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x00,
0x01, 0x9C, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x1D, 0xFF, 0x00,
0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0xFF, 0xF3, 0xC0, 0x00, 0x08, 0x00,
0x00, 0x00, 0x00, 0x00, 0x03, 0x80, 0x00, 0xF8, 0x79, 0xC0, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00,
0x00, 0x03, 0x00, 0x00, 0xE0, 0x79, 0xE0, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00,
0x01, 0xC0, 0x7F, 0xE0, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x01, 0x80, 0x3F,
0xE0, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1B, 0x00, 0x01, 0x80, 0x0F, 0x60, 0x00, 0x03,
0x00, 0x00, 0x00, 0x00, 0x06, 0x7B, 0x00, 0x01, 0x80, 0x00, 0x60, 0x00, 0x03, 0x80, 0x00, 0x00,
0x00, 0x1F, 0xFE, 0x00, 0x01, 0x80, 0x00, 0xC0, 0x00, 0x01, 0xC0, 0x00, 0x00, 0x00, 0xFB, 0x04,
0x00, 0x01, 0x80, 0x00, 0xC0, 0x00, 0x00, 0xE0, 0x00, 0x00, 0x1F, 0xFB, 0x00, 0x00, 0x01, 0xC0,
0x07, 0xC0, 0x00, 0x00, 0x70, 0x00, 0x7F, 0xFE, 0x1E, 0x00, 0x00, 0x00, 0xE0, 0x3F, 0xC0, 0x00,
0x00, 0x3D, 0xFF, 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xFF, 0x00, 0x00, 0x00, 0x0D, 0xF1,
0xD8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xC0, 0x00, 0x00, 0x00, 0x06, 0xC0, 0xF8, 0x00, 0x00,
0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x03, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

//Second animation frame image
static const unsigned char PROGMEM frame_001[] =
{
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78,
0x00, 0x00, 0x00, 0x00, 0x00, 0xC6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x07,
0xFF, 0xFF, 0x83, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x7F, 0xFF, 0xFF, 0x03,
0xC0, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x2E, 0x00, 0x03, 0xF0, 0x78, 0x07, 0x03, 0xFF, 0xCC, 0x00,
0x00, 0x00, 0x00, 0x3F, 0x00, 0x1F, 0xE0, 0x78, 0x01, 0x83, 0xBF, 0x0C, 0x00, 0x00, 0x00, 0x00,
0x3F, 0x80, 0x38, 0xE0, 0x30, 0x00, 0xC7, 0x3E, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x1C, 0xE0, 0xF0,
0xE0, 0x00, 0x00, 0xC6, 0x04, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x1D, 0xF9, 0xC0, 0x00, 0x00, 0x38,
0x06, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xDF, 0x80, 0x00, 0x00, 0x7F, 0x9F, 0x01, 0xCC,
0xE0, 0x00, 0x00, 0x00, 0x07, 0xDF, 0x00, 0x00, 0x00, 0x01, 0x9C, 0x07, 0x8F, 0xE0, 0x00, 0x00,
0x00, 0x01, 0xDE, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x32, 0xCE, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC,
0x00, 0x00, 0x00, 0x00, 0x00, 0xF1, 0xEC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00,
0x00, 0x00, 0xD9, 0xCF, 0x80, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10,
0x0F, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x40, 0x00,
0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00,
0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00,
0x0C, 0x39, 0x80, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x7F, 0xFF, 0x80,
0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x7F, 0xEF, 0xC0, 0x00, 0x06, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0xFC, 0x0F, 0xF0, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x20, 0x01, 0xC0, 0x0F, 0xF0, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60,
0x01, 0x80, 0x0F, 0xF0, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x03, 0x00, 0x07,
0xF0, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x30, 0x03, 0x00, 0x07, 0xD0, 0x00, 0x02,
0x70, 0x00, 0x00, 0x00, 0x00, 0x07, 0xD0, 0x03, 0x00, 0x01, 0xB0, 0x00, 0x03, 0xFF, 0x80, 0x00,
0x00, 0x0C, 0xFE, 0xF0, 0x01, 0x80, 0x00, 0x30, 0x00, 0x03, 0x83, 0xF3, 0xFF, 0xFF, 0xFE, 0xE0,
0x00, 0x01, 0x80, 0x00, 0x70, 0x00, 0x00, 0x00, 0x77, 0xFF, 0xFC, 0x07, 0x60, 0x00, 0x01, 0xF8,
0x0D, 0xF0, 0x00, 0x00, 0x00, 0x2E, 0x00, 0x00, 0x03, 0xE0, 0x00, 0x01, 0xBF, 0xFF, 0x00, 0x00,
0x00, 0x00, 0x3C, 0x00, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x0E, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

//Third animation frame image
static const unsigned char PROGMEM frame_002[] =
{
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xE0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x06, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07,
0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xC0, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x09, 0x80, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x1F, 0x80, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x80, 0x3F,
0xFF, 0xFC, 0x73, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x81, 0xFE, 0x0F, 0x1F, 0xC3,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0xC7, 0x9E, 0x0F, 0x03, 0xC1, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x09, 0xFE, 0x1E, 0x0E, 0x0F, 0x01, 0xE0, 0x38, 0x00, 0x00, 0x00, 0x00,
0x00, 0x0F, 0xF0, 0x0C, 0x06, 0x03, 0x81, 0xFC, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xE0,
0x00, 0x00, 0x01, 0xC1, 0xBF, 0xCC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x80, 0x00, 0x00, 0x00,
0xC2, 0x3F, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x03, 0x37, 0x04,
0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x3F, 0xC3, 0x00, 0x06, 0x00, 0x00, 0x00,
0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0xDD, 0x00, 0x46, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02,
0x00, 0x00, 0x00, 0x00, 0x1C, 0x03, 0xC6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00,
0x00, 0x0C, 0x23, 0x07, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x61,
0xC7, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x01, 0xE1, 0xC4, 0x00, 0x00,
0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x31, 0xCE, 0x00, 0x00, 0x00, 0x00, 0x00,
0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x0D, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x0C, 0x40, 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00,
0x3F, 0x33, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x3F, 0xFB, 0x00,
0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x7D, 0xFF, 0x80, 0x00, 0x02, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0xE0, 0x0D, 0xC0, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x30, 0x01, 0x80, 0x1E, 0x70, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20,
0x01, 0x80, 0x1E, 0xF0, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x03, 0x00, 0x1F,
0xF0, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x03, 0x00, 0x07, 0xF0, 0x00, 0x00,
0xC8, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x03, 0x00, 0x07, 0xF0, 0x00, 0x00, 0xEF, 0xC0, 0x00,
0x00, 0x00, 0x03, 0x80, 0x01, 0x00, 0x00, 0x60, 0x00, 0x00, 0x3D, 0xFF, 0x3C, 0x00, 0x00, 0x03,
0x00, 0x01, 0x80, 0x00, 0xE0, 0x00, 0x00, 0x3C, 0x0F, 0xBF, 0xFF, 0x81, 0xFB, 0x00, 0x01, 0xB0,
0x00, 0xC0, 0x00, 0x00, 0x08, 0x01, 0xB0, 0x3F, 0x9F, 0xFD, 0x00, 0x00, 0xFF, 0xFF, 0xC0, 0x00,
0x00, 0x00, 0x00, 0xF0, 0x00, 0xD8, 0x0D, 0x80, 0x00, 0xE7, 0xFF, 0xC0, 0x00, 0x00, 0x00, 0x00,
0x40, 0x00, 0xD8, 0x07, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

//Fourth animation frame image
static const unsigned char PROGMEM frame_003[] =
{
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00,
0x00, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4C, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5C, 0x00, 0x00, 0x0F, 0xE6, 0x73, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x7E, 0x00, 0x01, 0xFF, 0xFF, 0xC3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x7E, 0x00, 0x0F, 0xDE, 0x1F, 0xC1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7B, 0x00, 0x7E,
0x1E, 0x03, 0xC1, 0xC0, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x00, 0xFE, 0x1E, 0x00, 0xC1,
0xF8, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x37, 0x83, 0xDE, 0x1C, 0x00, 0x41, 0xBF, 0xE6, 0x00,
0x00, 0x00, 0x00, 0x00, 0x1F, 0xC7, 0x0E, 0x00, 0x3E, 0x67, 0xF7, 0x86, 0x00, 0x00, 0x00, 0x00,
0x00, 0x1E, 0xFE, 0x00, 0x00, 0x07, 0x86, 0x26, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x78,
0x00, 0x00, 0x01, 0x87, 0x06, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0xF0, 0x00, 0x00, 0x00,
0x1F, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xF0, 0x00, 0x00, 0x00, 0x3D, 0x00, 0xC6,
0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xE0, 0x00, 0x00, 0x00, 0x18, 0x03, 0xC6, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x63, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0xC0, 0x00, 0x00, 0x00, 0x01, 0xE3, 0xC7, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x00, 0x00,
0x00, 0x00, 0x33, 0xCE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x31,
0xCC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xC0, 0x00, 0x00, 0x00, 0x00,
0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00,
0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x00,
0x00, 0x18, 0x00, 0x0E, 0x18, 0x00, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00,
0x1F, 0x1D, 0x80, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x3F, 0xFF, 0x80,
0x00, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x38, 0x3F, 0x80, 0x00, 0x00, 0x60,
0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x30, 0x1E, 0xE0, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00,
0x00, 0x00, 0x60, 0x00, 0x60, 0x1E, 0xE0, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0,
0x00, 0x60, 0x1E, 0xF0, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x01, 0xC0, 0x00, 0xC0, 0x1F,
0xF0, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0xC0, 0x0F, 0xE0, 0x00, 0x00,
0x0C, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0xC0, 0x01, 0xE0, 0x00, 0x00, 0x07, 0x00, 0x00,
0x00, 0x00, 0x07, 0x00, 0x00, 0x60, 0x00, 0x60, 0x00, 0x00, 0x01, 0xC4, 0x06, 0x70, 0x00, 0x0E,
0x00, 0x00, 0x60, 0x00, 0xC0, 0x00, 0x00, 0x00, 0xE7, 0xFE, 0x7E, 0x60, 0x1C, 0x00, 0x00, 0x30,
0x01, 0xC0, 0x00, 0x00, 0x00, 0x73, 0xE3, 0xB6, 0x7F, 0xD8, 0x00, 0x00, 0x3F, 0xFF, 0x80, 0x00,
0x00, 0x00, 0x3E, 0x01, 0xF6, 0x61, 0xD8, 0x00, 0x00, 0x0F, 0xFF, 0x80, 0x00, 0x00, 0x00, 0x0E,
0x00, 0x03, 0x60, 0x68, 0x00, 0x00, 0x04, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xE0,
0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xE0, 0x38, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

//Pusheen Teacup static image
static const unsigned char PROGMEM pusheen_teacup[] =
{
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00,
0x00, 0x38, 0x03, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x07, 0xE0, 0x00, 0x00, 0x00, 0x00,
0x00, 0xE6, 0x0C, 0x61, 0xFE, 0x00, 0x00, 0x00, 0x00, 0xC7, 0xFC, 0x3F, 0xFF, 0xC0, 0x00, 0x00,
0x00, 0x83, 0xF8, 0x3E, 0x3D, 0xF0, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x38, 0xF8, 0x00, 0x00,
0x0F, 0x80, 0x00, 0x3C, 0x00, 0xFC, 0x00, 0x00, 0x3F, 0x9C, 0xC7, 0x3F, 0x00, 0x6E, 0x00, 0x00,
0x0F, 0x1C, 0xE7, 0x3D, 0x00, 0x03, 0x00, 0x00, 0x1F, 0x1D, 0xF3, 0x1E, 0x00, 0x03, 0x80, 0x00,
0x13, 0x01, 0x80, 0x02, 0x00, 0x01, 0x80, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x00,
0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0, 0x00, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x00,
0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00,
0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xC0,
0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0xE0, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x38,
0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x18, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x08,
0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x8C, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0xC4,
0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0xC4, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0xC6,
0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0xC6, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0xC4,
0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x8C, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x88,
0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCE, 0x18, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x38,
0x01, 0x80, 0x00, 0x00, 0x00, 0x01, 0xE0, 0x70, 0x01, 0x80, 0x00, 0x00, 0x00, 0x01, 0x80, 0xC0,
0x00, 0xC0, 0x00, 0x00, 0x00, 0x03, 0x07, 0x80, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x06, 0x3E, 0x00,
0x00, 0x60, 0x00, 0x00, 0x00, 0x07, 0xF0, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00,
0x00, 0x18, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00,
0x00, 0x07, 0x00, 0x00, 0x00, 0xE0, 0x00, 0x00, 0x00, 0x03, 0xC0, 0x00, 0x03, 0x80, 0x00, 0x00,
0x00, 0x00, 0xE0, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xFF, 0xFE, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

//Pusheen Artist static image
static const unsigned char PROGMEM pusheen_artist[] =
{
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
0x3E, 0x07, 0x80, 0x00, 0x00, 0x00, 0x00, 0x37, 0x0F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x33, 0xFC,
0xCF, 0x80, 0x00, 0x00, 0x00, 0x63, 0xF8, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x60, 0xF0, 0x41, 0xF8,
0x00, 0x00, 0x00, 0xC0, 0x00, 0x01, 0xFE, 0x00, 0x00, 0x0F, 0xC0, 0x00, 0x00, 0xCE, 0x00, 0x00,
0x0F, 0xDC, 0xC6, 0x00, 0x0F, 0x00, 0x00, 0x03, 0x9C, 0xCE, 0x7C, 0x0F, 0x80, 0x00, 0x0F, 0x8F,
0xEE, 0x1C, 0x01, 0x80, 0x00, 0x0F, 0x00, 0x60, 0x60, 0x00, 0xC0, 0x00, 0x03, 0x00, 0x00, 0x70,
0x00, 0xC0, 0x00, 0x03, 0x00, 0x00, 0x18, 0x00, 0xC0, 0x00, 0x02, 0x00, 0x00, 0x18, 0x00, 0xC0,
0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x06,
0x00, 0x00, 0x00, 0x00, 0x60, 0x07, 0xFA, 0x00, 0x00, 0x00, 0x00, 0x60, 0x06, 0xFA, 0x00, 0x00,
0x00, 0x00, 0xDC, 0x07, 0xFA, 0x00, 0x00, 0x00, 0x00, 0xDE, 0x06, 0xFB, 0x00, 0x00, 0x00, 0x00,
0xDE, 0x04, 0xCF, 0x00, 0x00, 0x00, 0x00, 0xDE, 0x04, 0xCD, 0x80, 0x00, 0x00, 0x01, 0xFE, 0x04,
0xE9, 0xC0, 0x00, 0x00, 0x03, 0xFE, 0x07, 0xF8, 0xE0, 0x00, 0x00, 0x03, 0xFC, 0x00, 0x03, 0xFE,
0x00, 0x00, 0x0F, 0xF8, 0x00, 0x0F, 0x7F, 0xC0, 0x00, 0x1F, 0xF0, 0x00, 0x7C, 0x01, 0xFF, 0xCF,
0xF8, 0x00, 0x38, 0x60, 0x00, 0xF7, 0xFF, 0xE0, 0x00, 0x7C, 0x70, 0x00, 0x03, 0x01, 0x80, 0x00,
0x7C, 0x38, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x7C, 0x1C, 0x00, 0x3C, 0x18, 0x0F, 0xC0, 0x6C, 0x07,
0x00, 0xF0, 0xFF, 0x8F, 0xFF, 0x6C, 0x03, 0x83, 0xC1, 0xCB, 0xC7, 0xFF, 0x6C, 0x01, 0xEF, 0x03,
0x1E, 0xF7, 0xFF, 0x3C, 0x10, 0x7C, 0x03, 0xD7, 0xF0, 0x00, 0x00, 0x3C, 0x10, 0x07, 0xFD, 0xD8,
0x00, 0x00, 0x2E, 0x00, 0x07, 0xE1, 0xD8, 0x00, 0x00, 0x3F, 0x00, 0x07, 0xE3, 0x98, 0x00, 0x00,
0x1F, 0x0F, 0x86, 0xC7, 0x38, 0x00, 0x00, 0x00, 0x3F, 0x86, 0x77, 0x70, 0x00, 0x00, 0x00, 0x7F,
0x86, 0xF7, 0xE0, 0x00, 0x00, 0x00, 0x7F, 0x03, 0xF1, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x03, 0xF3,
0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xE7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x00, 0x00
};

//Pusheen Scooter static image
static const unsigned char PROGMEM pusheen_scooter[] =
{
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00,
0x00, 0x00, 0x00, 0x66, 0x03, 0x80, 0x00, 0x00, 0x0C, 0xC3, 0x07, 0xC0, 0x00, 0x00, 0x0F, 0x83,
0xFC, 0xC0, 0x00, 0x00, 0x3B, 0x83, 0xF8, 0x40, 0x00, 0x00, 0x1F, 0x80, 0x00, 0x7C, 0x00, 0x00,
0x07, 0x38, 0x00, 0xFC, 0x00, 0x00, 0x06, 0x39, 0x80, 0xFC, 0x00, 0x00, 0x0C, 0x03, 0xCC, 0x7C,
0x00, 0x00, 0x08, 0x00, 0xCC, 0x47, 0x80, 0x00, 0x18, 0x00, 0x00, 0x07, 0xC0, 0x00, 0x18, 0x00,
0x00, 0x07, 0xF0, 0x00, 0x10, 0x00, 0x00, 0x00, 0x7C, 0x00, 0x30, 0x00, 0x00, 0x00, 0x7C, 0x00,
0x30, 0x00, 0x00, 0x00, 0x06, 0x00, 0x30, 0x00, 0x00, 0x00, 0x02, 0x00, 0x30, 0x00, 0x00, 0x00,
0x03, 0x00, 0x30, 0x00, 0x00, 0x00, 0x03, 0x00, 0x30, 0x00, 0x00, 0x00, 0x01, 0x00, 0x30, 0x00,
0x00, 0x00, 0x01, 0x00, 0x30, 0x00, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00, 0x01, 0x00,
0x18, 0x00, 0x00, 0x00, 0x01, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x03, 0x00, 0x0F, 0x00, 0x00, 0x00,
0x03, 0x00, 0x0F, 0xF8, 0x00, 0x00, 0x02, 0x00, 0x0E, 0x78, 0x00, 0x00, 0x03, 0xFC, 0x03, 0xFF,
0x00, 0x00, 0x07, 0xFE, 0x01, 0xFB, 0xC1, 0xC0, 0x1F, 0xFE, 0x01, 0x60, 0x79, 0xE0, 0x7B, 0xF8,
0x03, 0x60, 0x1F, 0xFF, 0xE0, 0x00, 0x03, 0x20, 0x1F, 0xFF, 0x80, 0x00, 0x02, 0x30, 0x1F, 0xFF,
0xC0, 0x00, 0x02, 0x30, 0x1F, 0xFF, 0x80, 0x00, 0x1E, 0x18, 0x18, 0x01, 0x80, 0x1E, 0x38, 0x0C,
0x30, 0x01, 0x86, 0x3E, 0x6F, 0xEC, 0x30, 0xF8, 0xCF, 0x3A, 0x5C, 0x64, 0x61, 0xDC, 0xDF, 0xBE,
0x7F, 0xFF, 0xE7, 0xFE, 0x5F, 0x9E, 0x7C, 0x78, 0x07, 0x8F, 0x4F, 0x00, 0x7B, 0xAF, 0xFF, 0x77,
0xCE, 0x00, 0x1B, 0xA5, 0xFB, 0x75, 0xC0, 0x00, 0x19, 0x20, 0x03, 0x24, 0x00, 0x00, 0x0C, 0x60,
0x01, 0x8C, 0x00, 0x00, 0x07, 0xC0, 0x00, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

//Pusheen Cookie static image
static const unsigned char PROGMEM pusheen_cookie[] =
{
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x0C, 0x00,
0x00, 0x00, 0x00, 0x00, 0x33, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x21, 0x80, 0x73, 0x00,
0x00, 0x00, 0x00, 0x00, 0x60, 0xC0, 0xE3, 0x00, 0x00, 0x00, 0x00, 0x30, 0x40, 0xFF, 0x81, 0x00,
0x00, 0x00, 0x00, 0x1E, 0xC0, 0x6F, 0x81, 0x00, 0x00, 0x00, 0x00, 0x07, 0x80, 0x6D, 0x01, 0x9E,
0x00, 0x00, 0x00, 0x03, 0x80, 0x00, 0x01, 0xF0, 0x00, 0x00, 0x00, 0xF6, 0x80, 0x00, 0x01, 0xC0,
0x00, 0x00, 0x00, 0x1F, 0x07, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x3B, 0x07, 0x04, 0x3C, 0xFF,
0x00, 0x00, 0x00, 0x70, 0x07, 0x0C, 0x3C, 0xC0, 0x00, 0x00, 0x00, 0xC0, 0x00, 0x7F, 0x18, 0x40,
0x00, 0x00, 0x03, 0x80, 0x00, 0xE1, 0xC0, 0x40, 0x00, 0x00, 0x07, 0x00, 0x01, 0xC3, 0x60, 0x60,
0x00, 0x00, 0x0C, 0x00, 0x03, 0x83, 0x30, 0x60, 0x00, 0x00, 0x18, 0x00, 0x03, 0x32, 0x30, 0x60,
0x00, 0x00, 0x30, 0x00, 0x01, 0x30, 0x30, 0x20, 0x00, 0x00, 0x60, 0x00, 0x01, 0x87, 0x30, 0x20,
0x00, 0x00, 0xC0, 0x00, 0x01, 0x87, 0x70, 0x20, 0x00, 0x01, 0x80, 0x00, 0x00, 0xF7, 0xF0, 0x20,
0x00, 0x03, 0x00, 0x00, 0x00, 0xFF, 0xD0, 0x30, 0x00, 0x03, 0x00, 0x00, 0x01, 0xB0, 0x00, 0x30,
0x00, 0x06, 0x00, 0x00, 0x00, 0x60, 0x00, 0x30, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30,
0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20,
0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20,
0x00, 0x08, 0x00, 0x3F, 0xF8, 0x00, 0x00, 0x60, 0x00, 0x08, 0x00, 0xE7, 0xBE, 0x00, 0x00, 0x60,
0x00, 0x08, 0x01, 0xBF, 0xF7, 0x00, 0x00, 0x60, 0x00, 0x08, 0x03, 0x60, 0x1D, 0xE0, 0x00, 0x40,
0x00, 0x0C, 0x03, 0xC0, 0x06, 0xF0, 0x00, 0xC0, 0x00, 0x04, 0x07, 0x80, 0x00, 0x10, 0x00, 0xC0,
0x1F, 0xFE, 0x07, 0x80, 0x00, 0x90, 0x01, 0x80, 0x7E, 0x6E, 0x07, 0x00, 0x00, 0x90, 0x01, 0xC0,
0xF7, 0xFF, 0xFF, 0x00, 0x00, 0x9F, 0xFF, 0xC0, 0x7F, 0xE0, 0x05, 0x00, 0x00, 0x9F, 0xFF, 0xC0,
0x00, 0x00, 0x05, 0x00, 0x00, 0x97, 0x80, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x9F, 0x80, 0x00,
0x00, 0x00, 0x07, 0x80, 0x00, 0xBD, 0xFF, 0xFC, 0x00, 0x00, 0x03, 0x80, 0x00, 0xB3, 0x80, 0x0C,
0x00, 0x00, 0x03, 0xC0, 0x06, 0x31, 0xFF, 0xFC, 0x00, 0x00, 0x03, 0x77, 0xBD, 0xE3, 0x30, 0x00,
0x00, 0x00, 0x01, 0xDF, 0xFF, 0x03, 0x70, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xF8, 0x00, 0x00, 0x00
};

//Pusheen Marshmallow static image
static const unsigned char PROGMEM pusheen_marshmallow[] =
{
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x03, 0x80, 0x00, 0x00, 0x00, 0x1E, 0x07, 0xC7,
0xF0, 0x00, 0x00, 0x36, 0x0C, 0xDF, 0xF8, 0x00, 0x00, 0x33, 0x7C, 0x79, 0xDE, 0x00, 0x00, 0x63,
0xF8, 0x63, 0xDF, 0x00, 0x00, 0x41, 0xF0, 0x1E, 0x1F, 0x00, 0x0F, 0xC0, 0x00, 0x3C, 0x1D, 0x80,
0x1F, 0xC0, 0x00, 0x30, 0x01, 0xC0, 0x00, 0xD8, 0x01, 0xBE, 0x00, 0xC0, 0x0F, 0x9E, 0x67, 0x00,
0x00, 0x60, 0x1D, 0x8E, 0x70, 0x00, 0x00, 0x60, 0x01, 0x80, 0x60, 0x00, 0x00, 0x60, 0x01, 0x80,
0x00, 0x00, 0x00, 0x20, 0x01, 0x00, 0x00, 0x00, 0x00, 0x30, 0x01, 0x00, 0x00, 0x00, 0x00, 0x30,
0x01, 0x00, 0x00, 0x00, 0x00, 0x30, 0x01, 0x80, 0x00, 0x00, 0x00, 0x30, 0x0F, 0xC0, 0x00, 0x00,
0x00, 0x38, 0x0C, 0xE0, 0x00, 0x00, 0x00, 0x3C, 0x18, 0x38, 0x00, 0x00, 0x02, 0x66, 0x30, 0x7E,
0x00, 0x00, 0x0F, 0xE6, 0x30, 0x67, 0x80, 0x00, 0x1F, 0xC2, 0x20, 0x21, 0xF8, 0x00, 0x7F, 0x83,
0x60, 0x30, 0x3F, 0xFF, 0xEE, 0x03, 0x60, 0x30, 0x03, 0xFF, 0xFC, 0x03, 0x40, 0x30, 0x00, 0x00,
0x78, 0x01, 0x40, 0x30, 0x00, 0x00, 0x70, 0x01, 0xC0, 0x10, 0x00, 0x00, 0x70, 0x01, 0xC0, 0x10,
0x00, 0x00, 0x60, 0x01, 0xC0, 0x10, 0x00, 0x00, 0x00, 0x01, 0xC0, 0x10, 0x00, 0x00, 0x00, 0x01,
0xC0, 0x10, 0x00, 0x00, 0x00, 0x01, 0x80, 0x10, 0x00, 0x00, 0x00, 0x01, 0x80, 0x10, 0x00, 0x00,
0x00, 0x01, 0x80, 0x10, 0x00, 0x00, 0x00, 0x01, 0xC0, 0x30, 0x00, 0x00, 0x00, 0x01, 0xC0, 0x30,
0x00, 0x00, 0x00, 0x01, 0xC0, 0x30, 0x00, 0x00, 0x00, 0x01, 0xC0, 0x20, 0x00, 0x00, 0x00, 0x03,
0x40, 0x60, 0x00, 0x00, 0x00, 0x03, 0x60, 0x60, 0x00, 0x00, 0x00, 0x03, 0x60, 0xC0, 0x00, 0x00,
0x00, 0x02, 0x30, 0xC0, 0x00, 0x00, 0x00, 0x06, 0x39, 0x80, 0x00, 0x00, 0x00, 0x06, 0x1C, 0x00,
0x00, 0x00, 0x00, 0x06, 0x0F, 0xFC, 0x00, 0x00, 0x00, 0x1C, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8
};




// Initialize the OLED display using SPI
// D5 -> CLK
// D7 -> MOSI (DOUT)
// D0 -> RES
// D2 -> DC
// D8 -> CS
// SSD1306Spi        display(D0, D2, D8);
// or
// SH1106Spi         display(D0, D2);

// Initialize the OLED display using brzo_i2c
// D3 (4) -> SDA
// D5 (5) -> SCL
// SSD1306Brzo display(0x3c, D3, D5);
// or
// SH1106Brzo  display(0x3c, D3, D5);

// Initialize the OLED display using Wire library
// SH1106 display(0x3c, D3, D5);



#define DEMO_DURATION 3000
typedef void (*Demo)(void);

int demoMode = 0;
int counter = 1;

void printMonth(int month)
{
  switch(month)
  {
    case 1: display.print("Jan ");break;
    case 2: display.drawString(40, 10,"Feb ");break;
    case 3: display.print("Mar ");break;
    case 4: display.print("Apr ");break;
    case 5: display.print("May ");break;
    case 6: display.print("Jun ");break;
    case 7: display.print("Jul ");break;
    case 8: display.print("Aug ");break;
    case 9: display.print("Sep ");break;
    case 10: display.print("Oct ");break;
    case 11: display.print("Nov ");break;
    case 12: display.print("Dec ");break;
    default: display.print("--- ");break; //Display dashes if error - avoids scrambling display
  } 
}


//Function to display day-of-week string from numerical day-of-week argument
void printDay(int day)
{
  switch(day)
  {
    case 1: display.print("Mon ");break;
    case 2: display.drawString(40, 10,"Tue ");break;
    case 3: display.print("Wed ");break;
    case 4: display.print("Thu ");break;
    case 5: display.print("Fri ");break;
    case 6: display.print("Sat ");break;
    case 7: display.print("Sun ");break;
    default: display.print("--- ");break; //Display dashes if error - avoids scrambling display
  } 
  display.display();
  delay(500);
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  pinMode(4, INPUT);
  digitalWrite(4, HIGH); // ENABLE PULLUPS

  pinMode(13, INPUT);
  digitalWrite(13, HIGH);

  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);

  // Initialising the UI will init the display too.
  display.init();

  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);

}

void drawFontFaceDemo() {
    // Font Demo1
    // create more fonts at http://oleddisplay.squix.ch/
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 0, "Hello world");
    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 10, "Hello world");
    display.setFont(ArialMT_Plain_24);
    display.drawString(0, 26, "Hello world");
}

void drawTextFlowDemo() {
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawStringMaxWidth(0, 0, 128,
      "Lorem ipsum\n dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore." );
}

void drawTextAlignmentDemo() {
    // Text alignment demo
  display.setFont(ArialMT_Plain_10);

  // The coordinates define the left starting point of the text
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 10, "Left aligned (0,10)");

  // The coordinates define the center of the text
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 22, "Center aligned (64,22)");

  // The coordinates define the right end of the text
  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.drawString(128, 33, "Right aligned (128,33)");
}

void drawRectDemo() {
      // Draw a pixel at given position
    for (int i = 0; i < 10; i++) {
      display.setPixel(i, i);
      display.setPixel(10 - i, i);
    }
    display.drawRect(12, 12, 20, 20);

    // Fill the rectangle
    display.fillRect(14, 14, 17, 17);

    // Draw a line horizontally
    display.drawHorizontalLine(0, 40, 20);

    // Draw a line horizontally
    display.drawVerticalLine(40, 0, 20);
}

void drawCircleDemo() {
  for (int i=1; i < 8; i++) {
    display.setColor(WHITE);
    display.drawCircle(32, 32, i*3);
    if (i % 2 == 0) {
      display.setColor(BLACK);
    }
    display.fillCircle(96, 32, 32 - i* 3);
  }
}

void drawProgressBarDemo() {
  int progress = (counter / 5) % 100;
  // draw the progress bar
  display.drawProgressBar(0, 32, 120, 10, progress);

  // draw the percentage as String
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 15, String(progress) + "%");
}

void drawImageDemo() {
    // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
    // on how to create xbm files
    display.drawXbm(34, 14, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
}

Demo demos[] = {drawFontFaceDemo, drawTextFlowDemo, drawTextAlignmentDemo, drawRectDemo, drawCircleDemo, drawProgressBarDemo, drawImageDemo};
int demoLength = (sizeof(demos) / sizeof(Demo));
long timeSinceLastModeSwitch = 0;

void loop() {
  // clear the display
  display.clear();

  char tempF[6] = "23 @C";
  unsigned long now = millis();

  Serial.println(mode);

  switch(mode)
  {
    case 0: break;
    case 1: //Day-of-week setting
      break;
    case 2: //Month setting
      display.setFont(ArialMT_Plain_10);
      display.drawString(0, 10, tempF);
      // write the buffer to the display
      display.display();
      break;
    case 6: //Date setting
      display.setFont(ArialMT_Plain_10);
      display.drawString(20, 20, String(mode));
      // write the buffer to the display
      delay(5000);
      break;
  }
  /*

  if(mode <= 4){
  //display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 10, tempF);
  // write the buffer to the display
  display.display();
  delay(1000);
  printDay(2);
  printMonth(9);
  }  
    
  if (mode >= 4){
  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.drawString(20, 20, String(mode));
  // write the buffer to the display
  display.display();
    
  }


   if(!digitalRead(4)){ //Reset alarm flag if set button pressed
   delay(25); //25ms debounce time
   if(!digitalRead(4)){
      mode += 1;
      }
   }*/
  
  /*
  
  // draw the current demo method
  demos[demoMode]();

  display.setTextAlignment(TEXT_ALIGN_RIGHT);


  if (millis() - timeSinceLastModeSwitch > DEMO_DURATION) {
    demoMode = (demoMode + 1)  % demoLength;
    timeSinceLastModeSwitch = millis();
  }
  counter++;
  delay(10);
  */
}
