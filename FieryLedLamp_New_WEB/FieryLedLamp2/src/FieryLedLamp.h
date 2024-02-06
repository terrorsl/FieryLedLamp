#ifndef FIERY_LED_LAMP_FILE
#define FIERY_LED_LAMP_FILE

#include"platform.h"
#include"Constants.h"
#include"effect.h"
#include"lang.h"

#include<ArduinoJson.h>

#include<Wire.h>
#include <Adafruit_SSD1306.h>
#include <U8g2_for_Adafruit_GFX.h>
//#include<SSD1306Wire.h>

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
	bool power_state;
	
	Languages language;
	//const char **effect_name;

	unsigned short currentEffect;
	FieryLedLampEffect *effect;
#if defined(USE_MQTT)
	FieryLedLampConfigMQTT mqtt;
#endif
	unsigned char speed, scale, brightness;
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

	bool change_effect(unsigned short index);
	void next_effect();
	void prev_effect();

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

	void update_effect();
	void update_button();

	void update_display();

	JsonDocument load_config();
	void save_config(JsonDocument *doc);
#ifdef USE_NTP
	NTPClient *ntpClient;
#endif
#ifdef USE_MQTT
	AsyncMqttClient mqtt;
#endif

	unsigned long remote_time_ms;

	bool button_down;
	unsigned long button_down_time, button_up_time;
	unsigned char button_down_count;

	unsigned long current_time;
	FieryLedLampConfig config;

	Adafruit_SSD1306 *display;
	U8G2_FOR_ADAFRUIT_GFX u8g2_for_adafruit_gfx;
	time_t display_update_time;
	int pos_x;
};

extern FieryLedLamp lamp;
#endif
