#include "BluetoothSerial.h"
#include "esp_mac.h"
#include "esp_bt_device.h"

// Set your new MAC Address
uint8_t newMACAddress[6] = {0x32, 0xAE, 0xA4, 0x07, 0x0D, 0x66};

BluetoothSerial SerialBT;

void setup(){
  // Change ESP32 Mac Address
  esp_base_mac_addr_set(&newMACAddress[0]);

  Serial.begin(115200);
  Serial.println();
  
  // Initialize Bluetooth
  SerialBT.begin("ESP32");

  // Read the new MAC address
  Serial.print("[NEW] ESP32 Board MAC Address: ");
  const uint8_t *macAddr = esp_bt_dev_get_address();
  for (int i = 0; i < 6; i++) {
    if (i != 0) Serial.print(":");
    Serial.print(macAddr[i], HEX);
  }
  Serial.println();
}
 
void loop(){

}