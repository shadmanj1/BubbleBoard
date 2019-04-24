// NeoPixel test program showing use of the WHITE channel for RGBW
// pixels only (won't look correct on regular RGB NeoPixel strips).

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN_LONG     8
#define LED_PIN_SHORT     13

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT_LONG  60
#define LED_COUNT_SHORT  55

// NeoPixel brightness, 0 (min) to 255 (max)
#define BRIGHTNESS 255

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip_long(LED_COUNT_LONG, LED_PIN_LONG, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel strip_short(LED_COUNT_SHORT, LED_PIN_SHORT, NEO_GRBW + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

void setup() {
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  strip_long.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip_long.show();            // Turn OFF all pixels ASAP
  strip_short.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip_short.show();            // Turn OFF all pixels ASAP
  strip_long.setBrightness(BRIGHTNESS); // Set BRIGHTNESS to about 1/5 (max = 255)
  strip_short.setBrightness(BRIGHTNESS); // Set BRIGHTNESS to about 1/5 (max = 255)

 
}


void loop() {
  // Fill along the length of the strip in various colors...

  //START CASEY'S CODE
  //Get next color for strips (right now it is random)
    //int red = random(0, 255);
    //int green = random(0, 255);
    //int blue = random(0, 255);
    //int white = random(0, 50);

  //blue 0, 39, 76
   uint32_t blue_short = strip_short.Color(0, 39, 76, 0);
   uint32_t blue_long = strip_long.Color(0, 39, 76, 0);
   
  //Maize 255, 203, 5 
   uint32_t maize_short = strip_short.Color(255, 203, 5, 0);
   uint32_t maize_long = strip_long.Color(255, 203, 5, 0);
    
  //set all pixels on both strips to that color
  //for (int n = 0; n <= LED_COUNT_LONG; n++){
    //strip_long.setPixelColor(n, red, green, blue, white);
    //strip_long.setPixelColor(n, blue_long);

    //strip_short.setPixelColor(n, red, green, blue, white);
    //strip_short.setPixelColor(n, blue_short);
    
    //send the color change to the strips (push twice might help?)
    //strip_short.show();
    //strip_long.show();

    //delay(25);
  //}

 

    
    //delay(3000);

    for (int n = 0; n <= LED_COUNT_LONG; n++){
    
      strip_long.setPixelColor(n, blue_long);
      strip_short.setPixelColor(n, maize_short);

      strip_short.show();
      strip_long.show();

      delay(500);
  }

//delay(500);
    
  //delay(1000);
}


