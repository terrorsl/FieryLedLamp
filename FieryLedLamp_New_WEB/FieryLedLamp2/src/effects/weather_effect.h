#ifndef WEATHER_EFFECT_FILE
#define WEATHER_EFFECT_FILE

const FieryLedLampEffectTypes weather_effects[] = {Whirl, StarFall, ColorRain};

class FieryLedLampEffectWhirl: public FieryLedLampEffect
{
public:
	FieryLedLampEffectWhirl(bool _oneColor):FieryLedLampEffect(HIGH_DELAY),oneColor(_oneColor){};
	void setup();
	void updateInner();
private:
	bool oneColor;
	uint16_t ff_x;
	uint16_t ff_y;
	uint16_t ff_z;

	static const uint8_t AVAILABLE_BOID_COUNT = 20U;
	Boid boids[AVAILABLE_BOID_COUNT];
};

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

class FieryLedLampEffectClouds: public FieryLedLampEffect
{
public:
	FieryLedLampEffectClouds():FieryLedLampEffect(HIGH_DELAY){};
    void setup();
	void updateInner();
private:
};
class FieryLedLampEffectOcean: public FieryLedLampEffect
{
public:
	FieryLedLampEffectOcean():FieryLedLampEffect(HIGH_DELAY){};
    void setup();
	void updateInner();
private:
};
class FieryLedLampEffectRainbowStripe: public FieryLedLampEffect
{
public:
	FieryLedLampEffectRainbowStripe():FieryLedLampEffect(HIGH_DELAY){};
    void setup();
	void updateInner();
private:
};
class FieryLedLampEffectStormyRain: public FieryLedLampEffect
{
public:
	FieryLedLampEffectStormyRain():FieryLedLampEffect(DYNAMIC_DELAY){
		dots = new uint32_t*[LED_WIDTH];
		for(int i=0;i<LED_HEIGHT;i++)
			dots[i]=new uint32_t[LED_HEIGHT];
	};
	~FieryLedLampEffectStormyRain()
	{
		for(int i=0;i<LED_HEIGHT;i++)
			delete[] dots[i];
		delete[] dots;
	}
    void setup();
	void updateInner();
private:
	uint32_t **dots;
	uint8_t line[LED_WIDTH];
};
class FieryLedLampEffectSimpleRain: public FieryLedLampEffect
{
public:
	FieryLedLampEffectSimpleRain():FieryLedLampEffect(DYNAMIC_DELAY){
		dots = new uint32_t*[LED_WIDTH];
		for(int i=0;i<LED_HEIGHT;i++)
			dots[i]=new uint32_t[LED_HEIGHT];
	};
	~FieryLedLampEffectSimpleRain()
	{
		for(int i=0;i<LED_HEIGHT;i++)
			delete[] dots[i];
		delete[] dots;
	}
    void setup();
	void updateInner();
private:
	uint32_t **dots;
	uint8_t line[LED_WIDTH];
};
class FieryLedLampEffectNorthernLights: public FieryLedLampEffect
{
public:
	FieryLedLampEffectNorthernLights():FieryLedLampEffect(DYNAMIC_DELAY){
	};
	~FieryLedLampEffectNorthernLights()
	{
	}
    void setup();
	void updateInner();
private:
	unsigned long polarTimer;
};
#endif