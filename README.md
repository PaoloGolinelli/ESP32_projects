# ESP32_projects
In this repository I will upload some of the codes I wrote to run on ESP32 to implement useful functionalities 
both integrated in the ESP32 chip and with the use of external components


# Projects

## integrated_bluetooth_ESP32
In this project I exploit the integrated bluetooth to send and receive messages with a smartphone
The messages are displayed on a LCD screen (I2C: connect SCL to pin GPIO22, SDA to pin GPIO21)
To comunicate with your smartphone you need to install Serial Bluetooth Terminal app (https://www.appsapk.com/serial-bluetooth-terminal)

## integrated_touch_sensor_ESP32
In this project I exploit the several integrated touch sensors available in an ESP32
The analog readings values for a "press" and a "not pressed" may vary by a lot depending on the length of the jumper cable used,
on the person touching the cable and other factors. Thus I wrote an auto calibration procedure that allow to detect 
various tipes of touches: touching the exposed metallic wire as well as touching the plastic coating.
