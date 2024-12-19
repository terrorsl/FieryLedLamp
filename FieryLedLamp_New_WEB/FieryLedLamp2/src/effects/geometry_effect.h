#ifndef GEOMETRY_EFFECT_FILE
#define GEOMETRY_EFFECT_FILE

class FieryLedLampEffectBall: public FieryLedLampEffect
{
public:
	FieryLedLampEffectBall():FieryLedLampEffect(DYNAMIC_DELAY){};
	void setup();
	void updateInner();
private:
	int16_t coordB[2];
	int8_t vectorB[2], deltaValue;
	CHSV _pulse_color;
	CRGB ballColor;
};
#endif