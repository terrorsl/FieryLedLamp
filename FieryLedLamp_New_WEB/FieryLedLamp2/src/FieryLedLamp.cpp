#include"FieryLedLamp.h"
#include"Constants.h"

#include<WiFiManager.h>

#ifdef USE_NTP
#include <WiFiUdp.h>
WiFiUDP ntpUdp;
#endif

void onWiFiConnected(const WiFiEventStationModeConnected &)
{
	lamp.connect_web();
	lamp.connect_mqtt();
};
void onWiFiDisconnected(const WiFiEventStationModeDisconnected &)
{
};

#ifdef USE_MQTT
void mqttOnConnect(bool sessionPresent)
{
	lamp.setup_mqtt_subscribe();
};
void mqttDisconnect(AsyncMqttClientDisconnectReason reason)
{
	lamp.connect_mqtt();
};
void mqttOnMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total)
{
	lamp.update_mqtt(payload);
};
#endif

void FieryLedLamp::setup()
{
	config.power_state=false;
	
	web=new WebServer(80);
	
	setup_pin();
	setup_config();
	setup_mqtt();
	setup_web_server();

	WiFi.onStationModeConnected(onWiFiConnected);
	WiFi.onStationModeDisconnected(onWiFiDisconnected);

	WiFi.persistent(true);
};

void FieryLedLamp::setup_config()
{
#if defined(ESP8266)
	LittleFS.begin();
#else
	LittleFS.begin(true);
#endif
	JsonDocument doc=load_config();
	
	if(doc.isNull())
	{
		config.mqtt.server="mqtt.dealgate.ru";
		config.mqtt.port=1883;
		config.mqtt.login="";
		config.mqtt.password="";
		config.mqtt.keep_alive=MQTT_KEEPALIVE;
#if defined(ESP8266)
		unsigned long id=ESP.getChipId();
#else
#endif
		char name[256];
		sprintf(name,"ledLamp%lx",id);
		config.mqtt.clientid=name;
	}
	else
	{
		config.mqtt.server=doc["mqtt_server"].as<std::string>();
		config.mqtt.port=doc["mqtt_port"].as<uint16_t>();
		config.mqtt.login=doc["mqtt_login"].as<std::string>();
		config.mqtt.password=doc["mqtt_password"].as<std::string>();
		config.mqtt.keep_alive=doc["mqtt_keepalive"].as<uint16_t>();
		config.mqtt.clientid=doc["mqtt_clientid"].as<std::string>();
	}
};

JsonDocument FieryLedLamp::load_config()
{
	JsonDocument doc;
	fs::File file=LittleFS.open("/config.json", "r");
	if(file)
	{
		deserializeJson(doc, file);
		file.close();
	}
	return doc;
};
void FieryLedLamp::save_config(JsonDocument *doc)
{
	fs::File file=LittleFS.open("/config.json", "w");
	if(file)
	{
		serializeJson(*doc, file);
		file.close();
	}
};

void FieryLedLamp::setup_pin()
{
#if BUTTON_IS_SENSORY
	pinMode(BUTTON_PIN, INPUT);
#else
	pinMode(BUTTON_PIN, INPUT_PULLUP);
#endif

	pinMode(LED_PIN, OUTPUT);

	pinMode(MOSFET_PIN, OUTPUT);
#ifdef MOSFET_LEVEL
	digitalWrite(MOSFET_PIN, !MOSFET_LEVEL);
#endif

#ifdef ALARM_PIN                                          // ������������� ����, ������������ ����������� � ��������� "��������"
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
void FieryLedLamp::connect_web()
{
	web->begin();
};
#ifdef USE_MQTT
void FieryLedLamp::setup_mqtt()
{
	mqtt.setServer(config.mqtt.server.c_str(), config.mqtt.port);
	mqtt.setCredentials(config.mqtt.login.c_str(), config.mqtt.password.c_str());
	mqtt.setKeepAlive(config.mqtt.keep_alive);
	mqtt.setClientId(config.mqtt.clientid.c_str());

	mqtt.onConnect(mqttOnConnect);
	mqtt.onMessage(mqttOnMessage);
	mqtt.onDisconnect(mqttDisconnect);
};
void FieryLedLamp::setup_mqtt_subscribe()
{
	std::string topic = config.mqtt.clientid + MQTT_COMMAND_TOPIC;
	mqtt.subscribe(topic.c_str(), 0);

	JsonDocument doc;
	doc["power"]=config.power_state;
	doc["effect"]=config.currentEffect;
	
	topic=config.mqtt.clientid + MQTT_RESULT_TOPIC;
	String payload;
	serializeJson(doc,payload);
	mqtt.publish(topic.c_str(), 0, true, payload.c_str());
};
#endif
void FieryLedLamp::update_time()
{
#ifdef USE_NTP
	ntpClient->update();
#endif
};
void FieryLedLamp::connect_mqtt()
{
	mqtt.connect();
};
void FieryLedLamp::update_effect()
{
	if(config.power_state && config.effect)
		config.effect->update();
};
void FieryLedLamp::power_button(bool state)
{
	config.power_state=state;
	digitalWrite(MOSFET_PIN, state);
};
void FieryLedLamp::update_button()
{
#if BUTTON_IS_SENSORY
	if(digitalRead(BUTTON_PIN)==HIGH)
#else
	if(digitalRead(BUTTON_PIN)==LOW)
#endif
	{
		if(button_down)
		{
			unsigned long delta=millis()-button_down_time;
			if(delta>=SETUP_BUTTON_TIME)
			{
				digitalWrite(BUILDIN_LED_PIN, LOW);

				web->stop();

				WiFiManager manager;
				WiFiManagerParameter server("mqtt_server","MQTT Server",config.mqtt.server.c_str(),40);
				WiFiManagerParameter port("mqtt_port","MQTT Port",String(config.mqtt.port).c_str(),6);
				WiFiManagerParameter login("mqtt_login","MQTT Login",config.mqtt.login.c_str(),40);
				WiFiManagerParameter password("mqtt_password","MQTT Password",config.mqtt.password.c_str(),40);
				WiFiManagerParameter clientid("mqtt_clientid","MQTT Client ID",config.mqtt.clientid.c_str(),40);
				WiFiManagerParameter keepalive("mqtt_keepalive","MQTT Keep alive",String(config.mqtt.keep_alive).c_str(),6);
				manager.addParameter(&server);
				manager.addParameter(&port);
				manager.addParameter(&login);
				manager.addParameter(&password);
				manager.addParameter(&clientid);
				manager.addParameter(&keepalive);
				if(manager.startConfigPortal(config.mqtt.clientid.c_str(), "12345678"))
				{
					JsonDocument doc=load_config();
					doc["mqtt_server"]=server.getValue();
					doc["mqtt_port"]=atol(port.getValue());
					doc["mqtt_login"]=login.getValue();
					doc["mqtt_password"]=password.getValue();
					doc["mqtt_clientid"]=clientid.getValue();
					doc["mqtt_keepalive"]=atol(keepalive.getValue());
					save_config(&doc);
				}
				web->begin();

				digitalWrite(BUILDIN_LED_PIN, HIGH);
				button_down=false;
			}
		}
		else
		{
			button_down=true;
			button_down_time=millis();
		}
	}
	else
	{
		if(button_down)
		{
			power_button(!config.power_state);
			button_down=false;
		}
	}
};
void FieryLedLamp::update()
{
	update_effect();

	update_button();

	web->handleClient();
};
void FieryLedLamp::change_effect(unsigned short index)
{
	if(config.currentEffect==index)
		return;
	FieryLedLampEffect *current=config.effect;
	switch(index)
	{
	case FieryLedLampEffectTypes::WhiteColor:
		config.effect=new FieryLedLampEffectWhiteColorStripeRoutine();
		break;
	case FieryLedLampEffectTypes::WaterColor:
		config.effect=new FieryLedLampEffectWaterColor();
		break;
	default:
		return;
	}
	config.currentEffect=index;
	config.effect->setup();
	delete current;
};