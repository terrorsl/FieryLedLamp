#ifndef WEATHER_EFFECT_FILE
#define WEATHER_EFFECT_FILE

const FieryLedLampEffectTypes weather_effects[] = {Whirl, StarFall, ColorRain};

class FieryLedLampEffectColorRain: public FieryLedLampEffect
{
public:
	FieryLedLampEffectColorRain():FieryLedLampEffect(DYNAMIC_DELAY){};
    void setup();
	void updateInner();
private:
    uint32_t cell[LED_WIDTH][LED_HEIGHT];
    uint8_t line[LED_WIDTH];
};
#endif