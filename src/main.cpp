// 参考
// https://github.com/espressif/arduino-esp32/blob/master/libraries/BLE/examples/BLE_client/BLE_client.ino

#include <M5Unified.h>
#include <BLEDevice.h>

#define SERVICE_UUID "068c47b7-fc04-4d47-975a-7952be1a576f"
#define CHARACTERISTIC_UUID "e3737b3f-a08d-405b-b32d-35a8f6c64c5d"

static BLEUUID serviceUUID(SERVICE_UUID);
static BLEUUID charUUID(CHARACTERISTIC_UUID);
static BLEAdvertisedDevice *pPeripheral;

static int8_t state = 0;

#define STATE_IDLE 0
#define STATE_DO_CONNECT 1
#define STATE_CONNECTED 3

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
{
  void onResult(BLEAdvertisedDevice advertisedDevice)
  {
    Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID))
    {
      Serial.println("Device found!");
      pPeripheral = new BLEAdvertisedDevice(advertisedDevice);
      advertisedDevice.getScan()->stop();
      state = STATE_DO_CONNECT;
    }
  }
};

class MyClientCallbacks : public BLEClientCallbacks
{
  void onConnect(BLEClient *pclient)
  {
    Serial.println("onConnect");
    state = STATE_CONNECTED;
  }

  void onDisconnect(BLEClient *pclient)
  {
    Serial.println("onDisconnect");
    state = STATE_IDLE;
  }
};

void scan()
{
  BLEScan *pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  // Interval, Windowはdefaultの値で動作して問題なさそうなため設定しない。
  // アドバタイズを受信するだけのためパッシブスキャン
  // trueにすると高速にペリフェラルを検出できるかもしれないが、パッシブでもすぐ検出できるため必要性は感じていない
  // https://github.com/espressif/arduino-esp32/blob/master/libraries/BLE/examples/BLE_scan/BLE_scan.ino#L27
  pBLEScan->setActiveScan(false);

  // スキャン5秒には特に意味はない。
  // スキャン結果を残しておく必要がないため、終わったクリアする。そのためにis_continueはfalseにする
  pBLEScan->start(5, false);
}

bool connect()
{
  BLEClient *pClient = BLEDevice::createClient();
  pClient->setClientCallbacks(new MyClientCallbacks());
  return pClient->connect(pPeripheral);
}

void setup()
{
  auto cfg = M5.config();
  M5.begin(cfg);

  BLEDevice::init("M5AtomLite BLE Client");

  // setupで単発実行。繰り返し実行するならloopに配置する必要がある
  scan();
}

void loop()
{
  switch (state)
  {
  case STATE_DO_CONNECT:
    if (connect())
    {
      Serial.println("Connected to server");
    }
    else
    {
      Serial.println("Failed to connect");
      state = STATE_IDLE;
    }
    break;
  default:
    break;
  }
}
