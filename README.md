# LED Pair Fade Chase – ESP32 + 74HC595

This project demonstrates how to control **8 LEDs** using an **ESP32 WROVER** and a **74HC595 shift register**, generating a smooth **fade-and-chase animation** in **LED pairs**:

* Pair 0 → **Q0 & Q1**
* Pair 1 → **Q2 & Q3**
* Pair 2 → **Q4 & Q5**
* Pair 3 → **Q6 & Q7**

The animation fades each pair **in and out from left to right**, then fades backward **without repeating the last pair**, creating a clean reversible sequence.

This project uses **software PWM** to simulate brightness on outputs that normally support only ON/OFF digital states.

---

## ✨ Features

* Controls **8 LEDs** using only **3 GPIO pins** on the ESP32
* Clean abstraction for updating the shift register
* Smooth software PWM fading
* Forward and reverse chase animation
* LED pairs fade together (2 LEDs lit at once)
* No repetition when switching directions

---

## 🧰 Hardware Requirements

* **ESP32 WROVER module** (or similar ESP32 dev board)
* **74HC595** 8-bit shift register
* **8 LEDs**
* **8 resistors** (220–470 Ω recommended)
* Breadboard and jumper wires

---

## 🔌 Wiring

### ESP32 → 74HC595

| ESP32 Pin | 74HC595 Pin | Function                             |
| --------- | ----------- | ------------------------------------ |
| GPIO 32   | DS (14)     | Serial Data                          |
| GPIO 25   | SH_CP (11)  | Shift Clock                          |
| GPIO 26   | ST_CP (12)  | Latch Clock                          |
| 3.3V      | VCC (16)    | Power                                |
| GND       | GND (8)     | Ground                               |
| 3.3V      | MR (10)     | Reset (active low, keep HIGH)        |
| GND       | OE (13)     | Output Enable (active low, keep LOW) |

### 74HC595 → LEDs

Each output **Q0–Q7** drives one LED:

```
Qn → resistor → LED anode  
LED cathode → GND
```

---

## 🧠 How It Works

### Shift Register Control

The 74HC595 provides 8 parallel outputs but only requires 3 microcontroller pins:

* **Data** (DS)
* **Clock** (SH_CP)
* **Latch** (ST_CP)

`shiftOut()` sends one byte at a time, and the latch stores the updated LED pattern.

### Pair Masking

Each pair of LEDs is represented using a bitmask:

| Pair    | Bits       | Mask   |
| ------- | ---------- | ------ |
| Q0 & Q1 | `00000011` | `0x03` |
| Q2 & Q3 | `00001100` | `0x0C` |
| Q4 & Q5 | `00110000` | `0x30` |
| Q6 & Q7 | `11000000` | `0xC0` |

The animation activates one mask at a time.

### Software PWM Fading

Brightness is simulated by rapidly toggling LEDs ON and OFF.

Duty cycle = brightness.

Higher duty cycle = brighter LED.

---

## 📄 Example Code

```cpp
// See full code in main.cpp or sketch.ino
updateShiftRegister(mask);   // LED pair ON
updateShiftRegister(0x00);   // LEDs OFF
```

The animation runs left → right → left, fading each pair.

---

## ▶️ Animation Flow

1. Fade in Q0 & Q1
2. Fade in Q2 & Q3
3. Fade in Q4 & Q5
4. Fade in Q6 & Q7
5. Fade back down: Q4 & Q5 → Q2 & Q3 → Q0 & Q1
6. Repeat

Q6 & Q7 are **not repeated** when reversing direction.

---

## 🚀 Getting Started

### 1. Install Arduino IDE or PlatformIO

### 2. Install ESP32 board support

### 3. Upload the sketch

### 4. Power the ESP32 and enjoy the LED animation!

---

## 🛠 Customization

You can easily modify:

* Fade speed (`steps`, `pwmCycles`, `delay` values)
* LED grouping (edit the `pairs[]` array)
* Direction or special effects
* Add more 74HC595 chips for more LEDs

Ask if you'd like help adding **breathing**, **sparkle**, **color patterns**, or **sound-reactive** modes.

---

## 📜 License

MIT License – free to use and modify.
