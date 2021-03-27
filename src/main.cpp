#include "Arduino.h"
#include "FastLED.h"

#define NUM_LEDS 16
#define CHIPSET WS2812B
CRGB leds[NUM_LEDS];

#define PIR_PIN 4
#define LED_PIN 2
int movement;
unsigned long time;
unsigned long movement_time;

// Prototypes of functions, implementation at the end
void lights_on();
// turns the light smoth on
void lights_off();
// turns the light smoth off

void setup(){
    Serial.begin(9600);
    pinMode(PIR_PIN, INPUT);
    FastLED.addLeds<CHIPSET, LED_PIN>(leds, NUM_LEDS);
    FastLED.setBrightness(0);
    FastLED.show();
}

void loop(){
    //TODO
    /*
    PIR for Movement
    Photodiode for Measuring the Light
    Calculating best Timeduration for the Lights, maybe in Comparrision to the light intensity
       */
    movement = digitalRead(PIR_PIN);
    if (movement == HIGH) {
	Serial.println("Movement Detected");
	time = millis();
	movement_time = millis();
	Serial.println("Turning Lights on");
	lights_on();
	Serial.println("Lights on");
	while (movement_time < time + 1000){
	    movement = digitalRead(PIR_PIN);
	    if (movement == HIGH) {
		movement_time = millis();
	    }
	    time = millis();
	    Serial.print("Movement: ");
	    Serial.println(movement);
	    Serial.print("Last movement: ");
	    Serial.println(movement_time);
	    Serial.print("Time: ");
	    Serial.println(time);
	}
	Serial.println("Turning Lights off");
	lights_off();
	Serial.println("Lights off");
    }
}

void lights_on(){
    for (int i = 0; i <= 255; i ++){
	FastLED.setBrightness(i);
    	fill_solid(leds, NUM_LEDS, CRGB::Blue);
    	FastLED.show();
	delay (10);
    }
}
void lights_off(){
    for (int i = 255; i >= 0; i --){
	FastLED.setBrightness(i);
    	fill_solid(leds, NUM_LEDS, CRGB::Blue);
    	FastLED.show();
	delay (10);
    }
}
