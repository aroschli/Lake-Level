# FortLoudoun-ESP32

Arduino sketch for a standard ESP32 + Adafruit 0.56" 4-digit 7-segment backpack display (product 881) to read Fort Loudoun Lake elevation from this repository's `data.json` and display it.

## Hardware

- ESP32 dev board
- Adafruit 7-segment backpack display (HT16K33, product 881)
- 4 jumper wires for I2C + power

## Wiring (ESP32 -> Display)

- `3V3` -> `+`
- `GND` -> `-`
- `GPIO 21 (SDA)` -> `D`
- `GPIO 22 (SCL)` -> `C`

## Arduino IDE setup

Install these libraries:

- Adafruit LED Backpack
- Adafruit GFX Library
- ArduinoJson
- esp32 library from Espressif Systems

(ESP32 core includes `WiFi.h` and `HTTPClient.h`.)

## Configure and upload

1. Open `FortLoudoun-ESP32.ino`.
2. Set:
   - `WIFI_SSID`
   - `WIFI_PASSWORD`
   - `DATA_URL` (if using a different branch/repo URL)
3. Upload to ESP32.

## Display format

The value is shown as `XXX.X` feet (e.g., `740.8`).

## Polling

Default refresh interval is 5 minutes (`POLL_INTERVAL_MS`).
