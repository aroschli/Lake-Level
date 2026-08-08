#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

Adafruit_7segment display = Adafruit_7segment();

void setup() {
  Wire.begin(21, 22);      // ESP32 default I2C pins
  display.begin(0x70);     // HT16K33 default I2C address
}

void loop() {
  static int counter = 0;

  display.print(counter);  // Show the number
  display.writeDisplay();  // Push to display

  counter++;
  if (counter > 9999) counter = 0;

  delay(1000);
}
