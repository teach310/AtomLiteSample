// #include <M5Unified.h>

// const int PIN_NUMBER = 8;

// void setup()
// {

//   auto cfg = M5.config();
//   M5.begin(cfg);
//   USBSerial.begin(115200);
//   pinMode(PIN_NUMBER, INPUT);
// }

// void loop()
// {

//   M5.Display.fillScreen(BLACK);
//   M5.Display.setCursor(0, 20);
//   // M5.Display.printf("GPIO%d: %d\n", PIN_NUMBER, analogRead(PIN_NUMBER));
//   // USBSerial.printf("GPIO%d: %d\n", PIN_NUMBER, analogRead(PIN_NUMBER));
//   // digitalReadにする
//   M5.Display.printf("GPIO%d: %d\n", PIN_NUMBER, digitalRead(PIN_NUMBER));
//   USBSerial.printf("GPIO%d: %d\n", PIN_NUMBER, digitalRead(PIN_NUMBER));
//   delay(500);
// }


#include <M5Unified.h> // M5Unifiedライブラリを使用可能にします。
#include <FastLED.h>   // FastLED(RGB LED)のライブラリを使用可能にします。

// RGB LEDの数を指定(M5Atom Matrixなら25)
#define NUM_LEDS 1
// RGB LEDのDATA PINを指定
#define LED_DATA_PIN 27

CRGB leds[NUM_LEDS];

void setup() {

  auto cfg = M5.config();       // M5Stack初期設定用の構造体を代入
  
  // configを設定する場合はここで設定
  // 例
  // cfg.external_spk = true;

  M5.begin(cfg);                                           // M5デバイスの初期化, Displayも初期化されます。
  Serial.println("充電します！");
}

void loop() {
}
