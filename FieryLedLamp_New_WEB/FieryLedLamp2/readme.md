# FieryLedLamp 2
The firmware and idea from the [project](https://github.com/MishanyaTS/FieryLedLamp_New_WEB) were taken as a basis.

The firmware has been completely rewritten. Added a setup mode that makes it easier to connect the lamp to the router and describe the broker.
A display has been added that displays the time when turned off and the name and number of the effect when turned on.
Using the button you can not only turn the lamp on/off, but also switch effects.
Added the ability to select a language.

What to do:
- [ ] Add music.
- [ ] Add on/off by time.
- [ ] Add an alarm.

[Lamp control with button](#Control-via-button)  
[Lamp control via mqtt](#Control-via-MQTT)  
[Scheme](#Scheme)  
[Installation and update](#Installation-and-update)

## Control via button
**Turning the lamp on/off**:  
Pressing the button once turns the lamp on or off.

**Switching effects**:
- Double clicking the button will enable the next effect.
- Triple click on the button will turn on the previous effect.

**Setting up a connection to WIFI**:
1. Keep the button pressed for 5 seconds.
2. The lamp will switch to WIFI setup mode and become an access point.
3. Connect to the lamp using the password 12345678.
4. Select your router from the list and enter the password.
5. Enter the settings for connecting to the MQTT broker.(**optional**)
6. Click the save button.

The lamp will exit setup mode and connect to your router.

## Control via MQTT
Sending messages to the lamp:  
Topic name: \<lamp name>/command  
Payload: string in the format JSON

### Control commands
|Command|Payload|
|--|--|
|Turn on|{"power":1}|
|Turn off|{"power":0}|
|Set effect|{"effect":<Number(0-122)>}|
|Set brightness|{"brighness":<Percent(1-100)>}|


## Management via Yandex Alice

Select a MQTT broker that allows you to interact with Alice.
Configure the device in the broker, as shown in the example of [dealgate.ru](dealgate.ru).

Register.
Create a Lamp device.
The skill Toggle switch will appear in the list of skills.

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
