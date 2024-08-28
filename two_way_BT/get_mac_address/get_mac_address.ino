#include "BluetoothSerial.h"
#include "esp_bt_device.h"
#include <esp_mac.h>

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  
  // Initialize Bluetooth
  SerialBT.begin("ESP32");

  // Retrieve and print the Bluetooth MAC address
  const uint8_t* macAddr = esp_bt_dev_get_address();
  Serial.print("Bluetooth MAC Address: ");
  for (int i = 0; i < 6; i++) {
    if (i != 0) Serial.print(":");
    Serial.print(macAddr[i], HEX);
  }
  Serial.println();
}

void loop() {
  // Your code here
}