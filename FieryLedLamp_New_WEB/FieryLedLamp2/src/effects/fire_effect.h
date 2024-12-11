#ifndef FIRE_EFFECT_FILE
#define FIRE_EFFECT_FILE

const FieryLedLampEffectTypes fire_effects[]={ Fire, Fire2012, Fire2018, Fire2020, Fire2021, FireFlyTop, FireFly, FireSparks };

class FieryLedLampEffectFire: public FieryLedLampEffect
{
public:
	FieryLedLampEffectFire():FieryLedLampEffect(DYNAMIC_DELAY){};
	void setup();
	void updateInner();
private:
	void generateLine();
	void shiftUp();
	void drawFrame(uint8_t pcnt, bool isColored);

	uint8_t pcnt, matrixValue[LED_WIDTH][LED_HEIGHT], line[LED_WIDTH], shiftHue[LED_HEIGHT], shiftValue[LED_HEIGHT];
};

class FieryLedLampEffectFire2012: public FieryLedLampEffect
{
public:
	FieryLedLampEffectFire2012():FieryLedLampEffect(DYNAMIC_DELAY){};
	void setup();
	void updateInner();
private:
	uint8_t noise3d[LED_WIDTH][LED_HEIGHT];
};

class FieryLedLampEffectFire2018: public FieryLedLampEffect
{
public:
	FieryLedLampEffectFire2018():FieryLedLampEffect(DYNAMIC_DELAY){};
	void setup();
	void updateInner();
private:
	uint8_t noise3d[2][LED_WIDTH][LED_HEIGHT];
	uint32_t noise32_x[2], noise32_y[2], noise32_z[2], scale32_x[2], scale32_y[2];
};

class FieryLedLampEffectFire2020: public FieryLedLampEffect
{
public:
	FieryLedLampEffectFire2020():FieryLedLampEffect(DYNAMIC_DELAY){};
	void setup();
	void updateInner();
private:
	uint8_t shiftHue[LED_HEIGHT], deltaValue, deltaHue, step;
	uint16_t ff_y, ff_z;
};

class FieryLedLampEffectFire2021: public FieryLedLampEffect
{
public:
	FieryLedLampEffectFire2021():FieryLedLampEffect(LOW_DELAY){};
	void setup();
	void updateInner();
private:
	uint8_t shiftHue[LED_HEIGHT], deltaValue, deltaHue, step, deltaHue2, pcnt;
	float speedfactor;
	uint16_t ff_x, ff_y, ff_z;
};

class FieryLedLampEffectFireFly: public FieryLedLampEffect
{
public:
	FieryLedLampEffectFireFly():FieryLedLampEffect(DYNAMIC_DELAY){};
    void setup();
	void updateInner();
};

class Spark
{
private:
    CRGB color;
    uint8_t Bri;
    uint8_t Hue;
    float x, y, speedy;
public:
    void addXY(float nx, float ny) {
      x += nx;
      y += ny * speedy;
    }

    float getY() {
      return y;
    }

    void reset();

    void draw(FieryLedLampEffect *effect) {
      color.fadeLightBy(256 / (LED_HEIGHT * 0.75));
      effect->drawPixelXYF(x, y, color);
    }
};

class FieryLedLampEffectFireSparks: public FieryLedLampEffect
{
public:
	FieryLedLampEffectFireSparks():FieryLedLampEffect(HIGH_DELAY){};
	void setup();
	void updateInner();
private:
    uint32_t t;
    Spark sparks[LED_WIDTH/4];
};
#endif