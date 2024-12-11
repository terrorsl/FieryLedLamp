#ifndef LIQUID_EFFECT_FILE
#define LIQUID_EFFECT_FILE

const FieryLedLampEffectTypes liquid_effects[] = {FieryLedLampEffectTypes::Pool, Waterfall, Waterfall4_1, Waves, StormyRain, Fountain, DropInWater, Drops, LLand};

class FieryLedLampEffectPool: public FieryLedLampEffect
{
public:
	FieryLedLampEffectPool():FieryLedLampEffect(DYNAMIC_DELAY){};
	void setup();
	void updateInner();
private:
	uint8_t deltaHue,deltaHue2,step;
};

class FieryLedLampEffectWaterfall: public FieryLedLampEffect
{
public:
	FieryLedLampEffectWaterfall():FieryLedLampEffect(DYNAMIC_DELAY){};
	void setup();
	void updateInner();
private:
	uint8_t noise3d[LED_WIDTH][LED_HEIGHT];     // двухслойная маска или хранилище свойств в размер всей матрицы
};

class FieryLedLampEffectDropInWater: public FieryLedLampEffect
{
public:
	FieryLedLampEffectDropInWater():FieryLedLampEffect(DYNAMIC_DELAY){};
	void setup();
	void updateInner();
private:
	int rad[(LED_HEIGHT + LED_WIDTH) / 8];
	byte posx[(LED_HEIGHT + LED_WIDTH) / 8], posy[(LED_HEIGHT + LED_WIDTH) / 8];
};

class FieryLedLampEffectWave: public FieryLedLampEffect
{
public:
	FieryLedLampEffectWave():FieryLedLampEffect(DYNAMIC_DELAY){};
	void setup();
	void updateInner();
private:
	uint8_t waveCount;
	uint8_t waveRotation;
	uint8_t waveThetaUpdate;
	uint8_t waveThetaUpdateFrequency;
	uint8_t waveTheta;
	uint8_t hueUpdate;
	uint8_t hueUpdateFrequency;
};

class FieryLedLampEffectLLand: public FieryLedLampEffect
{
public:
	FieryLedLampEffectLLand():FieryLedLampEffect(DYNAMIC_DELAY){};
	void setup();
	void updateInner();
private:
    uint8_t deltaValue;
    uint16_t ff_y, ff_z;
};
#endif