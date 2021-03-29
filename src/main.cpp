#include "Arduino.h"
#include "FastLED.h"

#define NUM_LEDS 16
#define CHIPSET WS2812B
CRGB leds[NUM_LEDS];

#define PIR_PIN 4
#define LED_PIN 2

// Prototypes of functions, implementation at the end
int lights_on(int led_intensity);
// turns the light smoth on
int lights_off(int led_intensity);
// turns the light smoth off
bool movement_detected();
// returns true if the PIR Sensor detects a movement
int light_intensity();

//global variables
unsigned long movement_timer, countdown;
// how long should the light be active
int active_time = 1000;
int led_intensity = 0;

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
	if (movement_detected() == true) {
		led_intensity = lights_on(led_intensity);
		movement_timer = millis();
		countdown = millis();
		while (countdown <= movement_timer + active_time){
			if (movement_detected() == true){
				movement_timer = millis();
			}
			countdown = millis();
		}
		led_intensity = lights_off(led_intensity);
	}
}

int lights_on(int led_intensity){
	Serial.println("Turning light on");
	for (; led_intensity <= 255; led_intensity ++){
		Serial.println(led_intensity);
		FastLED.setBrightness(led_intensity);
		fill_solid(leds, NUM_LEDS, CRGB::Blue);
		FastLED.show();
		delay (10);
	}
	Serial.println("Lights on");
	return led_intensity;
}

int lights_off(int led_intensity){
	Serial.println("Turning light off");
	for (; led_intensity >= 0; led_intensity --){
		Serial.println(led_intensity);
		if (movement_detected() == true) break;	//break out from turning off if new movement is detected
		FastLED.setBrightness(led_intensity);
		fill_solid(leds, NUM_LEDS, CRGB::Blue);
		FastLED.show();
		delay (10);
	}
	Serial.println("Lights off");
	return led_intensity;
}

bool movement_detected(){
	int movement;
	movement = digitalRead(PIR_PIN);
	if (movement == HIGH) {
		//Serial.println("Movement detected");
		return true;
	}
	else {
		return false;
	}
}

int light_intensity(){
	//TODO
}
