/*
  Copyright (c) 2021 Mehmet Ulrich  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "Arduino.h"
#include "FastLED.h"

#define NUM_LEDS 16
#define CHIPSET WS2812B
CRGB leds[NUM_LEDS];

#define PIR_PIN 4
#define LED_PIN 2
#define PHOTO_PIN A0

// Prototypes of functions, implementation at the end
// turns the light smoth on
uint8_t lights_on(uint8_t);
// turns the light smoth off, but exits if new movement is detected
uint8_t lights_off(uint8_t);
// returns true if the PIR Sensor detects a movement
bool movement_detected();
// returns true if the photo resistor goes below light_threshold
bool light_intensity_check(uint16_t);
uint16_t light_threshold = 500;

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
	if (movement_detected() && light_intensity_check(light_threshold) ) {
		led_intensity = lights_on(led_intensity);
		movement_timer = millis();
		countdown = millis();
		while (countdown <= movement_timer + active_time){
			if (movement_detected() == true){
				Serial.println("New movement detected, reseting timer");
				movement_timer = millis();
			}
			countdown = millis();
		}
		led_intensity = lights_off(led_intensity);
	}
}

uint8_t lights_on(uint8_t){
	Serial.println("Turning light on");
	while (led_intensity < 255){
		led_intensity++ ;
		FastLED.setBrightness(led_intensity);
		fill_solid(leds, NUM_LEDS, CRGB::Blue);
		FastLED.show();
		delay (10);
	}
	Serial.println("Lights on");
	return led_intensity;
}

uint8_t lights_off(uint8_t){
	Serial.println("Turning light off");
	while (led_intensity > 0){
		led_intensity-- ;
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
	//the PIR Sensor sends the HIGH signal around 4 Seconds, its not a short peak
	//4 Seconds is is smallest time frame to get from my sensor.
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

bool light_intensity_check(uint16_t){
	uint16_t photo_resistance = analogRead(PHOTO_PIN);
	Serial.println(photo_resistance);
	if (photo_resistance < light_threshold){
		return true;
	}
	else {
		return false;
	}
}
