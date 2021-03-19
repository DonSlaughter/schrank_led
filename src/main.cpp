#include "Arduino.h"
#include "FastLED.h"

#define NUM_LEDS 16
#define CHIPSET WS2812B
CRGB leds[NUM_LEDS];

// Prototypes of functions, implementation at the end
void lights_on();
// turns the light smoth on
void lights_off();
// turns the light smoth off

void setup(){
    FastLED.addLeds<CHIPSET, 2>(leds, NUM_LEDS);
}

void loop(){
    lights_on();
    delay(1000);
    lights_off();
    delay(1000);
}

void lights_on(){
    for (int i = 0; i <= 255; i ++){
	FastLED.setBrightness(i);
    	fill_solid(leds, NUM_LEDS, CRGB::Blue);
    	FastLED.show();
	delay (5);
    }
}
void lights_off(){
    for (int i = 255; i >= 0; i --){
	FastLED.setBrightness(i);
    	fill_solid(leds, NUM_LEDS, CRGB::Blue);
    	FastLED.show();
	delay (5);
    }
}
