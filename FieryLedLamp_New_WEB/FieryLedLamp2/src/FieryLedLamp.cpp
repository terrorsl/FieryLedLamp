#include"FieryLedLamp.h"
#include"Constants.h"

#include<WiFiManager.h>

#ifdef USE_NTP
#include <WiFiUdp.h>
WiFiUDP ntpUdp;
#endif

#include <TZ.h>

extern CRGB leds[NUM_LEDS];

void onWiFiConnected(const WiFiEventStationModeConnected &)
{
	lamp.connect_web();
	lamp.connect_mqtt();
};
void onWiFiDisconnected(const WiFiEventStationModeDisconnected &)
{
};

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

		config.scale=50;
		config.speed=50;
		config.brightness=BRIGHTNESS;
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
		config.scale=doc["scale"].as<uint8_t>();
		config.speed=doc["speed"].as<uint8_t>();
		config.brightness=doc["brightness"].as<uint8_t>();
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
	configTime(TZ_Europe_Moscow, "pool.ntp.org");
#ifdef USE_NTP
	ntpClient = new NTPClient(ntpUdp);
	ntpClient->setUpdateInterval(60*60000);
#endif
};
void FieryLedLamp::connect_web()
{
	web->begin();
};
void FieryLedLamp::update_time()
{
#ifdef USE_NTP
	if(ntpClient->update())
	{
	}
#endif
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

	if(state)
		FastLED.setBrightness(config.brightness);
	else
		FastLED.setBrightness(0);
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
#if defined(USE_MQTT)
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
#endif
				if(manager.startConfigPortal(config.mqtt.clientid.c_str(), "12345678"))
				{
					JsonDocument doc=load_config();
#if defined(USE_MQTT)
					doc["mqtt_server"]=server.getValue();
					doc["mqtt_port"]=atol(port.getValue());
					doc["mqtt_login"]=login.getValue();
					doc["mqtt_password"]=password.getValue();
					doc["mqtt_clientid"]=clientid.getValue();
					doc["mqtt_keepalive"]=atol(keepalive.getValue());
#endif
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
	update_time();
	update_effect();
	update_button();
	web->handleClient();
};
void FieryLedLamp::next_effect()
{
	unsigned short effect=config.currentEffect+1;
	if(effect>=FieryLedLampEffectTypes::MaxEffect)
		effect=0;
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
void FieryLedLamp::set_speed(uint8_t speed)
{
	config.speed=speed;
	config.effect->set_speed(speed);
};
bool FieryLedLamp::change_effect(unsigned short index)
{
	DBG_PRINT("change_effect:%d\n", index);
	if(config.currentEffect==index)
		return true;
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
	case FieryLedLampEffectTypes::Pool:
		config.effect=new FieryLedLampEffectPool();
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
	case FieryLedLampEffectTypes::Waterfall4_1:
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
	case FieryLedLampEffectTypes::WhirlMulti:
		config.effect=new FieryLedLampEffectWhirl(false);
		break;
	case FieryLedLampEffectTypes::StarFall:
		config.effect=new FieryLedLampEffectStarFall();
		break;
	case FieryLedLampEffectTypes::StormyRain:
		//config.effect=new FieryLedLampEffectStarFall();
		//break;
	case FieryLedLampEffectTypes::DNA:
		config.effect=new FieryLedLampEffectDNA();
		break;
	case FieryLedLampEffectTypes::Smoke:
		config.effect=new FieryLedLampEffectSmoke(false);
		break;
	case FieryLedLampEffectTypes::SmokeColor:
		config.effect=new FieryLedLampEffectSmoke(true);
		break;
	case FieryLedLampEffectTypes::SmokeBalls:
		config.effect=new FieryLedLampEffectSmokeBalls();
		break;
	case FieryLedLampEffectTypes::LiqudLamp:
		config.effect=new FieryLedLampEffectLiquidLamp(true);
		break;
	case FieryLedLampEffectTypes::LiqudLampAuto:
		config.effect=new FieryLedLampEffectLiquidLamp(false);
		break;
	case FieryLedLampEffectTypes::Swirl:
		config.effect=new FieryLedLampEffectSwirl();
		break;
	case FieryLedLampEffectTypes::Stars:
		config.effect=new FieryLedLampEffectStars();
		break;
	case FieryLedLampEffectTypes::Zebra:
		config.effect=new FieryLedLampEffectZebra();
		break;
	case FieryLedLampEffectTypes::TixyLand:
		config.effect=new FieryLedLampEffectTixyLand();
		break;
	case FieryLedLampEffectTypes::Snakes:
		config.effect=new FieryLedLampEffectSnakes();
		break;
	case FieryLedLampEffectTypes::Fountain:
		config.effect=new FieryLedLampEffectFontain();
		break;
	case FieryLedLampEffectTypes::DropInWater:
		config.effect=new FieryLedLampEffectDropInWater();
		break;
	case FieryLedLampEffectTypes::Drops:
		config.effect=new FieryLedLampEffectDrops();
		break;
	case FieryLedLampEffectTypes::Rings:
		config.effect=new FieryLedLampEffectRings();
		break;
	case FieryLedLampEffectTypes::Comet:
		config.effect=new FieryLedLampEffectComet();
		break;
	case FieryLedLampEffectTypes::CometColor:
		config.effect=new FieryLedLampEffectCometColor();
		break;
	case FieryLedLampEffectTypes::Comet2:
		config.effect=new FieryLedLampEffectCometCount(2);
		break;
	case FieryLedLampEffectTypes::Comet3:
		config.effect=new FieryLedLampEffectCometCount(3);
		break;
	case FieryLedLampEffectTypes::Contacts:
		config.effect=new FieryLedLampEffectContacts();
		break;
	case FieryLedLampEffectTypes::Sparkles:
		config.effect=new FieryLedLampEffectSparkles();
		break;
	case FieryLedLampEffectTypes::Cube2D:
		config.effect=new FieryLedLampEffectCube2D();
		break;
	case FieryLedLampEffectTypes::Lava:
		config.effect=new FieryLedLampEffectLava();
		break;
	case FieryLedLampEffectTypes::LavaLamp:
		config.effect=new FieryLedLampEffectLavaLamp();
		break;
	case FieryLedLampEffectTypes::ButterflyLamp:
		config.effect=new FieryLedLampEffectButterflyLamp(false);
		break;
	case FieryLedLampEffectTypes::Forest:
		config.effect=new FieryLedLampEffectForest();
		break;
	case FieryLedLampEffectTypes::Lumenjer:
		config.effect=new FieryLedLampEffectLumenjer();
		break;
	case FieryLedLampEffectTypes::Magma:
		config.effect=new FieryLedLampEffectMagma();
		break;
	case FieryLedLampEffectTypes::Paints:
		config.effect=new FieryLedLampEffectOilPaints();
		break;
	case FieryLedLampEffectTypes::Matrix:
		config.effect=new FieryLedLampEffectMatrix();
		break;
	case FieryLedLampEffectTypes::Twinkles:
		config.effect=new FieryLedLampEffectTwinkles();
		break;
	case FieryLedLampEffectTypes::Metaballs:
		config.effect=new FieryLedLampEffectMetaballs();
		break;
	case FieryLedLampEffectTypes::Mosaic:
		config.effect=new FieryLedLampEffectMosaic();
		break;
	case FieryLedLampEffectTypes::Butterflys:
		config.effect=new FieryLedLampEffectButterflyLamp(true);
		break;
	case FieryLedLampEffectTypes::BBalls:
		config.effect=new FieryLedLampEffectBballs();
		break;
	case FieryLedLampEffectTypes::BallsBounce:
		config.effect=new FieryLedLampEffectBallsBounce();
		break;
	case FieryLedLampEffectTypes::ChristmasTree:
		config.effect=new FieryLedLampEffectChristmasTree();
		break;
	case FieryLedLampEffectTypes::Fire:
		config.effect=new FieryLedLampEffectFire();
		break;
	case FieryLedLampEffectTypes::Fire2012:
		config.effect=new FieryLedLampEffectFire2012();
		break;
	case FieryLedLampEffectTypes::Fire2018:
		config.effect=new FieryLedLampEffectFire2018();
		break;
	case FieryLedLampEffectTypes::Fire2020:
		config.effect=new FieryLedLampEffectFire2020();
		break;
	case FieryLedLampEffectTypes::Fire2021:
		config.effect=new FieryLedLampEffectFire2021();
		break;
	case FieryLedLampEffectTypes::FireFlyTop:
		config.effect=new FieryLedLampEffectFireFlyTop();
		break;
	default:
		DBG_PRINT("unknown effect:%d\n", index);
		return false;
	}
	config.currentEffect=index;

	config.effect->set_speed(config.speed);
	config.effect->set_scale(config.scale);
	config.effect->set_bright(config.brightness);
	
	config.effect->setup();
	if(current)
		delete current;
	return true;
};