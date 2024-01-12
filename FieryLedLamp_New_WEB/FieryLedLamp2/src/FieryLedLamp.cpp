#include"FieryLedLamp.h"
#include"Constants.h"

#include<WiFiManager.h>

#ifdef USE_NTP
#include <WiFiUdp.h>
WiFiUDP ntpUdp;
#endif

extern CRGB leds[NUM_LEDS];

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
	DBG_START(9600);
	DBG_PRINT("start");
	
	config.power_state=false;
	
	web=new WebServer(80);
	
	setup_pin();
	
	FastLED.addLeds<WS2812B,LED_PIN,GRB>(leds,NUM_LEDS);
	FastLED.setBrightness(BRIGHTNESS);
  	/*if (current_limit > 0)
  	{
    	FastLED.setMaxPowerInVoltsAndMilliamps(5, current_limit);
  	}*/
	FastLED.setMaxPowerInVoltsAndMilliamps(5, 3000);
  	FastLED.clear();
  	FastLED.show();
	
	setup_config();
	setup_mqtt();
	setup_web_server();

	WiFi.onStationModeConnected(onWiFiConnected);
	WiFi.onStationModeDisconnected(onWiFiDisconnected);

	WiFi.persistent(true);
};

void FieryLedLamp::setup_config()
{
	Serial.println("setup_config");
#if defined(ESP8266)
	LittleFS.begin();
#else
	LittleFS.begin(true);
#endif
	JsonDocument doc=load_config();
	
	config.currentEffect = 0xff;

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

		change_effect(0);
	}
	else
	{
		config.mqtt.server=doc["mqtt_server"].as<std::string>();
		config.mqtt.port=doc["mqtt_port"].as<uint16_t>();
		config.mqtt.login=doc["mqtt_login"].as<std::string>();
		config.mqtt.password=doc["mqtt_password"].as<std::string>();
		config.mqtt.keep_alive=doc["mqtt_keepalive"].as<uint16_t>();
		config.mqtt.clientid=doc["mqtt_clientid"].as<std::string>();

		change_effect(doc["effect"].as<uint16_t>());
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
	DBG_PRINT("setup_pin");

	pinMode(BUILDIN_LED_PIN, OUTPUT);
	digitalWrite(BUILDIN_LED_PIN, HIGH);

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
	DBG_PRINT("power state:%d\n",state);
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
				DBG_PRINT("setup mode\n");
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
			/*else
			{
				if(delta>=POWER_BUTTON_TIME)
				{
					DBG_PRINT("power mode\n");
					power_button(!config.power_state);
					button_down=false;
				}
			}*/
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
			button_down=false;
			unsigned long delta=millis()-button_up_time;
			if(delta<=DELTA_BUTTON_DOWN)
			{
				button_down_count++;
			}
			else
				button_down_count=1;
			button_up_time=millis();
			DBG_PRINT("delta:%d\n",delta);
		}
		else
		{
			if(button_down_count && millis()-button_up_time>DELTA_BUTTON_DOWN)
			{
				switch(button_down_count)
				{
				case POWER_BUTTON_COUNT:
					power_button(!config.power_state);
					break;
				case NEXT_BUTTON_COUNT:
					DBG_PRINT("next mode\n");
					next_effect();
					break;
				case BEFOR_BUTTON_COUNT:
					DBG_PRINT("before mode\n");
					prev_effect();
					break;
				}
				button_down_count=0;
			}
		}
	}
};
void FieryLedLamp::update()
{
	update_effect();
	update_button();
	web->handleClient();
};
void FieryLedLamp::next_effect()
{
	unsigned short effect=config.currentEffect+1;
	if(effect>=FieryLedLampEffectTypes::MaxEffect)
		config.currentEffect=0;
	change_effect(effect);
};
void FieryLedLamp::prev_effect()
{
	unsigned short effect=config.currentEffect;
	if(effect==0)
		effect=FieryLedLampEffectTypes::MaxEffect;
	effect--;
	change_effect(effect);
};
void FieryLedLamp::change_effect(unsigned short index)
{
	DBG_PRINT("change_effect:%d", index);
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
	case FieryLedLampEffectTypes::FlowerRuta:
		config.effect=new FieryLedLampEffectFlowerRuta();
		break;
	case FieryLedLampEffectTypes::Bamboo:
		config.effect=new FieryLedLampEffectBamboo();
		break;
	case FieryLedLampEffectTypes::Madness:
		config.effect=new FieryLedLampEffectMadnessNoise();
		break;
	case FieryLedLampEffectTypes::Ball:
		config.effect=new FieryLedLampEffectBall();
		break;
	case FieryLedLampEffectTypes::Waterfall:
		config.effect=new FieryLedLampEffectWaterfall();
		break;
	case FieryLedLampEffectTypes::Waves:
		config.effect=new FieryLedLampEffectWave();
		break;
	case FieryLedLampEffectTypes::MagicLantern:
		config.effect=new FieryLedLampEffectMagicLantern();
		break;
	case FieryLedLampEffectTypes::Wine:
		config.effect=new FieryLedLampEffectWine();
		break;
	case FieryLedLampEffectTypes::Whirl:
		config.effect=new FieryLedLampEffectWhirl(true);
		break;
	default:
		return;
	}
	config.currentEffect=index;
	config.effect->setup();
	if(current)
		delete current;
};