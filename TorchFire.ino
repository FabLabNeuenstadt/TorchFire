#include <FastLED.h>

#define NUM_COLS 5
#define NUM_ROWS 10

#define MAX_IGNITE_HIGH 4

#define LED_PIN     5
#define COLOR_ORDER GRB
#define CHIPSET     WS2811
#define NUM_LEDS    NUM_COLS*NUM_ROWS

#define BRIGHTNESS  200
#define FRAMES_PER_SECOND 60

bool gReverseDirection = false;

CRGB leds[NUM_LEDS];

void setup() {
  delay(3000); // sanity delay
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );
}

void loop()
{
  // Add entropy to random number generator; we use a lot of it.
  // random16_add_entropy( random());

  Fire2012(); // run simulation frame
  
  FastLED.show(); // display this frame
  FastLED.delay(1000 / FRAMES_PER_SECOND);
}

// There are two main parameters you can play with to control the look and
// feel of your fire: COOLING (used in step 1 above), and SPARKING (used
// in step 3 above).
//
// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 50, suggested range 20-100 
#define COOLING  55

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 120

// returns the idx in the flat heat-array based on the given column and row.
inline unsigned int GetHeatIdx(int col, int row)
{
  // LEDs are arranged from bottom to the top row by row.
  // (therefore columns and rows are interchanged)
  return row*NUM_COLS + col;
}

void Fire2012()
{
  // Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
  for( int i = 0; i < NUM_LEDS; i++) {
    heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
  }
  
  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for(int col = 0; col < NUM_COLS; col++) {
    for( int row = NUM_ROWS - 1; row >= 2; row--) {
      heat[GetHeatIdx(col, row)] = (heat[GetHeatIdx(col, row - 1)] + heat[GetHeatIdx(col, row - 2)] + heat[GetHeatIdx(col, row - 2)]) / 3;
    }
  }
  
  // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
  for(int col = 0; col < NUM_COLS; col++) {
    if( random8() < SPARKING ) {
      int y = GetHeatIdx(col, random8(MAX_IGNITE_HIGH));
      heat[y] = qadd8( heat[y], random8(160,255) );
    }
  }

  // Step 4.  Map from heat cells to LED colors
  for( int j = 0; j < NUM_LEDS; j++) {
    CRGB color = HeatColor( heat[j]);
    int pixelnumber;
    if( gReverseDirection ) {
      pixelnumber = (NUM_LEDS-1) - j;
    } else {
      pixelnumber = j;
    }
    leds[pixelnumber] = color;
  }
}

