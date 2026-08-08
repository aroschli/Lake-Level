# Lake-Level
Lake level monitoring for East TN

This repository collects lake level data from public websites, stores it in `data.json`, and provides ESP32 Arduino sketches that read that data and display it on a 7-segment display.

## How it works

A GitHub Actions workflow (`update-lake-levels.yml`) runs every 30 minutes. It uses a Python script to scrape lake elevation data from public sources and writes the results to `data.json` at the root of the repository.

The workflow monitors two lakes:
- **Watts Bar Lake**
- **Fort Loudoun Lake**

The stored data for each lake is the water surface elevation in feet above mean sea level (MSL).

## data.json format

```json
{
  "watts_bar_lake": {
    "elevation_ft": 740.79,
    "last_updated": "2026-08-08T21:36:01Z",
    "source_used": "https://www.wattsbar.info/Level/",
    "source_timestamp": "2026-08-08T21:36:01Z"
  },
  "fort_loudoun_lake": {
    "elevation_ft": 812.84,
    "last_updated": "2026-08-08T21:36:01Z",
    "source_used": "https://www.fortloudoun.info/Level/",
    "source_timestamp": "2026-08-08T21:36:01Z"
  }
}
```

## Repository structure

```
Lake-Level/
├── .github/
│   └── workflows/
│       └── update-lake-levels.yml   # Scrapes lake levels every 30 minutes
├── FortLoudoun-ESP32/
│   ├── FortLoudoun-ESP32.ino        # ESP32 sketch for Fort Loudoun Lake display
│   └── README.md
├── WattsBar-ESP32/
│   ├── WattsBar-ESP32.ino           # ESP32 sketch for Watts Bar Lake display
│   └── README.md
├── test-7seg-backpack/
│   ├── test-7seg-backpack.ino       # Test sketch for the 7-segment display
│   └── README.md
├── CP210x_Universal_Windows_Driver.zip  # USB-to-UART driver for ESP32 programming
├── data.json                        # Latest scraped lake level data
├── .gitignore
├── LICENSE
└── README.md
```

## ESP32 display sketches

Each sketch connects an ESP32 to an Adafruit 4-digit 7-segment backpack (HT16K33, product 881) via I2C and displays the current lake elevation in `XXX.X` feet format. The sketches fetch `data.json` directly from this repository every 5 minutes.

See the individual `README.md` files in each sketch folder for hardware wiring and Arduino IDE setup instructions.

## Notes about Arduino Setup for ESP32

Windows likely requires the CP210x drivers to recognize the ESP32 module when connected via USB. Drivers are included as a zip folder in the repository. Simple connect the device, then manually update drivers from device manager and point to the folder where the zip file is stored.

Arduino needs the following libraries downloaded
* esp32 from Espressif Systems
* Adafruit LED Backpack
* Adafruit GFX Library
* ArduinoJson

Once drivers and libraries are installed, Arduino IDE can be connected to the ESP32 board via whichever COM port is being used. In Arduino, set the board type as ESP32 Dev Module
