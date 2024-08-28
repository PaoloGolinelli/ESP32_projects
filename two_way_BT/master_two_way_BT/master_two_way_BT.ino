#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

uint8_t slaveAddress[6] = {0x32, 0xAE, 0xA4, 0x07, 0x0D, 0x66};

void setup() {
  Serial.begin(115200);

  // Start Bluetooth in master mode
  SerialBT.begin("ESP_master", true);  // true enables master mode
  SerialBT.deleteAllBondedDevices();

  Serial.println("Attempting to connect to server...");

  if (SerialBT.connect(slaveAddress)) {    // Connect to the server using addres
    Serial.println("Connected to Server!");
  } else {
    Serial.println("Failed to connect to Server. Restarting...");
    ESP.restart();  // Restart if connection fails
  }
}

void loop() {
  // Send data to the server
  SerialBT.println("Hello from Client!");

  // Receive data from the server
  if (SerialBT.available()) {
    String receivedData = SerialBT.readString();
    Serial.print("Received from server: ");
    Serial.println(receivedData);
  }

  delay(1000);
}