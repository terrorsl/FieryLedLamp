#include"FieryLedLamp.h"
#include"Constants.h"

bool FieryLedLamp::setup_config()
{
	"config.json";
	return true;
};
void FieryLedLamp::setup_pin()
{
#if defined(ESP_USE_BUTTON) && defined(BUTTON_LOCK_ON_START)
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
#endif
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
	ntpClient = new NTPClient();
#endif
};

void FieryLedLamp::update_time()
{
#ifdef USE_NTP
	ntpClient->update();
#endif
};