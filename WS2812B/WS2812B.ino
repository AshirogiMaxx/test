// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>

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
void setup() {

  pixels.begin(); // This initializes the NeoPixel library.
  pinMode(BUTTON_LOW, INPUT_PULLUP);
  pinMode(BUTTON_MID, INPUT_PULLUP);
  pinMode(BUTTON_HIGH, INPUT_PULLUP);
  Serial.begin(9600); 
  
}

void loop() {

  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.
  
  bool newstate1 = digitalRead(BUTTON_LOW);
  bool newstate2 = digitalRead(BUTTON_MID);

  /*Serial.print("The status from newstate1 is: ");
  Serial.println(newstate1);
  Serial.print("The status from newstate2 is: ");
  Serial.println(newstate2);
  */
  if (newstate1 == LOW){
    for(int i=0;i<NUMPIXELS/2;i++){
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(70,70,70)); // Moderately bright green color.
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
