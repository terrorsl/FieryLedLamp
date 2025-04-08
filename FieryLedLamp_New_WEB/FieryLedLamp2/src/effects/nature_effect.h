#ifndef NATURE_EFFECT_FILE
#define NATURE_EFFECT_FILE

const FieryLedLampEffectTypes nature_effects[] = {FlowerRuta, Bamboo, Lava, Forest, Magma, ChristmasTree};

class FieryLedLampEffectPlanetEarth: public FieryLedLampEffect
{
public:
	FieryLedLampEffectPlanetEarth():FieryLedLampEffect(SOFT_DELAY){};
	void setup();
	void updateInner();
private:
};

class FieryLedLampEffectRainbow: public FieryLedLampEffect
{
public:
	FieryLedLampEffectRainbow():FieryLedLampEffect(DYNAMIC_DELAY){};
	void setup();
	void updateInner();
private:
    void rainbowHorVertRoutine(bool isVertical);
};

class FieryLedLampEffectRainbow3D: public FieryLedLampEffect
{
public:
	FieryLedLampEffectRainbow3D():FieryLedLampEffect(HIGH_DELAY){};
	void setup();
	void updateInner();
private:
};

class FieryLedLampEffectRadialWave: public FieryLedLampEffect
{
public:
	FieryLedLampEffectRadialWave():FieryLedLampEffect(DYNAMIC_DELAY){};
	void setup();
	void updateInner();
private:
	uint8_t noise3d[2][LED_WIDTH][LED_HEIGHT];
};

class FieryLedLampEffectBotswanaRivers: public FieryLedLampEffect
{
public:
	FieryLedLampEffectBotswanaRivers():FieryLedLampEffect(DYNAMIC_DELAY){};
	void setup();
	void updateInner();
private:
	void createScene(uint8_t idx);
	void animeBobbles();
	void flora();

	uint8_t step;
};

class FieryLedLampEffectRain: public FieryLedLampEffect
{
public:
	FieryLedLampEffectRain():FieryLedLampEffect(DYNAMIC_DELAY){};
	void setup();
	void updateInner();
private:
};

class FieryLedLampEffectPacific: public FieryLedLampEffect
{
public:
	FieryLedLampEffectPacific():FieryLedLampEffect(LOW_DELAY){};
	void setup();
	void updateInner();
private:
};

class FieryLedLampEffectTornado: public FieryLedLampEffect
{
public:
	FieryLedLampEffectTornado():FieryLedLampEffect(LOW_DELAY){};
	void setup();
	void updateInner();
private:
	uint8_t noise3d[2][LED_WIDTH][LED_HEIGHT];
};

class FieryLedLampEffectSand: public FieryLedLampEffect
{
public:
	FieryLedLampEffectSand():FieryLedLampEffect(DYNAMIC_DELAY){};
	void setup();
	void updateInner();
private:
	uint8_t pcnt;
};

class FieryLedLampEffectLotus: public FieryLedLampEffect
{
public:
	FieryLedLampEffectLotus():FieryLedLampEffect(DYNAMIC_DELAY){};
	void setup();
	void updateInner();
private:
	void drawLotusFlowerFragment(uint8_t posX, byte line);

	uint8_t step, deltaSpeed, deltaValue;
};

class FieryLedLampEffectTurbulence: public FieryLedLampEffect
{
public:
	FieryLedLampEffectTurbulence():FieryLedLampEffect(DYNAMIC_DELAY){};
	void setup();
	void updateInner();
private:
	void drawRandomCol(uint8_t x, uint8_t y, uint8_t offset, uint32_t count);
	
	uint8_t step, deltaValue;
	uint32_t count;
};

class FieryLedLampEffectLighters: public FieryLedLampEffect
{
public:
	FieryLedLampEffectLighters():FieryLedLampEffect(DYNAMIC_DELAY){};
	void setup();
	void updateInner();
private:
	uint8_t step;
};

#define BALLS_AMOUNT          (3U)                          // количество "шариков"
#define CLEAR_PATH            (1U)                          // очищать путь
#define BALL_TRACK            (1U)                          // (0 / 1) - вкл/выкл следы шариков
#define TRACK_STEP            (70U)                         // длина хвоста шарика (чем больше цифра, тем хвост короче)
class FieryLedLampEffectLighterTraces: public FieryLedLampEffect
{
public:
	FieryLedLampEffectLighterTraces():FieryLedLampEffect(DYNAMIC_DELAY){};
	void setup();
	void updateInner();
private:
	int16_t coord[BALLS_AMOUNT][2U];
	int8_t vector[BALLS_AMOUNT][2U];
	CRGB ballColors[BALLS_AMOUNT];
};

#endif