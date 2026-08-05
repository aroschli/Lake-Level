#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

// -----------------------------
// User configuration
// -----------------------------
const char* WIFI_SSID = "YOUR_WIFI_SSID";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

// Replace with your raw JSON URL when hosted.
// Example (GitHub raw):
// https://raw.githubusercontent.com/<owner>/<repo>/<branch>/data.json
const char* DATA_URL = "https://raw.githubusercontent.com/aroschli/Lake-Level/main/data.json";

// Poll interval (milliseconds)
const unsigned long POLL_INTERVAL_MS = 5UL * 60UL * 1000UL; // 5 minutes

// HT16K33 default I2C address for Adafruit 7-segment backpack
const uint8_t DISPLAY_I2C_ADDR = 0x70;

Adafruit_7segment display = Adafruit_7segment();
unsigned long lastPoll = 0;

void showStatusText(const char* text) {
  // 4-character helper for quick status display (e.g., "WiFi", "Err ")
  display.clear();

  for (uint8_t i = 0; i < 4 && text[i] != '\0'; i++) {
    char c = text[i];
    if (c >= 'a' && c <= 'z') c -= 32; // to upper
    display.writeDigitAscii(i, c);
  }

  display.writeDisplay();
}

void showElevation(float elevationFt) {
  // Display XX.X format when possible on 4 digits.
  // Example: 740.8 -> "7408" with decimal after 3rd digit.
  int scaled = (int)round(elevationFt * 10.0f);

  if (scaled < 0 || scaled > 9999) {
    showStatusText("Err ");
    return;
  }

  int thousands = (scaled / 1000) % 10;
  int hundreds = (scaled / 100) % 10;
  int tens = (scaled / 10) % 10;
  int ones = scaled % 10;

  display.clear();
  display.writeDigitNum(0, thousands);
  display.writeDigitNum(1, hundreds);
  display.writeDigitNum(3, tens, true);  // decimal point after tens place -> XXX.X
  display.writeDigitNum(4, ones);
  display.writeDisplay();
}

bool connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  showStatusText("WiFi");

  const unsigned long timeoutMs = 20000;
  unsigned long start = millis();

  while (WiFi.status() != WL_CONNECTED && (millis() - start) < timeoutMs) {
    delay(300);
  }

  return WiFi.status() == WL_CONNECTED;
}

bool fetchWattsBarElevation(float& elevationOut) {
  if (WiFi.status() != WL_CONNECTED) {
    return false;
  }

  HTTPClient http;
  http.begin(DATA_URL);
  int statusCode = http.GET();

  if (statusCode != HTTP_CODE_OK) {
    http.end();
    return false;
  }

  String payload = http.getString();
  http.end();

  // Keep doc size modest for expected JSON structure
  StaticJsonDocument<1024> doc;
  DeserializationError err = deserializeJson(doc, payload);
  if (err) {
    return false;
  }

  if (!doc["watts_bar_lake"].containsKey("elevation_ft")) {
    return false;
  }

  elevationOut = doc["watts_bar_lake"]["elevation_ft"].as<float>();
  return true;
}

void setup() {
  Wire.begin();
  display.begin(DISPLAY_I2C_ADDR);
  display.setBrightness(8); // 0-15

  showStatusText("Init");

  if (!connectWiFi()) {
    showStatusText("nEt ");
    return;
  }

  float elevation = 0.0f;
  if (fetchWattsBarElevation(elevation)) {
    showElevation(elevation);
    lastPoll = millis();
  } else {
    showStatusText("Err ");
  }
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    if (!connectWiFi()) {
      showStatusText("nEt ");
      delay(2000);
      return;
    }
  }

  unsigned long now = millis();
  if (now - lastPoll >= POLL_INTERVAL_MS || lastPoll == 0) {
    float elevation = 0.0f;
    if (fetchWattsBarElevation(elevation)) {
      showElevation(elevation);
      lastPoll = now;
    } else {
      showStatusText("Err ");
      delay(2000);
    }
  }

  delay(100);
}
