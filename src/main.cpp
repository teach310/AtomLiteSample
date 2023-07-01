#include "Adafruit_VL53L0X.h"
#include <M5Unified.h>
#include <FastLED.h>

#define NUM_LEDS 1
#define LED_DATA_PIN 27
#define SDA_PIN 25
#define SCL_PIN 21

Adafruit_VL53L0X lox = Adafruit_VL53L0X();
CRGB leds[NUM_LEDS];

bool isAvailable()
{
  Wire.beginTransmission(VL53L0X_I2C_ADDR);
  return (Wire.endTransmission() == 0);
}

void setup()
{
  auto cfg = M5.config();
  M5.begin(cfg);
  FastLED.addLeds<WS2811, LED_DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(20);
  Wire.setPins(SDA_PIN, SCL_PIN);
  if (!lox.begin())
  {
    Serial.println(F("Failed to boot VL53L0X"));
    while (1)
      ;
  }
  Serial.println(F("Start App\n\n"));
  lox.startRangeContinuous();
}

void loop()
{
  if (lox.isRangeComplete())
  {
    Serial.print("Distance in mm: ");
    Serial.println(lox.readRange());
    leds[0] = CRGB::Red;
    FastLED.show();
  }
  else
  {
    leds[0] = CRGB::Blue;
    FastLED.show();
  }
  delay(500);
}
