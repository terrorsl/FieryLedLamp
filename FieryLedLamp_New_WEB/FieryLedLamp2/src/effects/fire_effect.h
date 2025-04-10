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

class FieryLedLampEffectPlasma: public FieryLedLampEffect
{
public:
	FieryLedLampEffectPlasma():FieryLedLampEffect(HIGH_DELAY){};
	void setup();
	void updateInner();
private:
};

class FieryLedLampEffectSpider: public FieryLedLampEffect
{
public:
	FieryLedLampEffectSpider():FieryLedLampEffect(LOW_DELAY){};
	void setup();
	void updateInner();
private:
	float speedfactor;
	uint8_t pcnt;
};

class FieryLedLampEffectPlasmaWaves: public FieryLedLampEffect
{
public:
	FieryLedLampEffectPlasmaWaves():FieryLedLampEffect(SOFT_DELAY){};
	void setup();
	void updateInner();
private:
	uint64_t frameCount;
};

class FieryLedLampEffectFlame: public FieryLedLampEffect
{
public:
	FieryLedLampEffectFlame():FieryLedLampEffect(LOW_DELAY){};
	void setup();
	void updateInner();
private:
	void wu_pixel_maxV(uint8_t item);
	
	uint64_t frameCount;
	uint8_t enlargedObjectNUM;
	uint8_t noise3d[2][LED_WIDTH][LED_HEIGHT], shiftValue[LED_HEIGHT];
};

class FieryLedLampEffectFeatherCandle: public FieryLedLampEffect
{
public:
	FieryLedLampEffectFeatherCandle():FieryLedLampEffect(LOW_DELAY){};
	void setup();
	void updateInner();
private:
};

class FieryLedLampEffectFirework: public FieryLedLampEffect
{
public:
	FieryLedLampEffectFirework():FieryLedLampEffect(SOFT_DELAY){};
	void setup();
	void updateInner();
private:
	void VirtualExplosion(uint8_t f_type, int8_t timeline);

	uint8_t MAX_BRIGHTNESS;
	uint8_t DOT_EXPLOSION;
	uint8_t HORIZONT;
	uint8_t DELTA;
	float stepH;
	uint8_t FPS_DELAY;
	uint8_t STEP;
	uint8_t skyColor;
	uint8_t sizeH;
	uint8_t step, deltaHue2;
};
#endif