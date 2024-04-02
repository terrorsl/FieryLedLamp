#ifndef WEB_SERVER_FILE
#define WEB_SERVER_FILE

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#elif defined(ESP32)
#include <WiFi.h>
#include <AsyncTCP.h>
#endif

#include <ESPAsyncWebServer.h>
#include "AsyncJson.h"
#include "ArduinoJson.h"
// FIXME: fix
// TODO: 
// BUG: 

#endif