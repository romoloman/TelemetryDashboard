# FrSky Telemetry on Cheap Yellow Display

A project to display telemetry data from FrSky receivers (S.Port and D8 protocols) on an inexpensive display commonly known as "Cheap Yellow Display" (based on ESP32).

## Project Description

This repository contains the code and instructions to turn a simple ESP32-based "Cheap Yellow Display" into a practical telemetry screen for FrSky radio systems. It allows you to display essential flight parameters in real-time, such as battery voltage, RSSI signal strength, altitude, speed, and other data transmitted by FrSky receivers supporting S.Port or D8 protocols.

The goal is to provide an affordable and easy-to-build solution for having a separate telemetry display or one integrated into vintage radio convesione.

## Key Features

* Support for FrSky S.Port and D8 telemetry protocols.
* Display of key data (voltage, RSSI, altitude, speed, etc.).
* Simple and clear graphical interface on the display.
* Based on ESP32 for flexibility and Wi-Fi/Bluetooth connectivity (potential future developments).
* Voice alerts for telemetry lost/recovered low battery (both tx and rx) and rssi
* Easily customizable and expandable.
* The graphical interface has been designed using EEZ Studio and can be easily modified

## Hardware Requirements

To build this project, you will need:

* A "Cheap Yellow Display" module (typically ESP32 with a 2.8" or 3.2" TFT display).
* An FrSky receiver with telemetry support (S.Port or D8).
* Connection wires for the serial port (UART) between the display and the receiver.
* A power source for the Cheap Yellow Display you can use either the USB or the proper connector on the board.
* 1 3.3k resistor and 1 10k resistor to be able to read tx battery voltage.

## Software Requirements

* Arduino IDE (or another development environment for ESP32).
* Necessary libraries for display management (e.g., TFT_eSPI and LVGL 9.2)
* Necessary libraries for telemetry (have a look to https://github.com/romoloman/FrSkySportTelemetry )
* EEZ Studio to customize the interface
  
## Installation and Setup

1.  Clone this repository to your computer.
2.  Open the project in your preferred development environment (e.g., Arduino IDE with ESP32 support installed).
3.  Install the required libraries (check the specific instructions in the code or accompanying documentation).
4.  Connect your Cheap Yellow Display to your computer via USB.
5.  Connect the telemetry output of your FrSky receiver to the UART input on the ESP32 of the Cheap Yellow Display IO27.
6.  Coonect the 3.3k resistor between ground and IO35
7.  Connect a small 8 Ohm 3W speaker on audio port of the chaep yelllow display
8.  Upload the code to your Cheap Yellow Display.

## Usage

After uploading the code and connecting the display to the FrSky module correctly (ensure the receiver is powered and connected to your radio), the display should power on and begin showing telemetry data as it is received.
You can scroll the display on the left or on the right to display different telemetry screens

Make sure your FrSky receiver is configured to output telemetry and that your radio is powered on and bound.

## Contributing

Contributions are welcome! If you have ideas for improving the project, bug fixes, or new features, feel free to open an issue or submit a pull request.
