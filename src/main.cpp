#include "Arduino.h"
#include "FastLED.h"

#define NUM_LEDS 16
#define CHIPSET WS2812B
CRGB leds[NUM_LEDS];

#define PIR_PIN 4
#define LED_PIN 2
int PHOTO_PIN = A0;

// Prototypes of functions, implementation at the end
uint8_t lights_on(uint8_t led_intensity);
// turns the light smoth on
uint8_t lights_off(uint8_t led_intensity);
// turns the light smoth off
bool movement_detected();
// returns true if the PIR Sensor detects a movement
uint8_t check_light_intensity();
uint8_t light_intensity;

//global variables
unsigned long movement_timer, countdown;
// how long should the light be active
uint16_t active_time = 10000;
uint8_t led_intensity = 0;

void setup(){
	Serial.begin(9600);
	pinMode(PIR_PIN, INPUT);
	FastLED.addLeds<CHIPSET, LED_PIN>(leds, NUM_LEDS);
	FastLED.setBrightness(0);
	FastLED.show();
}

void loop(){
	light_intensity = check_light_intensity();
	if (movement_detected() == true && light_intensity < 100) {
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

uint8_t lights_on(uint8_t led_intensity){
	Serial.println("Turning light on");
	while (led_intensity < 255){
		led_intensity = ++led_intensity;
		FastLED.setBrightness(led_intensity);
		fill_solid(leds, NUM_LEDS, CRGB::Blue);
		FastLED.show();
		delay (10);
	}
	Serial.println("Lights on");
	return led_intensity;
}

uint8_t lights_off(uint8_t led_intensity){
	Serial.println("Turning light off");
	while (led_intensity > 0){
		led_intensity = --led_intensity;
		FastLED.setBrightness(led_intensity);
		fill_solid(leds, NUM_LEDS, CRGB::Blue);
		FastLED.show();
		delay (10);
		if (movement_detected()) {
			return led_intensity;
			break;	//break out from turning off if new movement is detected
		}
	}
	Serial.println("Lights off");
	return 0;
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

uint8_t check_light_intensity(){
	uint8_t photo_resistance = analogRead(PHOTO_PIN);
	//Serial.println(photo_resistance);
	return photo_resistance;
}
