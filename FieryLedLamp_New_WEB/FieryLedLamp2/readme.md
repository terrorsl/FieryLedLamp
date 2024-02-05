# FieryLedLamp 2

# Platform
[![Esp8266](https://img.shields.io/badge/platform-ESP8266-green)](https://www.espressif.com/en/products/socs/esp8266)
[![Esp32](https://img.shields.io/badge/platform-ESP32-green)](https://www.espressif.com/en/products/socs/esp32)

# Scheme
# Installation and update
## Automatically
Download firmware for the first time:
- Download the prepared [BIN]() file.
- Follow the instructions on the [page](https://github.com/SequoiaSan/Guide-How-To-Upload-bin-to-ESP8266-ESP32)

Updating installed firmware:
## Manually
- Download the firmware source code
- Prepare the IDE in which you plan to build the firmware
    - ## Arduino IDE
        - Install the IDE
        - Install the toolchain
        - Open the project FieryLedLamp2.ino
        - In the menu, select the ESP version, specify the port.
        - Select Build and Upload firmware.
    - ## Visual Studio Code
        - Install IDE
        - Install the toolchain(PlatformIO)
        - Open project
        - In platformio choose serial port and Build
        - In platformio choose Upload
