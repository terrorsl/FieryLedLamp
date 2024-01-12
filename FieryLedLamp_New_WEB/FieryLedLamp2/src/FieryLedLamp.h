#ifndef FIERY_LED_LAMP_FILE
#define FIERY_LED_LAMP_FILE

#include"platform.h"
#include"Constants.h"
#include"effect.h"

#include<ArduinoJson.h>

#ifdef USE_MQTT
#include <AsyncMqttClient.h>
#define MQTT_STATUS_TOPIC "status"
#define MQTT_COMMAND_TOPIC "/command"
#define MQTT_RESULT_TOPIC "/result"
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
	unsigned short currentEffect;
	FieryLedLampEffect *effect;
#if defined(USE_MQTT)
	FieryLedLampConfigMQTT mqtt;
#endif
};

typedef enum
{
	POWER,
	EFFECT
}FieryLedLampCommands;
class FieryLedLamp
{
public:
	void setup();

	void connect_web();
	void connect_mqtt();

	void change_effect(unsigned short index);
	void next_effect();
	void prev_effect();

#ifdef USE_MQTT
	void setup_mqtt_subscribe();
	void update_mqtt(const char *payload);
#endif
	void update();

	void update_time();

	WebServer *Web(){return web;}
private:
	void setup_pin();
	void setup_time();
	void setup_config();
#ifdef USE_MQTT
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
	unsigned long button_down_time, button_up_time;
	unsigned char button_down_count;

	unsigned long current_time;
	FieryLedLampConfig config;
};

extern FieryLedLamp lamp;
#endif
