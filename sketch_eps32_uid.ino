//*******
//** ADAPTED FROM ESP32 SAMPLE BLE_server ***
//*******


//** . by default, BLE client can READ the value
//** . can uncomment either INDICATE or NOTIFY so that
//**   BLE client will be notified of new value
//**   - NOTIFY   -- just notify
//**   - INDICATE -- notify, and sort of requiring acknowledgement
//#define INDICATE
#define NOTIFY


#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>


#define SERVICE_UUID        "dc717ef2-a3fe-48e9-9a14-34eece003ebb"
#define CHARACTERISTIC_UUID "6e400003-b5a3-f393-e0a9-e50e24dcca9e"


BLECharacteristic *pCharacteristic;

void setup() {
  BLEDevice::init("ESP32Ble");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_READ
#if defined(INDICATE)                                        
      | BLECharacteristic::PROPERTY_INDICATE
#elif defined (NOTIFY)                                        
      | BLECharacteristic::PROPERTY_NOTIFY
#endif                                        
  );
#if defined(INDICATE) || defined (NOTIFY)   
  // a 2902 descriptor is needed to enable notification                                     
  pCharacteristic->addDescriptor(new BLE2902());
#endif

  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  BLEDevice::startAdvertising();
}

long startMillis = millis();
void loop() {

  String dataStream = generateDataStream();
  String dataResult = generateDataResult();

  #if defined(INDICATE)                                        
    pCharacteristic->setValue(dataStream.c_str());
    pCharacteristic->indicate();
  #elif defined (NOTIFY)
    pCharacteristic->setValue(dataStream.c_str());
    pCharacteristic->notify();
  #else  
    pCharacteristic->setValue(dataStream.c_str());
  #endif
    delay(1000);
  
  

  #if defined(INDICATE)                                        
    pCharacteristic->setValue(dataResult.c_str());
    pCharacteristic->indicate();
  #elif defined (NOTIFY)
    pCharacteristic->setValue(dataResult.c_str());
    pCharacteristic->notify();
  #else  
    pCharacteristic->setValue(dataResult.c_str());
  #endif
    delay(1000);
}



String generateDataStream() {
  int day = 1;
  int month = 3;
  int year = 24;
  static int hour = 7;
  static int minute = 0;
  static int init = 0;
  static int adapt = 0;
  static int batt = 0;
  static float senssRaw =0;
  static float senss =0;
  static int pwmValue =0;
  static int deflectRate =0;
  static float heartRate =0;
  static int iRH =0;
  static int beats =0;

  minute++;
  if (minute > 59) {
    minute = 0;
    hour++;
    if (hour > 23) {
      hour = 0;
    }
  }

  init = random(0,2);
  adapt= random(0,2);
  batt = random(0,2);
  senssRaw = random(10, 1000) / 888.9;
  senss = random(0, 100);
  pwmValue = random(0, 256);
  deflectRate = 0;
  heartRate = random(0, 100);
  iRH = random(0,2);
  beats = random(0, 2);

  return "DAT," + String(hour) + ":" + String(minute) + " " + String(day) + "/" + String(month) + 
  "/" + String(year) + "," + String(init) + "," + String(adapt) + "," + String(batt) + "," + String(senssRaw) + 
  "," + String(senss) + "," + String(pwmValue) + "," + String(deflectRate) + "," + String(heartRate) + "," + 
  String(iRH) + "," + String(beats) +",TAD";
}

String generateDataResult() {
  static float sys = 0;
  static float dia = 0;
  static int map = 0;
  static float hr = 0;
  static int ihb = 0;
  static int tPump = 0;
  static int dPump = 0;
  static int sPump = 0;
  static int errorCode = 0;
  
  sys = random(0, 100);
  dia = random(0, 100);
  map = 0;
  hr = random(0, 100);
  ihb = random(0, 10);
  tPump = random(0, 10);
  dPump = random(0, 30);
  sPump = random(0, 30);
  errorCode = random(0, 8);

  return "vin,sys:" + String(sys) + ",dia:" + String(dia) + ",map:" + String(map) +
         ",hr:" + String(hr) + ",ihb:" + String(ihb) + ",t_pump:" + String(tPump) +
         ",d_pump:" + String(dPump) + ",s_pump:" + String(sPump) + ",err: " + String(errorCode) + ",niv";
}


