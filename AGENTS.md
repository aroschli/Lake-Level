# AGENTS.md

## Repository Overview

**Lake-Level** monitors lake water levels in East Tennessee. A GitHub Actions workflow scrapes elevation data from public websites every 30 minutes and stores the results in `data.json`. ESP32 Arduino sketches read that file over WiFi and display the elevation on a 4-digit 7-segment LED display.

## Repository Structure

```
Lake-Level/
├── .github/
│   └── workflows/
│       └── update-lake-levels.yml   # Scrapes lake levels every 30 minutes (Python)
├── FortLoudoun-ESP32/
│   ├── FortLoudoun-ESP32.ino        # Arduino/C++ sketch for Fort Loudoun Lake
│   └── README.md
├── WattsBar-ESP32/
│   ├── WattsBar-ESP32.ino           # Arduino/C++ sketch for Watts Bar Lake
│   └── README.md
├── test-7seg-backpack/
│   ├── test-7seg-backpack.ino       # Hardware test sketch for 7-segment display
│   └── README.md
├── CP210x_Universal_Windows_Driver.zip  # USB-to-UART driver for flashing ESP32
├── data.json                        # Latest lake level data (updated by CI)
├── .gitignore
├── LICENSE
└── README.md
```

## Languages and Technologies

- **Python** — GitHub Actions workflow script that scrapes and updates `data.json`. Uses `requests` and `beautifulsoup4`.
- **Arduino/C++** — ESP32 sketches that fetch `data.json` over WiFi and drive the display. Uses `WiFi.h`, `HTTPClient.h`, `ArduinoJson`, and the Adafruit LED Backpack / GFX libraries.

## Python Development Environment

The scraper runs inline inside `.github/workflows/update-lake-levels.yml`. If you need to develop or test it locally:

1. Create and activate a virtual environment:
   ```bash
   python -m venv .venv
   source .venv/bin/activate  # On Windows: .venv\Scripts\activate
   ```
2. Install dependencies:
   ```bash
   pip install requests beautifulsoup4
   ```

## Arduino/ESP32 Development

- Install the **Arduino IDE** and add the **Espressif ESP32** board package.
- Install these libraries via the Arduino Library Manager:
  - `Adafruit LED Backpack`
  - `Adafruit GFX Library`
  - `ArduinoJson`
- Set `WIFI_SSID` and `WIFI_PASSWORD` in the sketch before uploading.

## Agent Guidelines

- **Python style**: Follow PEP 8. Use `ruff` for linting if configured.
- **Secrets**: Never commit credentials, API keys, or `.env` files. The `.gitignore` already excludes them. ESP32 sketches use placeholder strings (`YOUR_WIFI_SSID`) — keep it that way.
- **data.json**: This file is written by CI. Do not hand-edit it for production. You may update it manually for local testing.
- **New lakes**: To add a lake, add an entry to `SOURCE_CANDIDATES` and `SANE_RANGES` in the workflow, and add a corresponding key to `data.json`.
- **New sketch**: Mirror the existing `FortLoudoun-ESP32` or `WattsBar-ESP32` folder structure and update this file and the root `README.md`.
- **CI workflow**: The workflow is at `.github/workflows/update-lake-levels.yml`. It runs on a 30-minute schedule and on `workflow_dispatch`.

