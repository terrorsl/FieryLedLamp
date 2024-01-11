#ifndef PLATFORM_FILE
#define PLATFORM_FILE

#if defined(ESP8266)
#include<ESP8266WiFi.h>
#include<ESP8266WebServer.h>

#define WebServer ESP8266WebServer

#define BUILDIN_LED_PIN 2
#define LED_PIN 0
#define BUTTON_PIN 4
#define MOSFET_PIN 5 

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
#define SETUP_BUTTON_TIME 5000

#endif