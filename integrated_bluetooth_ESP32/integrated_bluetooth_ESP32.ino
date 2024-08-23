/*--------------------------------------------
/               BLuetooth chat
/ --------------------------------------------
/   Connect the ESP32 to the SSD1306 display
/   Attach via USB to the PC and power it. 
/   Pair the smartphone with device ESP32chat 
/   and start sending messages from serial
/   monitor and from smartphone
/ --------------------------------------------*/

#include "BluetoothSerial.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// BT global variables
BluetoothSerial SerialBT;
#define BUFFSIZE 9            // nr of last messages saved (aka: nr of lines the display can print)
String  buffer[BUFFSIZE];     // buffer containing last BUFFSIZE messages
int     lines_arr[BUFFSIZE];  // array containing number of lines of each BUFFSIZE message

bool    first;      // flag turned to false once the first char has been read. Restored once '\n' is reached
int     length;     // numbers of char of the message
bool    read_msg;   // it is set to true if it is sending a message, so it wont send one
bool    send_msg;   // it is set to true if it is sending a message, so it wont receive one


// Display definition
#define   SCREEN_WIDTH    128   // OLED display width, in pixels
#define   SCREEN_HEIGHT   64    // OLED display height, in pixels
#define   CHARxLINE       21.0  // number of characters a line can display
#define   PIXEL_LINE_H    8     // height of line in pixels
Adafruit_SSD1306 display (SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


void setup() {
  Serial.begin(115200);

  // attach I2C comunication with display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;) delay(1); // Don't proceed, loop forever
  }

  // print initial page
  display.clearDisplay();
  display.setTextColor(1);
  display.setTextSize(3);
  display.setCursor(28, 5);   display.println(F(" BT "));
  display.setCursor(28, 30);  display.println(F("CHAT"));
  display.display();

  SerialBT.begin("ESP32chat"); // bluetooth device's name
  Serial.println("\nThe device started, now you can pair it with bluetooth!\n");

  first    = true;  // ready to read the first char
  read_msg = false;
  send_msg = false;
}

void loop() {
  char c_in;

  // Message sent
  if (Serial.available() && !read_msg) {  // PC keyboard sends via USB to ESP32
    if (first) {
      send_msg = true;              // turn flag to true, so it cant send a msg while receiveng one
      buffer[BUFFSIZE-1] += "-> ";  // draw -> since it is sent via BT 
      first = false;                // first char has been read
      length = 3;
    }

    c_in = Serial.read();           // read char from serial communication with PC
    SerialBT.write(c_in);           // msg sent to smartphone via BT

    if (c_in == '\n') {             // end of message
      lines_arr[BUFFSIZE-1] = (length/CHARxLINE) + 1; // calculate the number of extra lines
      shift_buff();                 // shift the buffer
      display_write();              // display the last messages
      first = true;                 // resets flags
      send_msg = false;             // allow to send a message
    } else {                        // word not complete
      buffer[BUFFSIZE-1] += c_in;   // insert char
      length++;                     // increment length of msg
    }
  }

  // Message received
  if (SerialBT.available() && !send_msg) {  // smartphone sends via BT to ESP32
    if (first) {
      read_msg = true;              // turn flag to true, so it cant receive a msg while sending one
      buffer[BUFFSIZE-1] += "<- ";  // draw "<- " since it is received via BT 
      first = false;                // first char has been read
      length = 3;
    }
  
    c_in = SerialBT.read();         // read char received from smartphone via BT 
    Serial.write(c_in);             // ESP32 writes to PC via USB
    
    if (c_in == '\n') {             // word complete
      lines_arr[BUFFSIZE-1] = (length-1)/CHARxLINE + 1; // calculate the number of extra lines
      shift_buff();                 // shift the buffer
      display_write();              // display the last messages
      first = true;                 // resets flags
      read_msg = false;             // allow to receive a message
    } else {                        // word not complete
      buffer[BUFFSIZE-1] += c_in;   // insert char
      length++;                     // increment length of msg
    }
  }

  //delay(1); // introduce a slight delay
}

void shift_buff() {
  // msg 1 goes to 0. msg 2 goes to 1...
  for (int i = 0; i < BUFFSIZE; i++) {
    buffer[i] = buffer[i+1];
    lines_arr[i] = lines_arr[i+1];
  }

  // new spot for msg is initialized
  buffer[BUFFSIZE-1] = "";
  lines_arr[BUFFSIZE-1] = 0;
}

void display_write() {
  // calculate the number of total extra lines
  int tot_n_lines = 0;

  for (int t = 0; t < BUFFSIZE; t++) {
    if (lines_arr[t] != 0)              // if a msg takes more than one line tot_n_lines is incremented
      tot_n_lines += lines_arr[t]-1;    // by the number of extra lines
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(1);
  display.setCursor(0,-PIXEL_LINE_H*tot_n_lines);  // starts above of PIXEL_LINE_H pixels per total extra lines 

  // display each message and goes to new line
  for (int i = 0; i < BUFFSIZE; i++) {
    display.println(buffer[i]);
  }

  display.display();
}