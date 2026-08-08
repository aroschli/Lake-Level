# test-7seg-backpack

Simple Arduino/ESP32 test sketch for an Adafruit 4-digit 7-segment I2C backpack display.

## What it does

The sketch in `test-7seg-backpack.ino`:
- Initializes I2C on ESP32 pins SDA `21` and SCL `22`
- Connects to the display at I2C address `0x70`
- Counts from `0` to `9999`, updating once per second

## Hardware

- ESP32 board
- Adafruit LED Backpack 7-segment display (HT16K33-based)
- Jumper wires

## Wiring

- ESP32 `3V3` -> Display `VCC`
- ESP32 `GND` -> Display `GND`
- ESP32 `GPIO 21` -> Display `SDA`
- ESP32 `GPIO 22` -> Display `SCL`

## Required libraries

Install these Arduino libraries:
- `Adafruit GFX Library`
- `Adafruit LED Backpack Library`

(`Wire` is included with the Arduino core.)

## Usage

1. Open `test-7seg-backpack.ino` in the Arduino IDE.
2. Select your ESP32 board and serial/USB port.
3. Install the required libraries if prompted.
4. Upload the sketch.
5. Confirm the display increments once per second.
