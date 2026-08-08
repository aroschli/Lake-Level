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

const char* DATA_URL = "https://raw.githubusercontent.com/aroschli/Lake-Level/main/data.json";

const unsigned long POLL_INTERVAL_MS = 5UL * 60UL * 1000UL; // 5 minutes
const uint8_t DISPLAY_I2C_ADDR = 0x70;

Adafruit_7segment display = Adafruit_7segment();
unsigned long lastPoll = 0;
float lastElevation = 0.0f;

void startupAnimation() {
  for (int i = 0; i < 6; i++) {
    display.clear();
    display.writeDigitNum(i % 4, i);
    display.writeDisplay();
    delay(120);
  }
  display.clear();
  display.writeDisplay();
}

void showStatusText(const char* text) {
  display.clear();
  for (uint8_t i = 0; i < 4 && text[i] != '\0'; i++) {
    char c = text[i];
    if (c >= 'a' && c <= 'z') c -= 32;
    display.writeDigitAscii(i, c);
  }
  display.writeDisplay();
}

void showElevation(float elevationFt) {
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
  display.writeDigitNum(3, tens, true);  // decimal point
  display.writeDigitNum(4, ones);
  display.writeDisplay();
}

bool fetchFLLElevation(float& elevationOut) {
  if (WiFi.status() != WL_CONNECTED) return false;

  for (int attempt = 0; attempt < 3; attempt++) {
    HTTPClient http;
    http.begin(DATA_URL);
    int statusCode = http.GET();

    if (statusCode == HTTP_CODE_OK) {
      String payload = http.getString();
      http.end();

      StaticJsonDocument<1024> doc;
      DeserializationError err = deserializeJson(doc, payload);
      if (!err && doc["fort_loudoun_lake"].containsKey("elevation_ft")) {
        elevationOut = doc["fort_loudoun_lake"]["elevation_ft"].as<float>();
        return true;
      }
    }

    http.end();
    delay(500);  // retry delay
  }

  return false;
}

bool connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.setSleep(false);   // more stable WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  showStatusText("WiFi");

  const unsigned long timeoutMs = 20000;
  unsigned long start = millis();

  while (WiFi.status() != WL_CONNECTED && (millis() - start) < timeoutMs) {
    delay(300);
  }

  return WiFi.status() == WL_CONNECTED;
}

void setup() {
  Wire.begin(21, 22);              // ESP32 I2C pins
  display.begin(DISPLAY_I2C_ADDR);
  delay(20);
  display.setBrightness(8);

  startupAnimation();

  //showStatusText("Init");

  if (!connectWiFi()) {
    showStatusText("nEt ");
    return;
  }

  float elevation = 0.0f;
  if (fetchFLLElevation(elevation)) {
    lastElevation = elevation;
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
    if (fetchFLLElevation(elevation)) {
      lastElevation = elevation;
      showElevation(elevation);

      // Heartbeat blink
      delay(200);
      int scaled = (int)round(elevation * 10.0f);
      int tens = (scaled / 10) % 10;
      display.writeDigitNum(3, tens, false);
      display.writeDisplay();

      lastPoll = now;
    } else {
      // Fallback to last known elevation
      showElevation(lastElevation);
      delay(2000);
    }
  }

  delay(100);
}
