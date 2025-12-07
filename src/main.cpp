// Control 8 LEDs with an ESP32 WROVER and a 74HC595 shift register.
// Author: crixnuts
// Date: 2025-12-07

#include <Arduino.h>

int tDelay = 60;  // Delay time in milliseconds
int latchPin = 26; // Pin connected to ST_CP of 74HC595
int clockPin = 25; // Pin connected to SH_CP of 74HC595
int dataPin = 32;  // Pin connected to DS of 74HC595

// Function declarations:
void updateShiftRegister(byte leds);
void fadeAll();

void setup() {
  // put your setup code here, to run once:
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  updateShiftRegister(0); // Initialize all LEDs to off
}

void loop() {
  // Simple LED chase effect
  for (int j = 0; j < 23; j++) {
    for (int i = 0; i < 8; i++) {
      updateShiftRegister(1 << i);
      delay(tDelay);
    }
    
    for (int i = 6; i >= 0; i--) {
      updateShiftRegister(1 << i);
      delay(tDelay);
    }
  }
  
  // Fade all LEDs in and out
  for (int i = 0; i < 23; i++) {
    fadeAll();
    delay(tDelay * 10);
  }
 }

// put function definitions here:
void updateShiftRegister(byte leds) {
  digitalWrite(latchPin, LOW); // Prepare to send data
  shiftOut(dataPin, clockPin, LSBFIRST, leds); // Send data to shift register
  digitalWrite(latchPin, HIGH); // Update the outputs
}

void fadeAll() {
  for (int brightness = 0; brightness < 255; brightness++) {
    for (int i = 0; i < 255; i++) {
      updateShiftRegister(i < brightness ? 0xFF : 0x00);
    }
    delay(3);
  }

  // Fade out
  for (int brightness = 255; brightness >= 0; brightness--) {
    for (int i = 0; i < 255; i++) {
      updateShiftRegister(i < brightness ? 0xFF : 0x00);
    }
    delay(3);
  }
}
