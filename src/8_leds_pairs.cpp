// Control 8 LEDs with an ESP32 WROVER and a 74HC595 shift register.
// Fade LED pairs: (Q0&Q1), (Q2&Q3), (Q4&Q5), (Q6&Q7) forward, then back without repeating last pair.
// Author: Chris
// Date: 2025-12-07

#include <Arduino.h>

int latchPin = 26; // ST_CP of 74HC595
int clockPin = 25; // SH_CP of 74HC595
int dataPin  = 32; // DS of 74HC595

// --- function declarations ---
void updateShiftRegister(byte leds);
void fadePair(byte pairMask);

// --- setup ---
void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin,  OUTPUT);

  updateShiftRegister(0); // all LEDs off
}

// --- main loop ---
void loop() {
  // Define the LED pairs using bitmasks
  // Pair 0: Q0 & Q1 -> 00000011 = 0x03
  // Pair 1: Q2 & Q3 -> 00001100 = 0x0C
  // Pair 2: Q4 & Q5 -> 00110000 = 0x30
  // Pair 3: Q6 & Q7 -> 11000000 = 0xC0
  const byte pairs[] = {
    0b00000011,
    0b00001100,
    0b00110000,
    0b11000000
  };

  const int numPairs = sizeof(pairs) / sizeof(pairs[0]);

  // Forward: 0 → 3
  for (int i = 0; i < numPairs; i++) {
    fadePair(pairs[i]);
  }

  // Backward: 2 → 0  (so we don't repeat the last pair)
  for (int i = numPairs - 2; i >= 0; i--) {
    fadePair(pairs[i]);
  }
}

// --- update the 74HC595 outputs ---
void updateShiftRegister(byte leds) {
  digitalWrite(latchPin, LOW);                 // prepare to send
  shiftOut(dataPin, clockPin, MSBFIRST, leds); // send 8 bits
  digitalWrite(latchPin, HIGH);                // latch outputs
}

// --- fade a pair of LEDs in and out using software PWM ---
void fadePair(byte pairMask) {
  const int steps          = 100;   // how many brightness levels (more = smoother, slower)
  const int pwmCycles      = 80;   // PWM slices per step (more = smoother, less flicker)
  const int pwmStepDelayUs = 200;  // delay per slice in microseconds

  // Fade in
  for (int level = 0; level <= steps; level++) {
    int onCycles = map(level, 0, steps, 0, pwmCycles);

    for (int c = 0; c < pwmCycles; c++) {
      if (c < onCycles) {
        updateShiftRegister(pairMask); // pair ON
      } else {
        updateShiftRegister(0x00);     // all OFF
      }
      delayMicroseconds(pwmStepDelayUs);
    }
  }

  // Fade out
  for (int level = steps; level >= 0; level--) {
    int onCycles = map(level, 0, steps, 0, pwmCycles);

    for (int c = 0; c < pwmCycles; c++) {
      if (c < onCycles) {
        updateShiftRegister(pairMask); // pair ON
      } else {
        updateShiftRegister(0x00);     // all OFF
      }
      delayMicroseconds(pwmStepDelayUs);
    }
  }
}

