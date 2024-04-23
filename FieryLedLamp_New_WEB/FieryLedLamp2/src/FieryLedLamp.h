#ifndef FIERY_LED_LAMP_FILE
#define FIERY_LED_LAMP_FILE

#include"platform.h"
#include"Constants.h"
#include"effect.h"
#include"lang.h"

#include"display.h"

#include<ArduinoJson.h>

#ifdef USE_MQTT
#include <AsyncMqttClient.h>
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
	bool need_save;
	bool power_state;
	
	Languages language;

	unsigned short currentEffect;
	FieryLedLampEffect *effect;
#if defined(USE_MQTT)
	FieryLedLampConfigMQTT mqtt;
#endif
	unsigned char speed, scale, brightness;
};

#define FieryButtonUnknownState 0
#define FieryButtonPowerState 1
#define FieryButtonSetupState 2

struct FieryButton
{
	bool is_down;
	// current state
	unsigned char state;
	// switch to state when release button
	unsigned char switch_state;
	// time when button pressed
	unsigned long down_time;

	unsigned char klick_count;
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

	// set effect by index
	bool change_effect(unsigned short index);
	// set next effect
	void next_effect();
	// set previe effect
	void prev_effect();

	// set next effect after some delay
	void auto_change_effect(unsigned char minutes);

	void set_speed(uint8_t speed);
	void set_brightness(uint8_t bright);

	Languages get_language(){return config.language;}
#ifdef USE_MQTT
	void setup_mqtt_subscribe();
	void update_mqtt(const char *topic, const char *payload);
#endif

	void power_button(bool state);
	
	void update();

	void update_time();
private:
	void setup_pin();
	void setup_display();
	void setup_time();
	void setup_config();
#ifdef USE_MQTT
	void setup_mqtt();
#endif
	void setup_web_server();

	void goto_setup_mode();

	void update_effect();
	void update_button();

	void update_display(unsigned long delta_ms);
	void update_effect_display();

	void update_save(unsigned long delta_ms);

	JsonDocument load_config();
	void save_config(JsonDocument *doc);
#ifdef USE_NTP
	NTPClient *ntpClient;
#endif
#ifdef USE_MQTT
	AsyncMqttClient mqtt;
#endif

	unsigned long remote_time_ms, loop_time_ms;
	unsigned short save_interval;

	FieryButton button;
	bool button_down;
	unsigned long button_down_time, button_up_time;
	unsigned char button_down_count;

	unsigned long current_time;
	FieryLedLampConfig config;

	//FieryDisplay display;
	Display display;
	time_t display_update_time;
	int pos_x;
};

extern FieryLedLamp lamp;
#endif
