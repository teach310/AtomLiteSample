#include "Adafruit_VL53L0X.h"
#include <M5Unified.h>
#include <FastLED.h>

// RGB LEDの数を指定(M5Atom Matrixなら25)
#define NUM_LEDS 1
// RGB LEDのDATA PINを指定
#define LED_DATA_PIN 27

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
  Wire.begin(25, 21);
  FastLED.addLeds<WS2811, LED_DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(20);
  Serial.print("Hello!!");
  if (isAvailable())
  {
    Serial.println("VL53L0X found");
  }
  else
  {
    Serial.println("VL53L0X not found");
  }
  if (!lox.begin())
  {
    Serial.println(F("Failed to boot VL53L0X"));
    while (1)
      ;
  }
  Serial.println(F("VL53L0X API Continuous Ranging example\n\n"));
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
