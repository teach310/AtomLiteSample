#include <M5Unified.h>

void setup()
{
  auto cfg = M5.config();

  M5.begin(cfg);
  Serial.println("充電します！");
}

void loop()
{
}
