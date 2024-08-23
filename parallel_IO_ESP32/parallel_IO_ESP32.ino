/*-----------------------------------------------
/                 Parallel IO 
/ -----------------------------------------------
/   In this project I show an example on how to
/   exploit the secondary core of ESP32 to do
/   work in parallel with the primary core.
/
/   It is in fact very common the need of reading 
/   inputs while preforming computation and/or 
/   outputting serial data, which is quite time
/   consuming. 
/   E.g.: printing on a display while reading
/   analog data
/ -----------------------------------------------*/

#include <stdint.h> 

const int inputPin = 4;       // define the input pin

volatile int inputValue = 0;  // variable to store the input value

// Task handle definition
TaskHandle_t handle_loop2;
void loop2(void *parameter);

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  delay(1000);

  pinMode(inputPin, INPUT);

  // Create tasks and assign them to specific cores
  xTaskCreatePinnedToCore(
    loop2,            // Function to implement the task
    "loop2",          // Name of the task
    2048,             // Stack size in words
    NULL,             // Task input parameter
    1,                // Priority of the task
    &handle_loop2,    // Task handle
    0);               // Core where the task should run
}

// loop prints output
void loop() {

  if (inputValue) 
    printf("Pressed\n");
  else
    printf("Released\n");

  delay(10);
}

// loop reading inputs
void loop2(void *parameter) {
  Serial.print("loop2 running on core ");
  Serial.println(xPortGetCoreID());

  for(;;) {
    // Read the input pin and store the value
    inputValue = digitalRead(inputPin);
    vTaskDelay(1);  // Short delay to yield control (allows other tasks to be performed)

/*     if (condition to stop the reading)
      break; */
  }

  vTaskDelete(NULL); // always delete the task after use
}