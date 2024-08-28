# ESP32_projects
In this repository I will upload some of the codes I wrote to run on ESP32 to implement useful functionalities 
both integrated in the ESP32 chip and with the use of external components


# Projects

## integrated_bluetooth_ESP32
In this project I exploit the integrated bluetooth to send and receive messages with a smartphone
The messages are displayed on a LCD screen (I2C: connect SCL to pin GPIO22, SDA to pin GPIO21)
To communicate with your smartphone you need to install Serial Bluetooth Terminal app (https://www.appsapk.com/serial-bluetooth-terminal)

## integrated_touch_sensor_ESP32
In this project I exploit the several integrated touch sensors available in an ESP32
The analog readings values for a "press" and a "not pressed" may vary by a lot depending on the length of the jumper cable used,
on the person touching the cable and other factors. Thus I wrote an auto calibration procedure that allow to detect 
various tipes of touches: touching the exposed metallic wire as well as touching the plastic coating.

## parallel_IO_ESP32
In this project I show an example on how to exploit the secondary core of ESP32 to do work in parallel with the primary core.
It is in fact very common the need of reading inputs while preforming computation and/or outputting serial data, which is quite time consuming
E.g.: printing on a display while reading analog data

## two_way_BT 
In this folder you will find 4 sketches, each of them contains useful codes to pair two ESP32 via bluetooth, and let them communicate.
In particular it handles the MAC address of the ESP, which is normally an hardwired 6 bite code unique for each device.
The master device is required to know the MAC address of the slave to connect to its bluetooth. If you want be able to
upload the same code to different devices and let them be either the master and the slave, it is required to set the same
MAC address to each device acting as slave.
### get_mac_address
Contains the code used to get the current MAC address of the device
### set_mac_address
Contains the code used to overwrite the MAC address with a desired one. Note that is reversible, since it goes back to the original MAC address once the system is rebooted
### slave_two_way_BT
Contains the code for the slave device: it set the MAC address and initiates the BT
After communication is succesfull it receives and sends data from/to master.
### master_two_way_BT
Contains the code for the master device: it initiates the BT and then connect to the slave via MAC address.
After communication is succesfull it sends and receives data to/from slave. 
