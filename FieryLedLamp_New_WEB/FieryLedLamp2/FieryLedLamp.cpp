#include"FieryLedLamp.h"
#include"Constants.h"

#ifdef USE_NTP
#include <WiFiUdp.h>
WiFiUDP ntpUdp;
#endif

#ifdef USE_MQTT
void mqttOnConnect(bool sessionPresent)
{
	lamp.setup_mqtt_subscribe();
};
void mqttOnMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total)
{
}
#endif

void FieryLedLamp::setup()
{
	setup_config();
	setup_pin();
	setup_mqtt();
};

bool FieryLedLamp::setup_config()
{
	"config.json";
	return true;
};
void FieryLedLamp::setup_pin()
{
/*#if defined(ESP_USE_BUTTON) && defined(BUTTON_LOCK_ON_START)
	pinMode(BTN_PIN, INPUT);
#if (BUTTON_IS_SENSORY == 1)
	if (digitalRead(BTN_PIN)) {
		buttonBlocing = true;
	}
#endif
#if (BUTTON_IS_SENSORY == 0)
	if (!digitalRead(BTN_PIN)) {
		buttonBlocing = true;
	}
#endif
#endif*/
	// ѕ»Ќџ
#ifdef MOSFET_PIN                                         // инициализаци€ пина, управл€ющего MOSFET транзистором в состо€ние "выключен"
	pinMode(MOSFET_PIN, OUTPUT);
#ifdef MOSFET_LEVEL
	digitalWrite(MOSFET_PIN, !MOSFET_LEVEL);
#endif
#endif

#ifdef ALARM_PIN                                          // инициализаци€ пина, управл€ющего будильником в состо€ние "выключен"
	pinMode(ALARM_PIN, OUTPUT);
#ifdef ALARM_LEVEL
	digitalWrite(ALARM_PIN, !ALARM_LEVEL);
#endif
#endif
};
void FieryLedLamp::setup_time()
{
#ifdef USE_NTP
	ntpClient = new NTPClient(ntpUdp);
#endif
};
bool FieryLedLamp::setup_mqtt()
{
#ifdef USE_MQTT
	std::string clientId = "lamp";
	mqtt.setServer("mqtt.dealgate.ru", 1883);
	mqtt.setCredentials("","");
	mqtt.setWill(MQTT_STATUS_TOPIC,0,true,"disconnect");
	mqtt.setClientId(clientId.c_str());

	mqtt.onConnect(mqttOnConnect);
	mqtt.onMessage(mqttOnMessage);
#endif
	return true;
};
void FieryLedLamp::setup_mqtt_subscribe()
{
#ifdef USE_MQTT
	mqtt.subscribe(MQTT_COMMAND_TOPIC, 0);
	mqtt.publish(MQTT_STATUS_TOPIC, 0, true, "connect");
#endif
};
void FieryLedLamp::update_time()
{
#ifdef USE_NTP
	ntpClient->update();
#endif
};
void FieryLedLamp::update_effect()
{

};
void FieryLedLamp::update()
{
	update_effect();

	if (WiFi.isConnected() == false)
	{
		WiFi.begin();
		return;
	}
	if (mqtt.connected() == false)
	{
		mqtt.connect();
		return;
	}
};