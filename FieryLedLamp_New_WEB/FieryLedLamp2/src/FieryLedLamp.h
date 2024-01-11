#ifndef FIERY_LED_LAMP_FILE
#define FIERY_LED_LAMP_FILE

#include"platform.h"
#include"Constants.h"

#include<ArduinoJson.h>

#ifdef USE_MQTT
#include <AsyncMqttClient.h>
#define MQTT_STATUS_TOPIC "status"
#define MQTT_COMMAND_TOPIC "command"
#define MQTT_DIAGNOSTIC_TOPIC "result"
#endif

#ifdef USE_NTP
#include <NTPClient.h>
#endif

#if defined(USE_MQTT)
struct FieryLedLampConfigMQTT
{
	std::string server, login, password;
	uint16_t port, keep_alive;
	std::string clientid;
};
#endif
struct FieryLedLampConfig
{
	bool power_state;
	bool wifi_ap_mode;
#if defined(USE_MQTT)
	FieryLedLampConfigMQTT mqtt;
#endif
};

class FieryLedLamp
{
public:
	void setup();

	void connect_web();
	void connect_mqtt();

	void setup_time();
	void setup_wifi();
	bool setup_led();

	void setup_mqtt_subscribe();

	void update();

	void update_time();

	WebServer *Web(){return web;}
private:
#ifdef USE_MQTT
	void setup_pin();
	void setup_config();
	void setup_mqtt();
#endif
	void setup_web_server();

	void update_effect();
	void update_button();

	void power_button(bool state);

	JsonDocument load_config();
	void save_config(JsonDocument *doc);
#ifdef USE_NTP
	NTPClient *ntpClient;
#endif
#ifdef USE_MQTT
	AsyncMqttClient mqtt;
#endif
	WebServer *web;

	bool button_down;
	unsigned long button_down_time;

	unsigned long current_time;
	FieryLedLampConfig config;
};

extern FieryLedLamp lamp;
#endif
