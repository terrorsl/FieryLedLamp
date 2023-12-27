#ifndef FIERY_LED_LAMP_FILE
#define FIERY_LED_LAMP_FILE

#include"platform.h"
#include"Constants.h"

#ifdef USE_NTP
#include <NTPClient.h>
#endif

class FieryLedLamp
{
public:
	bool setup_config();
	void setup_time();
	void setup_pin();
	void setup_wifi();
	bool setup_led();
	bool setup_web_server();

	void update_time();
private:
#ifdef USE_NTP
	NTPClient *ntpClient;
#endif
};
#endif