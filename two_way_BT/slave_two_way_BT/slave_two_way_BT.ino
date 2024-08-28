#include "BluetoothSerial.h"
#include "esp_mac.h"
#include "esp_bt_device.h"

BluetoothSerial SerialBT;
uint8_t newMACAddress[6] = {0x32, 0xAE, 0xA4, 0x07, 0x0D, 0x66};

void setup() {
  newMACAddress[5] -= 2;
  esp_base_mac_addr_set(&newMACAddress[0]);
  newMACAddress[5] += 2;

  Serial.begin(115200);
  SerialBT.begin("ESP_slave", false);  // Start Bluetooth in slave mode
  Serial.println("Bluetooth Server Started. Waiting for clients...");

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
  if (SerialBT.available()) {
    String receivedData = SerialBT.readString();
    Serial.print("Received from client: ");
    Serial.println(receivedData);

    // Respond to the client
    SerialBT.println("Hello from Server!");
  }
  delay(100);
}