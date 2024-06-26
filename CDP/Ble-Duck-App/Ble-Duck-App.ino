#include <MamaDuck.h>

#ifdef SERIAL_PORT_USBVIRTUAL
#define Serial SERIAL_PORT_USBVIRTUAL
#endif

#include "BluetoothSerial.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;
MamaDuck duck;
char message[32]; 
int counter = 1;

void setup()
{
  std::string deviceId("MAMA0001");
  std::vector<byte> devId;
  devId.insert(devId.end(), deviceId.begin(), deviceId.end());
  duck.setDeviceId(devId);
  duck.setupSerial();
  duck.setupRadio();
  Serial.println("MAMA-DUCK...READY!");
  SerialBT.begin("MamaDuck1"); 
  Serial.println("The device started, now you can pair it with bluetooth!");
}

void loop()
{
  if (SerialBT.available())
  {
    String text = SerialBT.readString();
    Serial.println(text);
    duck.sendData(topics::status, text.c_str());
    delay(10);
    String message = "Third 9";
    uint8_t buffer[256];
    memcpy(buffer, message.c_str(), message.length());
    SerialBT.write(buffer, message.length());
  }
  duck.run();
}