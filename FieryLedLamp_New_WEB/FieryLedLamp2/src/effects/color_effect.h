#ifndef COLOR_EFFECT_FILE
#define COLOR_EFFECT_FILE

const FieryLedLampEffectTypes color_effects[] = {WhiteColor, WaterColor, Lumenjer, Paints};

class FieryLedLampEffectWhiteColorStripeRoutine: public FieryLedLampEffect
{
public:
	FieryLedLampEffectWhiteColorStripeRoutine():FieryLedLampEffect(HIGH_DELAY){};
	void setup();
	void updateInner();
};

class FieryLedLampEffectWaterColor: public FieryLedLampEffect
{
public:
	FieryLedLampEffectWaterColor():FieryLedLampEffect(DYNAMIC_DELAY){};
	void setup();
	void updateInner();
private:
	void SmearPaint(uint8_t *obj);

	bool lastColor;
	uint8_t x,end_x,y,end_y,direction, width;

	uint8_t step, deltaValue, deltaHue;
  	uint8_t trackingObject[6];
};
#endif