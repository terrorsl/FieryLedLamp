#ifndef FIERY_LED_LAMP_FILE
#define FIERY_LED_LAMP_FILE

#include"platform.h"
#include"Constants.h"

#ifdef USE_MQTT
#include <AsyncMqttClient.h>
#define MQTT_STATUS_TOPIC "status"
#define MQTT_COMMAND_TOPIC "command"
#define MQTT_DIAGNOSTIC_TOPIC "result"
#endif

#ifdef USE_NTP
#include <NTPClient.h>
#endif

class FieryLedLamp
{
public:
	void setup();

	bool setup_config();
	void setup_time();
	void setup_pin();
	void setup_wifi();
	bool setup_led();
	bool setup_web_server();

	void setup_mqtt();
	void setup_mqtt_subscribe();

	void update();

	void update_time();
private:
	void update_effect();
	//ArduinoJson
#ifdef USE_NTP
	NTPClient *ntpClient;
#endif
#ifdef USE_MQTT
	AsyncMqttClient mqtt;
#endif
};

extern FieryLedLamp lamp;
#endif
