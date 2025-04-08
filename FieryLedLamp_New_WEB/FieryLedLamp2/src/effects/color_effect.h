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
	void SmearPaint();

	bool lastColor;
	uint8_t x,end_x,y,end_y,direction, width;

	uint8_t step, deltaValue, deltaHue;
};

class FieryLedLampEffectPaintball: public FieryLedLampEffect
{
public:
	FieryLedLampEffectPaintball():FieryLedLampEffect(DYNAMIC_DELAY){};
	void setup();
	void updateInner();
private:
};

class FieryLedLampEffectNightCity: public FieryLedLampEffect
{
public:
	FieryLedLampEffectNightCity():FieryLedLampEffect(HIGH_DELAY){};
	void setup();
	void updateInner();
private:
	uint8_t step;
};

class FieryLedLampEffectPicasso: public FieryLedLampEffect
{
public:
	FieryLedLampEffectPicasso():FieryLedLampEffect(DYNAMIC_DELAY){};
	void setup();
	void updateInner();
private:
	void PicassoPosition();
	void PicassoGenerate(bool reset);
	void PicassoRoutine();
	void PicassoRoutine2();
	void PicassoRoutine3();

	uint8_t enlargedObjectNUM;
};

class FieryLedLampEffectColor: public FieryLedLampEffect
{
public:
	FieryLedLampEffectColor():FieryLedLampEffect(LOW_DELAY){};
	void setup();
	void updateInner();
private:
};

class FieryLedLampEffectColors: public FieryLedLampEffect
{
public:
	FieryLedLampEffectColors():FieryLedLampEffect(HIGH_DELAY){};
	void setup();
	void updateInner();
private:
	uint8_t step;
	uint8_t change;
};
#endif