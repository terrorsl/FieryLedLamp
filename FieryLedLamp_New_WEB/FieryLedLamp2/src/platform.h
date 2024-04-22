#ifndef PLATFORM_FILE
#define PLATFORM_FILE

#define DEBUG_PRINT_ENABLE

#if defined(DEBUG_PRINT_ENABLE)
#define DBG_START(baud) Serial.begin(baud);delay(500)
#define DBG_PRINT(format,...) Serial.printf(format,##__VA_ARGS__)
#else
#endif

#if defined(ESP8266)
#include<ESP8266WiFi.h>

#define BUILDIN_LED_PIN 2
#define LED_PIN 0
#define BUTTON_PIN 4
#define MOSFET_PIN 5 

#define DISPLAY_ADDRESS 0x3c
#define DISPLAY_SDA 12
#define DISPLAY_SCL 13

#elif defined(ESP32)
#include<WiFi.h>

#define LED_PIN 0
#define BUTTON_PIN 4
#define MOSFET_PIN 5 
#else
#error "unknown platform"
#endif

#define BUTTON_IS_SENSORY 1

// time in ms
#define SETUP_BUTTON_TIME 7000
#define POWER_BUTTON_TIME 2000

#define DELTA_BUTTON_DOWN 500
#define POWER_BUTTON_COUNT 1
#define NEXT_BUTTON_COUNT 1
#define BEFOR_BUTTON_COUNT 2

#endif