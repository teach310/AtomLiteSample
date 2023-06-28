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

// グローバル変数（プログラム全体で使用する変数の定義をします。）
uint32_t count;
CRGB leds[NUM_LEDS];

// setup関数は起動時に一度だけ実行されます。
// 主に初期化処理を記述します。
void setup() {

  auto cfg = M5.config();       // M5Stack初期設定用の構造体を代入
  
  // configを設定する場合はここで設定
  // 例
  // cfg.external_spk = true;

  M5.begin(cfg);                                           // M5デバイスの初期化, Displayも初期化されます。
  FastLED.addLeds<WS2811, LED_DATA_PIN, GRB>(leds, NUM_LEDS);   // RGB LEDを初期設定
  FastLED.setBrightness(20);                               // 明るさを設定（20以上は熱で壊れる可能性あり。）
  Serial.println("Hello World!!");
  count = 0;                                               // countを初期化

}

// loop関数は起動している間ずっと繰り返し実行されます。
// センサーから値を取得したり、画面を書き換える動作等をおこないます。
void loop() {

  leds[0] = CRGB::Red;                      // LED[0]を赤に設定
  FastLED.show();                           // LEDを表示
  delay(500);                               // 0.5秒待つ
  leds[0] = CRGB::Black;                    // LEDを黒に設定
  FastLED.show();                           // LEDを表示（黒なので消灯）
  delay(500);                               // 0.5秒待つ

  Serial.printf("COUNT: %d\n", count);      // シリアルにcount値を表示
  count++;                                  // カウントを1増やす。
}
