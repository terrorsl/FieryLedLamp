#include "FieryLedLamp.h"
#include "Constants.h"

#include "lang_ru.h"

#include <WiFiManager.h>

#ifdef USE_NTP
#include <WiFiUdp.h>
WiFiUDP ntpUdp;
#endif

#include <TZ.h>

extern CRGB leds[NUM_LEDS];

WiFiEventHandler stationConnectedHandler;

void onWiFiConnected(const WiFiEventStationModeConnected &)
{
	DBG_PRINT("onWiFiConnected\n");
	// lamp.connect_web();
	// lamp.connect_mqtt();
};
void onWiFiDisconnected(const WiFiEventStationModeDisconnected &)
{
	DBG_PRINT("onWiFiDisconnected\n");
	WiFi.reconnect();
};

void FieryLedLamp::setup()
{
	categoryes[FieryLedLampEffectCategoryType::FieryLedLampEffectCategoryType_Fire].effects = fire_effects;

	DBG_START(115200);
	DBG_PRINT("start\n");

	config.power_state = false;

	setup_pin();

	setup_display();

	setup_config();
	setup_mqtt();
	setup_web_server();

	WiFi.onStationModeDisconnected(&onWiFiDisconnected);

	WiFi.setAutoConnect(true);

	WiFi.persistent(true);
	WiFi.begin();
};

void FieryLedLamp::setup_config()
{
	Serial.println("setup_config");
#if defined(ESP8266)
	LittleFS.begin();
#else
	LittleFS.begin(true);
#endif
	JsonDocument doc = load_config();

	config.effect = 0;

	if (doc.isNull())
	{
		config.mqtt.server = "mqtt.dealgate.ru";
		config.mqtt.port = 1883;
		config.mqtt.login = "";
		config.mqtt.password = "";
		config.mqtt.keep_alive = MQTT_KEEPALIVE;
#if defined(ESP8266)
		unsigned long id = ESP.getChipId();
#else
#endif
		char name[256];
		sprintf(name, "ledLamp%lx", id);
		config.mqtt.clientid = name;

		config.scale = 50;
		config.speed = 50;
		config.brightness = BRIGHTNESS;
		config.currentEffect = 0;
	}
	else
	{
		config.mqtt.server = doc["mqtt_server"].as<std::string>();
		config.mqtt.port = doc["mqtt_port"].as<uint16_t>();
		config.mqtt.login = doc["mqtt_login"].as<std::string>();
		config.mqtt.password = doc["mqtt_password"].as<std::string>();
		config.mqtt.keep_alive = doc["mqtt_keepalive"].as<uint16_t>();
		config.mqtt.clientid = doc["mqtt_clientid"].as<std::string>();

		if (doc.containsKey("effect"))
			config.currentEffect = doc["effect"].as<uint16_t>();
		else
			config.currentEffect = 0;
		config.scale = doc["scale"].as<uint8_t>();
		config.speed = doc["speed"].as<uint8_t>();
		config.brightness = doc["brightness"].as<uint8_t>();
	}

	config.language.setLanguage(RUSSIAN);
	// config.effect_name=effect_name_ru;

	DBG_PRINT("mqtt_server:%s\n", config.mqtt.server.c_str());
	DBG_PRINT("mqtt_port:%d\n", config.mqtt.port);
	DBG_PRINT("mqtt_login:%s\n", config.mqtt.login.c_str());
	DBG_PRINT("mqtt_pass:%s\n", config.mqtt.password.c_str());

	DBG_PRINT("effect:%d\n", config.currentEffect);
	DBG_PRINT("scale:%d\n", config.scale);
	DBG_PRINT("speed:%d\n", config.speed);

	FastLED.setBrightness(config.brightness);
	change_effect(config.currentEffect);
	DBG_PRINT("setup_config done\n");
};

JsonDocument FieryLedLamp::load_config()
{
	JsonDocument doc;
	if (LittleFS.exists("/config.json.tmp"))
	{
		DBG_PRINT("swap config\n");
		fs::File file = LittleFS.open("/config.json.tmp", "r");
		if (file)
		{
			/*while(file.available())
				DBG_PRINT("%s:\n", file.readString().c_str());
			file.seek(0);*/
			if (deserializeJson(doc, file) == DeserializationError::Ok)
			{
				file.close();
				LittleFS.remove("/config.json");
				LittleFS.rename("/config.json.tmp", "/config.json");
				return doc;
			}
		}
	}
	fs::File file = LittleFS.open("/config.json", "r");
	if (file)
	{
		while (file.available())
			DBG_PRINT("%s:\n", file.readString().c_str());
		file.seek(0);
		if (deserializeJson(doc, file) != DeserializationError::Ok)
		{
			DBG_PRINT("error:\n");
			doc.clear();
		}
		file.close();
	}
	return doc;
};
void FieryLedLamp::save_config(JsonDocument *doc)
{
	fs::File file = LittleFS.open("/config.json.tmp", "w");
	if (file)
	{
		serializeJson(*doc, file);
		file.close();
	}
};

void FieryLedLamp::setup_pin()
{
	DBG_PRINT("setup_pin\n");

	pinMode(BUILDIN_LED_PIN, OUTPUT);
	digitalWrite(BUILDIN_LED_PIN, HIGH);

#if BUTTON_IS_SENSORY
	pinMode(BUTTON_PIN, INPUT);
#else
	pinMode(BUTTON_PIN, INPUT_PULLUP);
#endif
	button.is_down = false;
	button.klick_count = 0;

	pinMode(LED_PIN, OUTPUT);

	pinMode(MOSFET_PIN, OUTPUT);
#ifdef MOSFET_LEVEL
	digitalWrite(MOSFET_PIN, !MOSFET_LEVEL);
#endif

#ifdef ALARM_PIN
	pinMode(ALARM_PIN, OUTPUT);
#ifdef ALARM_LEVEL
	digitalWrite(ALARM_PIN, !ALARM_LEVEL);
#endif
#endif

	//memset(leds, 255, sizeof(leds));

	FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
	FastLED.setMaxPowerInVoltsAndMilliamps(5, 3000);
	FastLED.clear();
};
void FieryLedLamp::setup_display()
{
	Wire.begin(DISPLAY_SDA, DISPLAY_SCL);
	display.init();

	pos_x = display.width();
	// display_update_time=0;
};
void FieryLedLamp::setup_time()
{
	configTime(TZ_Europe_Moscow, "pool.ntp.org");
#ifdef USE_NTP
	ntpClient = new NTPClient(ntpUdp);
	ntpClient->setUpdateInterval(60 * 60000);
#endif
};
void FieryLedLamp::update_time()
{
#ifdef USE_NTP
	if (ntpClient->update())
	{
	}
#endif
};
void FieryLedLamp::update_effect()
{
	if (config.power_state && config.effect)
		config.effect->update();
};
void FieryLedLamp::power_button(bool state)
{
	DBG_PRINT("power state:%d\n", state);
	config.power_state = state;
	digitalWrite(MOSFET_PIN, state);

	// display_update_time=time(0);

	if (state == true)
		display.set_state(ON_STATE);
	else
		display.set_state(OFF_STATE);

	/*if (state)
		FastLED.setBrightness(config.brightness);
	else
		FastLED.setBrightness(0);*/
	if(state==false)
	{
		//FastLED.clear(true);
		if(config.effect)
			config.effect->clear();
	}
	else
	{
		if(config.effect)
			config.effect->setup();
	}
	FastLED.show();

	update_effect_display();
};
void FieryLedLamp::goto_setup_mode()
{
	DBG_PRINT("setup mode\n");
	digitalWrite(BUILDIN_LED_PIN, LOW);

	WiFiManager manager;
#if defined(USE_MQTT)
	WiFiManagerParameter server("mqtt_server", "MQTT Server", config.mqtt.server.c_str(), 40);
	WiFiManagerParameter port("mqtt_port", "MQTT Port", String(config.mqtt.port).c_str(), 6);
	WiFiManagerParameter login("mqtt_login", "MQTT Login", config.mqtt.login.c_str(), 40);
	WiFiManagerParameter password("mqtt_password", "MQTT Password", config.mqtt.password.c_str(), 40);
	WiFiManagerParameter clientid("mqtt_clientid", "MQTT Client ID", config.mqtt.clientid.c_str(), 40);
	WiFiManagerParameter keepalive("mqtt_keepalive", "MQTT Keep alive", String(config.mqtt.keep_alive).c_str(), 6);
	manager.addParameter(&server);
	manager.addParameter(&port);
	manager.addParameter(&login);
	manager.addParameter(&password);
	manager.addParameter(&clientid);
	manager.addParameter(&keepalive);
#endif
	if (manager.startConfigPortal(config.mqtt.clientid.c_str(), "12345678"))
	{
		JsonDocument doc = load_config();
#if defined(USE_MQTT)
		doc["mqtt_server"] = server.getValue();
		doc["mqtt_port"] = atol(port.getValue());
		doc["mqtt_login"] = login.getValue();
		doc["mqtt_password"] = password.getValue();
		doc["mqtt_clientid"] = clientid.getValue();
		doc["mqtt_keepalive"] = atol(keepalive.getValue());
#endif
		save_config(&doc);
	}
};
void FieryLedLamp::update_button()
{
	if (digitalRead(BUTTON_PIN) == BUTTON_IS_SENSORY)
	{
		if (button.is_down)
		{
			unsigned long delta = millis() - button.down_time;
			if (config.power_state == false)
			{
				if (delta >= SETUP_BUTTON_TIME)
				{
					button.state = FieryButtonSetupState;
				}
			}
			else
			{
				if (delta >= POWER_BUTTON_TIME)
				{
					button.state = FieryButtonPowerState;
				}
			}
		}
		else
		{
			button.is_down = true;
			button.down_time = millis();
		}
	}
	else
	{
		if (button.is_down)
		{
			button.is_down = false;
			unsigned long delta = millis() - button.down_time;
			if (delta <= DELTA_BUTTON_DOWN)
			{
				button.klick_count++;
				DBG_PRINT("delta %ld, %d\n", delta, button.klick_count);
			}
			else
				button.klick_count = 1;
		}
		else
		{
			unsigned long delta = millis() - button.down_time;
			if (delta >= DELTA_BUTTON_DOWN && button.klick_count)
			{
				if (config.power_state)
				{
					switch (button.state)
					{
					case FieryButtonUnknownState:
						switch (button.klick_count)
						{
						case NEXT_BUTTON_COUNT:
							DBG_PRINT("next mode\n");
							next_effect();
							break;
						case BEFOR_BUTTON_COUNT:
							DBG_PRINT("before mode\n");
							prev_effect();
							break;
						}
						display.set_state(ON_STATE);
						break;
					case FieryButtonSetupState:
						goto_setup_mode();
						digitalWrite(BUILDIN_LED_PIN, HIGH);
						break;
					case FieryButtonPowerState:
						power_button(false);
						display.set_state(OFF_STATE, true);
						break;
					}
				}
				else
				{
					power_button(true);
					display.set_state(ON_STATE, true);
				}
				button.state = FieryButtonUnknownState;
				button.klick_count = 0;
			}
		}
	}
};
void FieryLedLamp::update_save(unsigned long delta_ms)
{
	save_interval += delta_ms;
	if (save_interval < SAVE_TIME_INTERVAL || config.need_save == false)
		return;
	save_interval = 0;
	config.need_save = false;
	JsonDocument doc = load_config();

	doc["scale"] = config.scale;
	doc["speed"] = config.speed;
	doc["brightness"] = config.brightness;
	doc["effect"] = config.currentEffect;

#if defined(USE_MQTT)
	doc["mqtt_server"] = config.mqtt.server;
	doc["mqtt_port"] = config.mqtt.port;
	doc["mqtt_login"] = config.mqtt.login;
	doc["mqtt_password"] = config.mqtt.password;
	doc["mqtt_clientid"] = config.mqtt.clientid;
#endif
	save_config(&doc);
};
void FieryLedLamp::update()
{
	unsigned long t = millis();

	unsigned long delta = t - loop_time_ms;

	update_time();
	update_display(delta);
	update_effect();
	update_button();

	update_save(delta);

	if (WiFi.isConnected() == false && t - remote_time_ms > 2000)
	{
		// DBG_PRINT("Try connect to WIFI");
		remote_time_ms = t;
		return;
	}
	else
	{
		if (mqtt.connected() == false && t - remote_time_ms > 2000)
		{
			connect_mqtt();
			remote_time_ms = t;
		}
	}
	if (delta)
		loop_time_ms = t;
};
void FieryLedLamp::update_display(unsigned long delta_ms)
{
	if (delta_ms == 0)
		return;
	static unsigned short sec = 1000;
	if (config.power_state == false)
	{
		if (delta_ms >= sec)
		{

			sec = 1000;
			time_t t = time(0);
			tm timeSt;
			gmtime_r(&t, &timeSt);

			char str[32];
			sprintf(str, "%02d:%02d:%02d", timeSt.tm_hour, timeSt.tm_min, timeSt.tm_sec);
			display.draw(str);
		}
		else
		{
			sec -= delta_ms;
		}
	}
	display.update(delta_ms);
#if 0
	if(config.power_state==false)
	{
		time_t t=time(0);
		if(t-display_update_time>0)
		{
			display_update_time=t;
			tm timeSt;
			gmtime_r(&t, &timeSt);

			display->clearDisplay();
			
			char str[256];
			u8g2_for_adafruit_gfx.setFont(u8g2_font_inr16_mr);
			//u8g2_for_adafruit_gfx.setFont(u8g2_font_inr19_mr);
			u8g2_for_adafruit_gfx.setFontMode(1);                 // use u8g2 transparent mode (this is default)
			u8g2_for_adafruit_gfx.setFontDirection(0);            // left to right (this is default)
			u8g2_for_adafruit_gfx.setForegroundColor(WHITE);      // apply Adafruit GFX color

			sprintf(str, "%02d:%02d:%02d", timeSt.tm_hour, timeSt.tm_min, timeSt.tm_sec);
			int size=u8g2_for_adafruit_gfx.getUTF8Width(str);
			int posx=(display->width()-size)/2;
			u8g2_for_adafruit_gfx.drawUTF8(posx, 45, str);
			display->display();
		}
	}
	else
	{
		unsigned long t = millis();
		if(t - display_update_time < 200)
			return;
		display_update_time = t;
		display->clearDisplay();
		//display->setTextSize(3);
		display->setTextWrap(false);
		
		char str[256];
		//u8g2_for_adafruit_gfx.setFont(u8g2_font_cu12_t_cyrillic);  // select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall
		u8g2_for_adafruit_gfx.setFont(u8g2_font_inr24_t_cyrillic);  // select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall
		u8g2_for_adafruit_gfx.setFontMode(0);                 // use u8g2 transparent mode (this is default)
		u8g2_for_adafruit_gfx.setFontDirection(0);            // left to right (this is default)
		u8g2_for_adafruit_gfx.setForegroundColor(WHITE);      // apply Adafruit GFX color
		//u8g2_for_adafruit_gfx.setCursor(0,20);                // start writing at this position
		
		sprintf(str, "%d: %s ip:%s", config.currentEffect, config.language.GetEffect(config.currentEffect), WiFi.localIP().toString().c_str());
		//u8g2_for_adafruit_gfx.setCursor(pos_x,4);                // start writing at this position
		//u8g2_for_adafruit_gfx.print("Umlaut ÄÖÜ");            // UTF-8 string with german umlaut chars
		u8g2_for_adafruit_gfx.drawUTF8(pos_x, 43, str);
		display->display();

		pos_x=pos_x-4;
		if(pos_x < -1*u8g2_for_adafruit_gfx.getUTF8Width(str))
			pos_x=display->width();
	}
#endif
};
void FieryLedLamp::next_effect()
{
	unsigned short effect = config.currentEffect + 1;
	if (effect >= FieryLedLampEffectTypes::MaxEffect)
		effect = 0;
	change_effect(effect);
};
void FieryLedLamp::prev_effect()
{
	unsigned short effect = config.currentEffect;
	if (effect == 0)
		effect = FieryLedLampEffectTypes::MaxEffect;
	effect--;
	change_effect(effect);
};
void FieryLedLamp::set_speed(uint8_t speed)
{
	config.speed = speed;
	config.effect->set_speed(speed);
};
void FieryLedLamp::set_brightness(uint8_t bright)
{
	config.brightness = bright;
	config.effect->set_bright(bright);
};
void FieryLedLamp::update_effect_display()
{
	char str[256];
	sprintf(str, "%d: %s ip:%s", config.currentEffect, config.language.GetEffect(config.currentEffect), WiFi.localIP().toString().c_str());
	display.draw(str);
};
bool FieryLedLamp::change_effect(unsigned short index)
{
	DBG_PRINT("change_effect:%d\n", index);
	if (config.effect && config.currentEffect == index)
		return true;
	FieryLedLampEffect *current = config.effect;
	switch (index)
	{
	case FieryLedLampEffectTypes::WhiteColor:
		config.effect = new FieryLedLampEffectWhiteColorStripeRoutine();
		break;
	case FieryLedLampEffectTypes::Aurora:
		config.effect = new FieryLedLampEffectAurora();
		break;
	case FieryLedLampEffectTypes::WaterColor:
		config.effect = new FieryLedLampEffectWaterColor();
		break;
	case FieryLedLampEffectTypes::FlowerRuta:
		config.effect = new FieryLedLampEffectFlowerRuta();
		break;
	case FieryLedLampEffectTypes::Pool:
		config.effect = new FieryLedLampEffectPool();
		break;
	case FieryLedLampEffectTypes::Bamboo:
		config.effect = new FieryLedLampEffectBamboo();
		break;
	case FieryLedLampEffectTypes::Madness:
		config.effect = new FieryLedLampEffectMadnessNoise();
		break;
	case FieryLedLampEffectTypes::Ball:
		config.effect = new FieryLedLampEffectBall();
		break;
	case FieryLedLampEffectTypes::Waterfall:
		config.effect = new FieryLedLampEffectWaterfall();
		break;
	case FieryLedLampEffectTypes::Waterfall4_1:
	case FieryLedLampEffectTypes::Waves:
		config.effect = new FieryLedLampEffectWave();
		break;
	case FieryLedLampEffectTypes::MagicLantern:
		config.effect = new FieryLedLampEffectMagicLantern();
		break;
	case FieryLedLampEffectTypes::Wine:
		config.effect = new FieryLedLampEffectWine();
		break;
	case FieryLedLampEffectTypes::Whirl:
		config.effect = new FieryLedLampEffectWhirl(true);
		break;
	case FieryLedLampEffectTypes::WhirlMulti:
		config.effect = new FieryLedLampEffectWhirl(false);
		break;
	case FieryLedLampEffectTypes::StarFall:
		config.effect = new FieryLedLampEffectStarFall();
		break;
	case FieryLedLampEffectTypes::StormyRain:
		config.effect=new FieryLedLampEffectStormyRain();
		break;
	case FieryLedLampEffectTypes::DNA:
		config.effect = new FieryLedLampEffectDNA();
		break;
	case FieryLedLampEffectTypes::Smoke:
		config.effect = new FieryLedLampEffectSmoke(false);
		break;
	case FieryLedLampEffectTypes::SmokeColor:
		config.effect = new FieryLedLampEffectSmoke(true);
		break;
	case FieryLedLampEffectTypes::SmokeBalls:
		config.effect=new FieryLedLampEffectSmokeBalls();
		break;
	case FieryLedLampEffectTypes::LiqudLamp:
		config.effect = new FieryLedLampEffectLiquidLamp(true);
		break;
	case FieryLedLampEffectTypes::LiqudLampAuto:
		config.effect = new FieryLedLampEffectLiquidLamp(false);
		break;
	case FieryLedLampEffectTypes::Swirl:
		config.effect = new FieryLedLampEffectSwirl();
		break;
	case FieryLedLampEffectTypes::Stars:
		config.effect = new FieryLedLampEffectStars();
		break;
	case FieryLedLampEffectTypes::Zebra:
		config.effect = new FieryLedLampEffectZebra();
		break;
	case FieryLedLampEffectTypes::TixyLand:
		config.effect = new FieryLedLampEffectTixyLand();
		break;
	case FieryLedLampEffectTypes::Snakes:
		config.effect = new FieryLedLampEffectSnakes();
		break;
	case FieryLedLampEffectTypes::Fountain:
		config.effect = new FieryLedLampEffectFontain();
		break;
	case FieryLedLampEffectTypes::DropInWater:
		config.effect = new FieryLedLampEffectDropInWater();
		break;
	case FieryLedLampEffectTypes::Drops:
		config.effect = new FieryLedLampEffectDrops();
		break;
	case FieryLedLampEffectTypes::LLand:
		config.effect = new FieryLedLampEffectLLand();
		break;
	case FieryLedLampEffectTypes::Rings:
		config.effect = new FieryLedLampEffectRings();
		break;
	case FieryLedLampEffectTypes::Comet:
		config.effect=new FieryLedLampEffectComet();
		break;
	case FieryLedLampEffectTypes::CometColor:
		config.effect = new FieryLedLampEffectCometColor();
		break;
	case FieryLedLampEffectTypes::Comet2:
		config.effect = new FieryLedLampEffectCometCount(2);
		break;
	case FieryLedLampEffectTypes::Comet3:
		config.effect = new FieryLedLampEffectCometCount(3);
		break;
	case FieryLedLampEffectTypes::Contacts:
		config.effect = new FieryLedLampEffectContacts();
		break;
	case FieryLedLampEffectTypes::Sparkles:
		config.effect = new FieryLedLampEffectSparkles();
		break;
	case FieryLedLampEffectTypes::Cube2D:
		config.effect = new FieryLedLampEffectCube2D();
		break;
	case FieryLedLampEffectTypes::Lava:
		config.effect = new FieryLedLampEffectLava();
		break;
	case FieryLedLampEffectTypes::LavaLamp:
		config.effect = new FieryLedLampEffectLavaLamp();
		break;
	case FieryLedLampEffectTypes::ButterflyLamp:
		config.effect = new FieryLedLampEffectButterflyLamp(false);
		break;
	case FieryLedLampEffectTypes::Forest:
		config.effect = new FieryLedLampEffectForest();
		break;
	case FieryLedLampEffectTypes::Lumenjer:
		config.effect = new FieryLedLampEffectLumenjer();
		break;
	case FieryLedLampEffectTypes::Magma:
		config.effect = new FieryLedLampEffectMagma();
		break;
	case FieryLedLampEffectTypes::Paints:
		config.effect = new FieryLedLampEffectOilPaints();
		break;
	case FieryLedLampEffectTypes::Matrix:
		config.effect = new FieryLedLampEffectMatrix();
		break;
	case FieryLedLampEffectTypes::Twinkles:
		config.effect = new FieryLedLampEffectTwinkles();
		break;
	case FieryLedLampEffectTypes::Metaballs:
		config.effect = new FieryLedLampEffectMetaballs();
		break;
	case FieryLedLampEffectTypes::WebTools:
		config.effect = new FieryLedLampEffectWebTool();
		break;
	case FieryLedLampEffectTypes::Mosaic:
		config.effect = new FieryLedLampEffectMosaic();
		break;
	case FieryLedLampEffectTypes::Butterflys:
		config.effect = new FieryLedLampEffectButterflyLamp(true);
		break;
	case FieryLedLampEffectTypes::BBalls:
		config.effect = new FieryLedLampEffectBballs();
		break;
	case FieryLedLampEffectTypes::BallsBounce:
		config.effect = new FieryLedLampEffectBallsBounce();
		break;
	case FieryLedLampEffectTypes::ChristmasTree:
		config.effect = new FieryLedLampEffectChristmasTree();
		break;
	case FieryLedLampEffectTypes::NightCity:
		config.effect = new FieryLedLampEffectNightCity();
		break;
	case FieryLedLampEffectTypes::Fire:
		config.effect = new FieryLedLampEffectFire();
		break;
	case FieryLedLampEffectTypes::Fire2012:
		config.effect = new FieryLedLampEffectFire2012();
		break;
	case FieryLedLampEffectTypes::Fire2018:
		config.effect = new FieryLedLampEffectFire2018();
		break;
	case FieryLedLampEffectTypes::Fire2020:
		config.effect = new FieryLedLampEffectFire2020();
		break;
	case FieryLedLampEffectTypes::Fire2021:
		config.effect = new FieryLedLampEffectFire2021();
		break;
	case FieryLedLampEffectTypes::FireFlyTop:
		config.effect = new FieryLedLampEffectFireFlyTop();
		break;
	case FieryLedLampEffectTypes::FireFly:
		config.effect = new FieryLedLampEffectFireFly();
		break;
	case FieryLedLampEffectTypes::FireSparks:
		config.effect = new FieryLedLampEffectFireSparks();
		break;
	case FieryLedLampEffectTypes::ColorRain:
		config.effect = new FieryLedLampEffectColorRain();
		break;
	case FieryLedLampEffectTypes::Oscillating:
		config.effect = new FieryLedLampEffectOscillating();
		break;
	case FieryLedLampEffectTypes::Clouds:
		config.effect = new FieryLedLampEffectClouds();
		break;
	case FieryLedLampEffectTypes::Ocean:
		config.effect = new FieryLedLampEffectOcean();
		break;
	case FieryLedLampEffectTypes::Octopus:
		config.effect = new FieryLedLampEffectOctopus();
		break;
	case FieryLedLampEffectTypes::RainbowStripe:
		config.effect = new FieryLedLampEffectRainbowStripe();
		break;
	case FieryLedLampEffectTypes::HourGlass:
		config.effect = new FieryLedLampEffectHourglass();
		break;
	case FieryLedLampEffectTypes::Paintball:
		config.effect = new FieryLedLampEffectPaintball();
		break;
	case FieryLedLampEffectTypes::Picasso:
		// FIXME: доделать эффект, чтобы не вылетал за граници матрици. Разобраться с переменными
		config.effect = new FieryLedLampEffectPicasso();
		break;
	case FieryLedLampEffectTypes::Plasma:
		config.effect = new FieryLedLampEffectPlasma();
		break;
	case FieryLedLampEffectTypes::Spider:
		// FIXME: доделать эффект, чтобы не вылетал за граници матрици. Разобраться с переменными
		config.effect = new FieryLedLampEffectSpider();
		break;
	case FieryLedLampEffectTypes::PlasmaWaves:
		config.effect = new FieryLedLampEffectPlasmaWaves();
		break;
	case FieryLedLampEffectTypes::Flame:
		config.effect = new FieryLedLampEffectFlame();
		break;
	case FieryLedLampEffectTypes::PlanetEarth:
		config.effect = new FieryLedLampEffectPlanetEarth();
		break;
	case FieryLedLampEffectTypes::ByEffect:
		//config.effect = new FieryLedLampEffectByEffect();
		//break;
	case FieryLedLampEffectTypes::Popcorn:
		//config.effect = new FieryLedLampEffectPopcorn();
		//break;
	case FieryLedLampEffectTypes::Prismata:
	case FieryLedLampEffectTypes::Attract:
	case FieryLedLampEffectTypes::Leapers:
		config.effect = new FieryLedLampEffectLeapers();
		break;
	case FieryLedLampEffectTypes::Pulse:
	case FieryLedLampEffectTypes::PulseWhite:
	case FieryLedLampEffectTypes::PulseRainbow:
	case FieryLedLampEffectTypes::RadialWave:
		config.effect = new FieryLedLampEffectRadialWave();
		break;
	case FieryLedLampEffectTypes::Rainbow:
		config.effect = new FieryLedLampEffectRainbow();
		break;
	case FieryLedLampEffectTypes::Rainbow3D:
		config.effect = new FieryLedLampEffectRainbow3D();
		break;
	case FieryLedLampEffectTypes::RainbowSpot:
	case FieryLedLampEffectTypes::Snake:
	case FieryLedLampEffectTypes::Dandelions:
	case FieryLedLampEffectTypes::Rain:
		config.effect = new FieryLedLampEffectRain();
		break;
	case FieryLedLampEffectTypes::Rivers:
		config.effect = new FieryLedLampEffectBotswanaRivers();
		break;
	case FieryLedLampEffectTypes::Lighters:
		config.effect = new FieryLedLampEffectLighters();
		break;
	case FieryLedLampEffectTypes::LighterTraces:
		config.effect = new FieryLedLampEffectLighterTraces();
		break;
	case FieryLedLampEffectTypes::FeatherCandle:	// Свеча
		config.effect = new FieryLedLampEffectFeatherCandle();
		break;
	case FieryLedLampEffectTypes::NorthernLights: // Северное сияние
	case FieryLedLampEffectTypes::Serpentine:		// Серпантин
	case FieryLedLampEffectTypes::Scanner:		// Сканер
	case FieryLedLampEffectTypes::Sinusoid3:		// Синусоид
	case FieryLedLampEffectTypes::Colors:			// Смена цвета
		config.effect = new FieryLedLampEffectColors();
		break;
	case FieryLedLampEffectTypes::Snow:			// Снегопад
	case FieryLedLampEffectTypes::Specrum:		// Спектрум
	case FieryLedLampEffectTypes::Spiro:			// Спирали
	case FieryLedLampEffectTypes::Flock:			// Стая
	case FieryLedLampEffectTypes::FLOCK_N_PR:		// Стая и хищник
	case FieryLedLampEffectTypes::Arrows:			// Стрелки
	case FieryLedLampEffectTypes::Strobe:			// Строб.Хаос.Дифузия
	case FieryLedLampEffectTypes::Shadows:		// Тени
	case FieryLedLampEffectTypes::Pacific:		// Тихий океан
		config.effect = new FieryLedLampEffectPacific();
		break;
	case FieryLedLampEffectTypes::Tornado:		// Торнадо
		config.effect = new FieryLedLampEffectTornado();
		break;
	case FieryLedLampEffectTypes::SimpleRain:		// Tyчкa в банке
		config.effect = new FieryLedLampEffectSimpleRain();
		break;
	case FieryLedLampEffectTypes::Firework:		// Фейерверк
		config.effect = new FieryLedLampEffectFirework();
		break;
	case FieryLedLampEffectTypes::Firework2:		// Фейерверк 2
	case FieryLedLampEffectTypes::Fairy:			// Фея
	case FieryLedLampEffectTypes::Fontan:			// Фонтан
	case FieryLedLampEffectTypes::Color:			// Цвет
		config.effect = new FieryLedLampEffectColor();
		break;
	case FieryLedLampEffectTypes::ColoredPython:	// Цветной Питон
	case FieryLedLampEffectTypes::Sand:			// Цветные драже
		config.effect = new FieryLedLampEffectSand();
		break;
	case FieryLedLampEffectTypes::ColorFrizzles:	// Цветные кудри
	case FieryLedLampEffectTypes::Lotus:			// Цветок лотоса
		config.effect = new FieryLedLampEffectLotus();
		break;
	case FieryLedLampEffectTypes::Turbulence:		// Цифровая турбулентность
		config.effect = new FieryLedLampEffectTurbulence();
		break;
	case FieryLedLampEffectTypes::Spheres:		// Шapы
	case FieryLedLampEffectTypes::Nexus:
	case FieryLedLampEffectTypes::Clock:
		config.effect = new FieryLedLampEffectClock();
		break;
	default:
		DBG_PRINT("unknown effect:%d\n", index);
		return false;
	}
	config.currentEffect = index;

	config.effect->set_speed(config.speed);
	config.effect->set_scale(config.scale);
	config.effect->set_bright(config.brightness);

	config.effect->clear();
	config.effect->setup();
	if (current)
		delete current;

	update_effect_display();
	config.need_save = true;
	return true;
};