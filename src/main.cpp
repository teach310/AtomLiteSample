// 参考
// https://github.com/espressif/arduino-esp32/blob/master/libraries/BLE/examples/BLE_client/BLE_client.ino

#include <M5Unified.h>
#include <BLEDevice.h>

#define SERVICE_UUID "068c47b7-fc04-4d47-975a-7952be1a576f"
#define CHARACTERISTIC_UUID "e3737b3f-a08d-405b-b32d-35a8f6c64c5d"
#define NOTIFY_CHARACTERISTIC_UUID "c9da2ce8-d119-40d5-90f7-ef24627e8193"

static BLEUUID serviceUUID(SERVICE_UUID);
static BLEUUID charUUID(CHARACTERISTIC_UUID);
static BLEUUID notifyCharUUID(NOTIFY_CHARACTERISTIC_UUID);
static BLEAdvertisedDevice *pPeripheral;
static BLERemoteCharacteristic *pRemoteCharacteristic;
static BLERemoteCharacteristic *pNotifyCharacteristic;

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

static void notifyCallback(
    BLERemoteCharacteristic *pBLERemoteCharacteristic,
    uint8_t *pData,
    size_t length,
    bool isNotify)
{
  Serial.print("Notify callback for characteristic ");
  Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
  Serial.print(" of data length ");
  Serial.println(length);
  Serial.print("data: ");
  Serial.println((char *)pData);
}

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
  if (!pClient->connect(pPeripheral))
  {
    return false;
  }

  BLERemoteService *pRemoteService = pClient->getService(serviceUUID);
  if (pRemoteService == nullptr)
  {
    Serial.print("Failed to find our service UUID: ");
    Serial.println(serviceUUID.toString().c_str());
    pClient->disconnect();
    return false;
  }
  Serial.println(" - Found our service");

  // Writeで使うためにCharacteristicを保持しておく
  pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
  if (pRemoteCharacteristic == nullptr)
  {
    Serial.print("Failed to find our characteristic UUID: ");
    Serial.println(charUUID.toString().c_str());
    pClient->disconnect();
    return false;
  }
  Serial.println(" - Found our characteristic");

  if (pRemoteCharacteristic->canRead())
  {
    std::string value = pRemoteCharacteristic->readValue();
    Serial.print("The characteristic value was: ");
    Serial.println(value.c_str());
  }

  if (!pRemoteCharacteristic->canWrite())
  {
    Serial.println("Characteristic is not writable");
    pClient->disconnect();
    return false;
  }

  pNotifyCharacteristic = pRemoteService->getCharacteristic(notifyCharUUID);
  if (pNotifyCharacteristic == nullptr)
  {
    Serial.print("Failed to find our notify characteristic UUID: ");
    Serial.println(notifyCharUUID.toString().c_str());
    pClient->disconnect();
    return false;
  }

  if (pNotifyCharacteristic->canNotify())
  {
    pNotifyCharacteristic->registerForNotify(notifyCallback);
    Serial.println(" - Registered for notify");
  }

  return true;
}

void connectedLoop()
{
  M5.update();
  if (M5.BtnA.wasClicked())
  {
    String value = "Write Data " + String(random(100, 999));
    pRemoteCharacteristic->writeValue(value.c_str(), value.length());
    Serial.println("Write: " + value);
  }
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
  case STATE_CONNECTED:
    connectedLoop();
    break;
  default:
    break;
  }
}
