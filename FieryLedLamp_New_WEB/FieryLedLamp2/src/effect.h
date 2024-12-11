#ifndef EFFECT_FILE
#define EFFECT_FILE

#include"platform.h"
#include"Constants.h"
#include<FastLED.h>

//константы размера матрицы вычисляется только здесь и не меняется в эффектах
#define CENTER_X_MINOR ((LED_WIDTH / 2) -  ((LED_WIDTH - 1) & 0x01)) // центр матрицы по ИКСУ, сдвинутый в меньшую сторону, если ширина чётная
#define CENTER_Y_MINOR ((LED_HEIGHT / 2) - ((LED_HEIGHT - 1) & 0x01)) // центр матрицы по ИГРЕКУ, сдвинутый в меньшую сторону, если высота чётная
const uint8_t CENTER_X_MAJOR = LED_WIDTH / 2  + (LED_WIDTH % 2); // центр матрицы по ИКСУ, сдвинутый в большую сторону, если ширина чётная
const uint8_t CENTER_Y_MAJOR = LED_HEIGHT / 2  + (LED_HEIGHT % 2); // центр матрицы по ИГРЕКУ, сдвинутый в большую сторону, если высота чётная

typedef enum
{
	WhiteColor, // Бeлый cвeт
	Aurora, // Аврора
	WaterColor, // Акварель
	FlowerRuta, // Аленький цветочек
	Pool, // Бассейн
	Bamboo, // Бамбук
	Madness, // Безумие
	Ball, // Блуждающий кубик
	Waterfall, // Водопад
	Waterfall4_1, // Водопад 4в1
	Waves, // Волны
	MagicLantern, // Волшебный Фонарик
	Wine, // Вино
	Whirl, // Вихри пламени
	WhirlMulti, // Вихри разноцветные
	StarFall, // Вьюга
	StormyRain, // Гроза в банке
	DNA, // ДНК
	Smoke, // Дым
	SmokeColor, // Дым разноцветный
	SmokeBalls, // Дымовые шашки
	LiqudLamp, // Жидкая лампа
	LiqudLampAuto, // Жидкая лампа авто
	Swirl, // Завиток
	Stars, // Звезды
	Zebra, // Зебра
	TixyLand, // Земля Тикси
	Snakes, // Змейки
	Fountain, // Источник
	DropInWater, // Капли на воде
	Drops, // Капли на стекле
	LLand, // Кипение
	Rings, // Кодовый замок
	Comet, // Комета
	CometColor, // Комета одноцветная
	Comet2, // Комета двойная
	Comet3, // Комета тройная
	Contacts, // Контакты
	Sparkles, // Конфетти
	Cube2D, // Кубик Рубика
	Lava, // Лава
	LavaLamp, // Лавовая лампа
	ButterflyLamp, // Лампа с мотыльками
	Forest,  // Лес
	Lumenjer, // Люмeньep
	Magma, // Магма
	Paints, // Масляные краски
	Matrix, // Матрица
	Twinkles, // Мерцание
  	Metaballs, // Метоболз
	WebTools, // Мечта дизайнера
	Mosaic, // Мозайка
	Butterflys, // Moтыльки
	BBalls, // Мячики
	BallsBounce, // Мячики без границ
	ChristmasTree, // Новогодняя Елка
	NightCity, // Ночной Город
	Fire, // Огонь
	Fire2012, // Огонь 2012
	Fire2018, // Огонь 2018
	Fire2020, // Огонь 2020
	Fire2021, // Огонь 2021
	FireFlyTop, // Огoнь верховой
	FireFly, // Огoнь парящий
	FireSparks, // Огонь с искрами
	ColorRain, // Осадки
/*	Oscillating, // Осциллятор
	Clouds, // Облака
	Ocean, // Океан
	Octopus, // Осьминог
#define EFF_RAINBOW_STRIPE      ( 70U)    // Павлин
#define EFF_HOURGLASS           ( 71U)    // Песочные часы
#define EFF_PAINTBALL           ( 72U)    // Пейнтбол
#define EFF_PICASSO             ( 73U)    // Пикассо
#define EFF_PLASMA              ( 74U)    // Плазма
#define EFF_SPIDER              ( 75U)    // Плазменная лампа
#define EFF_PLASMA_WAVES        ( 76U)    // Плазменные волны
#define EFF_FLAME               ( 77U)    // Пламя
#define EFF_PLANETEARTH         ( 78U)    // Планета Земля
#define EFF_BY_EFFECT           ( 79U)    // Побочный эффект
#define EFF_POPCORN             ( 80U)    // Попкорн
#define EFF_PRISMATA            ( 81U)    // Призмата
#define EFF_ATTRACT             ( 82U)    // Притяжение
#define EFF_LEAPERS             ( 83U)    // Пpыгyны
#define EFF_PULSE               ( 84U)    // Пульс
#define EFF_PULSE_WHITE         ( 85U)    // Пульс белый
#define EFF_PULSE_RAINBOW       ( 86U)    // Пульс радужный
#define EFF_RADIAL_WAWE         ( 87U)    // Радиальная волна
#define EFF_RAINBOW_VER         ( 88U)    // Радуга
#define EFF_RAINBOW             ( 89U)    // Радуга 3D
#define EFF_RAINBOW_SPOT        ( 90U)    // Радужное Пятно
#define EFF_SNAKE               ( 91U)    // Радужный змей
#define EFF_DANDELIONS          ( 92U)    // Разноцветные одуванчики
#define EFF_RAIN                ( 93U)    // Разноцветный дождь
#define EFF_RIVERS              ( 94U)    // Реки Ботсваны
#define EFF_LIGHTERS            ( 95U)    // Светлячки
#define EFF_LIGHTER_TRACES      ( 96U)    // Светлячки со шлейфом
#define EFF_FEATHER_CANDLE      ( 97U)    // Свеча
#define EFF_AURORA              ( 98U)    // Северное сияние
#define EFF_SERPENTINE          ( 99U)    // Серпантин
#define EFF_SCANNER             (100U)    // Сканер
#define EFF_SINUSOID3           (101U)    // Синусоид
#define EFF_COLORS              (102U)    // Смена цвета
#define EFF_SNOW                (103U)    // Снегопад
#define EFF_SPECTRUM            (104U)    // Спектрум
#define EFF_SPIRO               (105U)    // Спирали
#define EFF_FLOCK               (106U)    // Стая
#define EFF_FLOCK_N_PR          (107U)    // Стая и хищник
#define EFF_ARROWS              (108U)    // Стрелки
#define EFF_STROBE              (109U)    // Строб.Хаос.Дифузия
#define EFF_SHADOWS             (110U)    // Тени
#define EFF_PACIFIC             (111U)    // Тихий океан
#define EFF_TORNADO             (112U)    // Торнадо
#define EFF_SIMPLE_RAIN         (113U)    // Tyчкa в банке
#define EFF_FIREWORK            (114U)    // Фейерверк
#define EFF_FIREWORK_2          (115U)    // Фейерверк 2
#define EFF_FAIRY               (116U)    // Фея
#define EFF_FONTAN              (117U)    // Фонтан
#define EFF_COLOR               (118U)    // Цвет
#define EFF_EFF_COLORED_PYTHON  (119U)    // Цветной Питон
#define EFF_EFF_SAND            (120U)    // Цветные драже
#define EFF_COLOR_FRIZZLES      (121U)    // Цветные кудри
#define EFF_EFF_LOTUS           (122U)    // Цветок лотоса
#define EFF_TURBULENCE          (123U)    // Цифровая турбулентность
#define EFF_SPHERES             (124U)    // Шapы
#define EFF_NEXUS               (125U)    // Nexus
#define EFF_CLOCK               (126U)    // Часы*/
	MaxEffect
}FieryLedLampEffectTypes;

typedef enum
{
	FieryLedLampEffectCategoryType_Fire,
	FieryLedLampEffectCategoryType_MaxSize
}FieryLedLampEffectCategoryType;

struct FieryLedLampEffectCategory
{
	const FieryLedLampEffectTypes *effects;
};

#define HIGH_DELAY 0
#define LOW_DELAY 1
#define DYNAMIC_DELAY 2
#define SOFT_DELAY 3

// палитра для типа реалистичного водопада (если ползунок Масштаб выставить на 100)
const TProgmemRGBPalette16 WaterfallColors_p FL_PROGMEM = {0x000000, 0x060707, 0x101110, 0x151717, 0x1C1D22, 0x242A28, 0x363B3A, 0x313634, 0x505552, 0x6B6C70, 0x98A4A1, 0xC1C2C1, 0xCACECF, 0xCDDEDD, 0xDEDFE0, 0xB2BAB9};

// добавлено изменение текущей палитры (используется во многих эффектах ниже для бегунка Масштаб)
static const TProgmemRGBPalette16 *palette_arr[] = {
  &PartyColors_p,
  &OceanColors_p,
  &LavaColors_p,
  &HeatColors_p,
  &WaterfallColors_p,
  &CloudColors_p,
  &ForestColors_p,
  &RainbowColors_p,
  &RainbowStripeColors_p
};

// дополнительные палитры для пламени
// для записи в PROGMEM преобразовывал из 4 цветов в 16 на сайте https://colordesigner.io/gradient-generator, но не уверен, что это эквивалент CRGBPalette16()
// значения цветовых констант тут: https://github.com/FastLED/FastLED/wiki/Pixel-reference
const TProgmemRGBPalette16 WoodFireColors_p FL_PROGMEM = {CRGB::Black, 0x330e00, 0x661c00, 0x992900, 0xcc3700, CRGB::OrangeRed, 0xff5800, 0xff6b00, 0xff7f00, 0xff9200, CRGB::Orange, 0xffaf00, 0xffb900, 0xffc300, 0xffcd00, CRGB::Gold};             //* Orange
const TProgmemRGBPalette16 NormalFire_p FL_PROGMEM = {CRGB::Black, 0x330000, 0x660000, 0x990000, 0xcc0000, CRGB::Red, 0xff0c00, 0xff1800, 0xff2400, 0xff3000, 0xff3c00, 0xff4800, 0xff5400, 0xff6000, 0xff6c00, 0xff7800};                             // пытаюсь сделать что-то более приличное
const TProgmemRGBPalette16 NormalFire2_p FL_PROGMEM = {CRGB::Black, 0x560000, 0x6b0000, 0x820000, 0x9a0011, CRGB::FireBrick, 0xc22520, 0xd12a1c, 0xe12f17, 0xf0350f, 0xff3c00, 0xff6400, 0xff8300, 0xffa000, 0xffba00, 0xffd400};                      // пытаюсь сделать что-то более приличное
const TProgmemRGBPalette16 LithiumFireColors_p FL_PROGMEM = {CRGB::Black, 0x240707, 0x470e0e, 0x6b1414, 0x8e1b1b, CRGB::FireBrick, 0xc14244, 0xd16166, 0xe08187, 0xf0a0a9, CRGB::Pink, 0xff9ec0, 0xff7bb5, 0xff59a9, 0xff369e, CRGB::DeepPink};        //* Red
const TProgmemRGBPalette16 SodiumFireColors_p FL_PROGMEM = {CRGB::Black, 0x332100, 0x664200, 0x996300, 0xcc8400, CRGB::Orange, 0xffaf00, 0xffb900, 0xffc300, 0xffcd00, CRGB::Gold, 0xf8cd06, 0xf0c30d, 0xe9b913, 0xe1af1a, CRGB::Goldenrod};           //* Yellow
const TProgmemRGBPalette16 CopperFireColors_p FL_PROGMEM = {CRGB::Black, 0x001a00, 0x003300, 0x004d00, 0x006600, CRGB::Green, 0x239909, 0x45b313, 0x68cc1c, 0x8ae626, CRGB::GreenYellow, 0x94f530, 0x7ceb30, 0x63e131, 0x4bd731, CRGB::LimeGreen};     //* Green
const TProgmemRGBPalette16 AlcoholFireColors_p FL_PROGMEM = {CRGB::Black, 0x000033, 0x000066, 0x000099, 0x0000cc, CRGB::Blue, 0x0026ff, 0x004cff, 0x0073ff, 0x0099ff, CRGB::DeepSkyBlue, 0x1bc2fe, 0x36c5fd, 0x51c8fc, 0x6ccbfb, CRGB::LightSkyBlue};  //* Blue
const TProgmemRGBPalette16 RubidiumFireColors_p FL_PROGMEM = {CRGB::Black, 0x0f001a, 0x1e0034, 0x2d004e, 0x3c0068, CRGB::Indigo, CRGB::Indigo, CRGB::Indigo, CRGB::Indigo, CRGB::Indigo, CRGB::Indigo, 0x3c0084, 0x2d0086, 0x1e0087, 0x0f0089, CRGB::DarkBlue};        //* Indigo
const TProgmemRGBPalette16 PotassiumFireColors_p FL_PROGMEM = {CRGB::Black, 0x0f001a, 0x1e0034, 0x2d004e, 0x3c0068, CRGB::Indigo, 0x591694, 0x682da6, 0x7643b7, 0x855ac9, CRGB::MediumPurple, 0xa95ecd, 0xbe4bbe, 0xd439b0, 0xe926a1, CRGB::DeepPink}; //* Violet
static const TProgmemRGBPalette16 *firePalettes[] = {
  //    &HeatColors_p, // эта палитра уже есть в основном наборе. если в эффекте подключены оба набора палитр, тогда копия не нужна
  &WoodFireColors_p,
  &NormalFire_p,
  &NormalFire2_p,
  &LithiumFireColors_p,
  &SodiumFireColors_p,
  &CopperFireColors_p,
  &AlcoholFireColors_p,
  &RubidiumFireColors_p,
  &PotassiumFireColors_p
};

extern CRGB leds[NUM_LEDS];
void blurScreen(fract8 blur_amount, CRGB *LEDarray = leds);
void dimAll(uint8_t value, CRGB *LEDarray = leds);
uint16_t XY(uint8_t x, uint8_t y);

class FieryLedLampEffect
{
public:
	FieryLedLampEffect(uint8_t delayType):delat_type(delayType){};
	virtual ~FieryLedLampEffect(){}

	void set_bright(unsigned char val){brightness=val;}
	void set_speed(unsigned char val){speed=val;}
	void set_scale(unsigned char val){scale=val;}

	virtual void setup() = 0;
	virtual void updateInner() = 0;

	void update();

	// функция отрисовки точки по координатам X Y
	void drawPixelXY(int8_t x, int8_t y, CRGB color);
	//по мотивам
	//https://gist.github.com/sutaburosu/32a203c2efa2bb584f4b846a91066583
	void drawPixelXYF(float x, float y, CRGB color);
protected:
	void setCurrentPalette() {
		if (scale > 100U)
			scale = 100U; // чтобы не было проблем при прошивке без очистки памяти
  		//curPalette = palette_arr[(uint8_t)(scale / 100.0F * ((sizeof(palette_arr) / sizeof(TProgmemRGBPalette16 *)) - 0.01F))];
		unsigned char index = map(scale, 0, 100, 0, (sizeof(palette_arr) / sizeof(TProgmemRGBPalette16 *))-1);
		curPalette = palette_arr[index];
	}
	// функция получения цвета пикселя по его номеру
	CRGB getPixelColor(uint32_t thisSegm);
	// функция получения цвета пикселя в матрице по его координатам
	CRGB getPixelColorXY(uint8_t x, uint8_t y);
	// залить все
	void fillAll(CRGB color);
	void fadePixel(uint8_t x, uint8_t y, uint8_t step);
	void drawStar(float xlocl, float ylocl, float biggy, float little, int16_t points, float dangle, uint8_t koler);
	void DrawLine(int x1, int y1, int x2, int y2, CRGB color);
	void drawCircleF(float x0, float y0, float radius, CRGB color);
	void drawCircle(int x0, int y0, int radius, const CRGB &color);
	void fillNoiseLED(const TProgmemRGBPalette16 *currentPalette);
	void FillNoise(uint8_t **noise3d, uint32_t noise32_x, uint32_t noise32_y, uint32_t noise32_z, uint32_t scale32_x, uint32_t scale32_y, uint8_t noisesmooth);
	void drawRecCHSV(uint8_t startX, uint8_t startY, uint8_t endX, uint8_t endY, CHSV color);

	uint8_t speed, scale, brightness;

	//массивы состояния объектов, которые могут использоваться в любом эффекте
	#define trackingOBJECT_MAX_COUNT                         (100U)  // максимальное количество отслеживаемых объектов (очень влияет на расход памяти)
	float   trackingObjectPosX[trackingOBJECT_MAX_COUNT];
	float   trackingObjectPosY[trackingOBJECT_MAX_COUNT];
	float   trackingObjectSpeedX[trackingOBJECT_MAX_COUNT];
	float   trackingObjectSpeedY[trackingOBJECT_MAX_COUNT];
	float   trackingObjectShift[trackingOBJECT_MAX_COUNT];
	uint8_t trackingObjectHue[trackingOBJECT_MAX_COUNT];
	uint8_t trackingObjectState[trackingOBJECT_MAX_COUNT];
	bool    trackingObjectIsShift[trackingOBJECT_MAX_COUNT];

	#if (LED_WIDTH > LED_HEIGHT)
	uint8_t noise[WIDTH][WIDTH];
	#else
	uint8_t noise[LED_HEIGHT][LED_HEIGHT];
	#endif
	uint8_t colorLoop;
	uint8_t ihue = 0;

	uint8_t delat_type;
	uint8_t hue,hue2;

	uint16_t x,y,z;

	const TProgmemRGBPalette16 *curPalette;

	unsigned long effectTimer, FPSdelay;
};

class FieryLedLampEffectFlowerRuta:public FieryLedLampEffect
{
public:
	FieryLedLampEffectFlowerRuta():FieryLedLampEffect(DYNAMIC_DELAY){};
	void setup();
	void updateInner();
private:
	uint8_t PETALS;
	uint32_t t;

	#define NUM_LAYERSMAX 2
	uint8_t noise3d[NUM_LAYERSMAX][LED_WIDTH][LED_HEIGHT];     // двухслойная маска или хранилище свойств в размер всей матрицы
};

class FieryLedLampEffectBamboo: public FieryLedLampEffect
{
public:
	FieryLedLampEffectBamboo():FieryLedLampEffect(DYNAMIC_DELAY){};
	void setup();
	void updateInner();
private:
	uint8_t step;
	float index;
	uint8_t posY;
	float deltaX;
	bool direct;
	uint8_t colLine;
};

class FieryLedLampEffectMagicLantern: public FieryLedLampEffect
{
public:
	FieryLedLampEffectMagicLantern():FieryLedLampEffect(DYNAMIC_DELAY){};
	void setup();
	void updateInner();
private:
	void gradientVertical(uint8_t startX, uint8_t startY, uint8_t endX, uint8_t endY, uint8_t start_color, uint8_t end_color, uint8_t start_br, uint8_t end_br, uint8_t saturate);
	uint8_t saturation;
  	uint8_t brightness;
  	uint8_t low_br;
	uint8_t deltaValue,step;
};

class FieryLedLampEffectBall: public FieryLedLampEffect
{
public:
	FieryLedLampEffectBall():FieryLedLampEffect(DYNAMIC_DELAY){};
	void setup();
	void updateInner();
private:
	int16_t coordB[2U];
	int8_t vectorB[2U], deltaValue;
	CHSV _pulse_color;
	CRGB ballColor;
};

class FieryLedLampEffectWine: public FieryLedLampEffect
{
public:
	FieryLedLampEffectWine():FieryLedLampEffect(DYNAMIC_DELAY){};
	void setup();
	void updateInner();
private:
	uint8_t deltaHue, deltaHue2, step, pcnt;
};

class FieryLedLampEffectMadnessNoise: public FieryLedLampEffect
{
public:
	FieryLedLampEffectMadnessNoise():FieryLedLampEffect(HIGH_DELAY){};
	void setup();
	void updateInner();
private:
	void fillnoise8();

	uint8_t noise[LED_WIDTH][LED_WIDTH];
	uint16_t x,y,z;
};

// ============= ЭФФЕКТ СТАЯ ===============
// https://github.com/pixelmatix/aurora/blob/master/PatternFlock.h
// Адаптация от (c) SottNick и @kDn

template <class T>
class Vector2 {
  public:
    T x, y;

    Vector2() : x(0), y(0) {}
    Vector2(T x, T y) : x(x), y(y) {}
    Vector2(const Vector2& v) : x(v.x), y(v.y) {}

    Vector2& operator=(const Vector2& v) {
      x = v.x;
      y = v.y;
      return *this;
    }

    bool isEmpty() {
      return x == 0 && y == 0;
    }

    bool operator==(Vector2& v) {
      return x == v.x && y == v.y;
    }

    bool operator!=(Vector2& v) {
      return !(x == y);
    }

    Vector2 operator+(Vector2& v) {
      return Vector2(x + v.x, y + v.y);
    }
    Vector2 operator-(Vector2& v) {
      return Vector2(x - v.x, y - v.y);
    }

    Vector2& operator+=(Vector2& v) {
      x += v.x;
      y += v.y;
      return *this;
    }
    Vector2& operator-=(Vector2& v) {
      x -= v.x;
      y -= v.y;
      return *this;
    }

    Vector2 operator+(double s) {
      return Vector2(x + s, y + s);
    }
    Vector2 operator-(double s) {
      return Vector2(x - s, y - s);
    }
    Vector2 operator*(double s) {
      return Vector2(x * s, y * s);
    }
    Vector2 operator/(double s) {
      return Vector2(x / s, y / s);
    }

    Vector2& operator+=(double s) {
      x += s;
      y += s;
      return *this;
    }
    Vector2& operator-=(double s) {
      x -= s;
      y -= s;
      return *this;
    }
    Vector2& operator*=(double s) {
      x *= s;
      y *= s;
      return *this;
    }
    Vector2& operator/=(double s) {
      x /= s;
      y /= s;
      return *this;
    }

    void set(T x, T y) {
      this->x = x;
      this->y = y;
    }

    void rotate(double deg) {
      double theta = deg / 180.0 * M_PI;
      double c = cos(theta);
      double s = sin(theta);
      double tx = x * c - y * s;
      double ty = x * s + y * c;
      x = tx;
      y = ty;
    }

    Vector2& normalize() {
      if (length() == 0) return *this;
      *this *= (1.0 / length());
      return *this;
    }

    float dist(Vector2 v) const {
      Vector2 d(v.x - x, v.y - y);
      return d.length();
    }
    float length() const {
      return sqrt(x * x + y * y);
    }

    float mag() const {
      return length();
    }

    float magSq() {
      return (x * x + y * y);
    }

    void truncate(double length) {
      double angle = atan2f(y, x);
      x = length * cos(angle);
      y = length * sin(angle);
    }

    Vector2 ortho() const {
      return Vector2(y, -x);
    }

    static float dot(Vector2 v1, Vector2 v2) {
      return v1.x * v2.x + v1.y * v2.y;
    }
    static float cross(Vector2 v1, Vector2 v2) {
      return (v1.x * v2.y) - (v1.y * v2.x);
    }

    void limit(float max) {
      if (magSq() > max * max) {
        normalize();
        *this *= max;
      }
    }
};

typedef Vector2<float> PVector;

// Flocking
// Daniel Shiffman <http://www.shiffman.net>
// The Nature of Code, Spring 2009

// Boid class
// Methods for Separation, Cohesion, Alignment added

class Boid {
  public:

    PVector location;
    PVector velocity;
    PVector acceleration;
    float maxforce;    // Maximum steering force
    float maxspeed;    // Maximum speed

    float desiredseparation = 4;
    float neighbordist = 8;
    byte colorIndex = 0;
    float mass;

    boolean enabled = true;

    Boid() {}

    Boid(float x, float y) {
      acceleration = PVector(0, 0);
      velocity = PVector(randomf(), randomf());
      location = PVector(x, y);
      maxspeed = 1.5;
      maxforce = 0.05;
    }

    static float randomf() {
      return mapfloat(random(0, 255), 0, 255, -.5, .5);
    }

    static float mapfloat(float x, float in_min, float in_max, float out_min, float out_max) {
      return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }

    void run(Boid boids [], uint8_t boidCount) {
      flock(boids, boidCount);
      update();
      // wrapAroundBorders();
      // render();
    }

    // Method to update location
    void update() {
      // Update velocity
      velocity += acceleration;
      // Limit speed
      velocity.limit(maxspeed);
      location += velocity;
      // Reset acceleration to 0 each cycle
      acceleration *= 0;
    }

    void applyForce(PVector force) {
      // We could add mass here if we want A = F / M
      acceleration += force;
    }

    void repelForce(PVector obstacle, float radius) {
      //Force that drives boid away from obstacle.

      PVector futPos = location + velocity; //Calculate future position for more effective behavior.
      PVector dist = obstacle - futPos;
      float d = dist.mag();

      if (d <= radius) {
        PVector repelVec = location - obstacle;
        repelVec.normalize();
        if (d != 0) { //Don't divide by zero.
          // float scale = 1.0 / d; //The closer to the obstacle, the stronger the force.
          repelVec.normalize();
          repelVec *= (maxforce * 7);
          if (repelVec.mag() < 0) { //Don't let the boids turn around to avoid the obstacle.
            repelVec.y = 0;
          }
        }
        applyForce(repelVec);
      }
    }

    // We accumulate a new acceleration each time based on three rules
    void flock(Boid boids [], uint8_t boidCount) {
      PVector sep = separate(boids, boidCount);   // Separation
      PVector ali = align(boids, boidCount);      // Alignment
      PVector coh = cohesion(boids, boidCount);   // Cohesion
      // Arbitrarily weight these forces
      sep *= 1.5;
      ali *= 1.0;
      coh *= 1.0;
      // Add the force vectors to acceleration
      applyForce(sep);
      applyForce(ali);
      applyForce(coh);
    }

    // Separation
    // Method checks for nearby boids and steers away
    PVector separate(Boid boids [], uint8_t boidCount) {
      PVector steer = PVector(0, 0);
      int count = 0;
      // For every boid in the system, check if it's too close
      for (int i = 0; i < boidCount; i++) {
        Boid other = boids[i];
        if (!other.enabled)
          continue;
        float d = location.dist(other.location);
        // If the distance is greater than 0 and less than an arbitrary amount (0 when you are yourself)
        if ((d > 0) && (d < desiredseparation)) {
          // Calculate vector pointing away from neighbor
          PVector diff = location - other.location;
          diff.normalize();
          diff /= d;        // Weight by distance
          steer += diff;
          count++;            // Keep track of how many
        }
      }
      // Average -- divide by how many
      if (count > 0) {
        steer /= (float) count;
      }

      // As long as the vector is greater than 0
      if (steer.mag() > 0) {
        // Implement Reynolds: Steering = Desired - Velocity
        steer.normalize();
        steer *= maxspeed;
        steer -= velocity;
        steer.limit(maxforce);
      }
      return steer;
    }

    // Alignment
    // For every nearby boid in the system, calculate the average velocity
    PVector align(Boid boids [], uint8_t boidCount) {
      PVector sum = PVector(0, 0);
      int count = 0;
      for (int i = 0; i < boidCount; i++) {
        Boid other = boids[i];
        if (!other.enabled)
          continue;
        float d = location.dist(other.location);
        if ((d > 0) && (d < neighbordist)) {
          sum += other.velocity;
          count++;
        }
      }
      if (count > 0) {
        sum /= (float) count;
        sum.normalize();
        sum *= maxspeed;
        PVector steer = sum - velocity;
        steer.limit(maxforce);
        return steer;
      }
      else {
        return PVector(0, 0);
      }
    }

    // Cohesion
    // For the average location (i.e. center) of all nearby boids, calculate steering vector towards that location
    PVector cohesion(Boid boids [], uint8_t boidCount) {
      PVector sum = PVector(0, 0);   // Start with empty vector to accumulate all locations
      int count = 0;
      for (int i = 0; i < boidCount; i++) {
        Boid other = boids[i];
        if (!other.enabled)
          continue;
        float d = location.dist(other.location);
        if ((d > 0) && (d < neighbordist)) {
          sum += other.location; // Add location
          count++;
        }
      }
      if (count > 0) {
        sum /= count;
        return seek(sum);  // Steer towards the location
      }
      else {
        return PVector(0, 0);
      }
    }

    // A method that calculates and applies a steering force towards a target
    // STEER = DESIRED MINUS VELOCITY
    PVector seek(PVector target) {
      PVector desired = target - location;  // A vector pointing from the location to the target
      // Normalize desired and scale to maximum speed
      desired.normalize();
      desired *= maxspeed;
      // Steering = Desired minus Velocity
      PVector steer = desired - velocity;
      steer.limit(maxforce);  // Limit to maximum steering force
      return steer;
    }

    // A method that calculates a steering force towards a target
    // STEER = DESIRED MINUS VELOCITY
    void arrive(PVector target) {
      PVector desired = target - location;  // A vector pointing from the location to the target
      float d = desired.mag();
      // Normalize desired and scale with arbitrary damping within 100 pixels
      desired.normalize();
      if (d < 4) {
        float m = map(d, 0, 100, 0, maxspeed);
        desired *= m;
      }
      else {
        desired *= maxspeed;
      }

      // Steering = Desired minus Velocity
      PVector steer = desired - velocity;
      steer.limit(maxforce);  // Limit to maximum steering force
      applyForce(steer);
      //Serial.println(d);
    }

    void wrapAroundBorders() {
      if (location.x < 0) location.x = LED_WIDTH - 1;
      if (location.y < 0) location.y = LED_HEIGHT - 1;
      if (location.x >= LED_WIDTH) location.x = 0;
      if (location.y >= LED_HEIGHT) location.y = 0;
    }

    void avoidBorders() {
      PVector desired = velocity;

      if (location.x < 8) desired = PVector(maxspeed, velocity.y);
      if (location.x >= LED_WIDTH - 8) desired = PVector(-maxspeed, velocity.y);
      if (location.y < 8) desired = PVector(velocity.x, maxspeed);
      if (location.y >= LED_HEIGHT - 8) desired = PVector(velocity.x, -maxspeed);

      if (desired != velocity) {
        PVector steer = desired - velocity;
        steer.limit(maxforce);
        applyForce(steer);
      }

      if (location.x < 0) location.x = 0;
      if (location.y < 0) location.y = 0;
      if (location.x >= LED_WIDTH) location.x = LED_WIDTH - 1;
      if (location.y >= LED_HEIGHT) location.y = LED_HEIGHT - 1;
    }

    bool bounceOffBorders(float bounce) {
      bool bounced = false;

      if (location.x >= LED_WIDTH) {
        location.x = LED_WIDTH - 1;
        velocity.x *= -bounce;
        bounced = true;
      }
      else if (location.x < 0) {
        location.x = 0;
        velocity.x *= -bounce;
        bounced = true;
      }

      if (location.y >= LED_HEIGHT) {
        location.y = LED_HEIGHT - 1;
        velocity.y *= -bounce;
        bounced = true;
      }
      else if (location.y < 0) {
        location.y = 0;
        velocity.y *= -bounce;
        bounced = true;
      }

      return bounced;
    }

    void render() {
      // // Draw a triangle rotated in the direction of velocity
      // float theta = velocity.heading2D() + radians(90);
      // fill(175);
      // stroke(0);
      // pushMatrix();
      // translate(location.x,location.y);
      // rotate(theta);
      // beginShape(TRIANGLES);
      // vertex(0, -r*2);
      // vertex(-r, r*2);
      // vertex(r, r*2);
      // endShape();
      // popMatrix();
      // backgroundLayer.drawPixel(location.x, location.y, CRGB::Blue);
    }
};

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

class FieryLedLampEffectStarFall: public FieryLedLampEffect
{
public:
	FieryLedLampEffectStarFall():FieryLedLampEffect(DYNAMIC_DELAY){};
	void setup();
	void updateInner();
private:
};

class FieryLedLampEffectDNA: public FieryLedLampEffect
{
public:
	FieryLedLampEffectDNA():FieryLedLampEffect(LOW_DELAY){};
	void setup();
	void updateInner();
private:
	uint8_t step, deltaHue;
};

class FieryLedLampEffectSmoke: public FieryLedLampEffect
{
public:
	FieryLedLampEffectSmoke(bool _colored):FieryLedLampEffect(DYNAMIC_DELAY),colored(_colored){};
	~FieryLedLampEffectSmoke();

	void setup();
	void updateInner();
private:
	bool colored;
	uint8_t deltaHue, deltaHue2;
	uint32_t noise32_x,noise32_y,noise32_z;
	uint8_t **noise3d;
};

class FieryLedLampEffectSmokeBalls: public FieryLedLampEffect
{
public:
	FieryLedLampEffectSmokeBalls():FieryLedLampEffect(LOW_DELAY){};
	void setup();
	void updateInner();
private:
	uint8_t enlargedObjectNUM;
	float speedfactor;
};

#define enlargedOBJECT_MAX_COUNT                     (LED_WIDTH * 2) // максимальное количество сложных отслеживаемых объектов (меньше, чем trackingOBJECT_MAX_COUNT)
class FieryLedLampEffectLiquidLamp: public FieryLedLampEffect
{
public:
	FieryLedLampEffectLiquidLamp(bool colored):FieryLedLampEffect(LOW_DELAY),isColored(colored){};
	void setup();
	void updateInner();
private:
	void fillPalette(uint8_t hue, bool isInvert = false);
	void position();
	void physic();

	bool isColored;
	float speedfactor;
	uint8_t deltaHue, enlargedObjectNUM;
	float liquidLampHot[enlargedOBJECT_MAX_COUNT], liquidLampSpf[enlargedOBJECT_MAX_COUNT];
	uint8_t liquidLampMX[enlargedOBJECT_MAX_COUNT], liquidLampSC[enlargedOBJECT_MAX_COUNT], liquidLampTR[enlargedOBJECT_MAX_COUNT];
	CRGBPalette16 palette;
};

class FieryLedLampEffectSwirl: public FieryLedLampEffect
{
public:
	FieryLedLampEffectSwirl():FieryLedLampEffect(DYNAMIC_DELAY){};
	void setup();
	void updateInner();
private:
	uint8_t step, deltaValue, deltaHue, deltaHue2, custom_eff;
};

class FieryLedLampEffectStars: public FieryLedLampEffect
{
public:
	FieryLedLampEffectStars():FieryLedLampEffect(DYNAMIC_DELAY){};
	void setup();
	void updateInner();
private:
#define STARS_NUM (8U)
#define STAR_BLENDER (128U)
#define CENTER_DRIFT_SPEED (6U)
	uint8_t points[STARS_NUM];
  	float color[STARS_NUM] ;
  	int delay_arr[STARS_NUM];
  	float counter;
  	float driftx;
  	float  drifty;
  	float cangle;
  	float  sangle;
  	uint8_t stars_count;
};

class FieryLedLampEffectZebra: public FieryLedLampEffect
{
public:
	FieryLedLampEffectZebra():FieryLedLampEffect(HIGH_DELAY){};
	void setup();
	void updateInner();
private:
};

class FieryLedLampEffectTixyLand: public FieryLedLampEffect
{
public:
	FieryLedLampEffectTixyLand():FieryLedLampEffect(DYNAMIC_DELAY){};
	void setup();
	void updateInner();
private:
	void processFrame(double t, double x, double y);
	float code(double t, double i, double x, double y);

	uint8_t deltaHue2, pcnt;
};

class FieryLedLampEffectSnakes: public FieryLedLampEffect
{
public:
	FieryLedLampEffectSnakes():FieryLedLampEffect(LOW_DELAY){};
	void setup();
	void updateInner();
private:
	float speedfactor;
	uint8_t enlargedObjectNUM;
	unsigned long enlargedObjectTime[enlargedOBJECT_MAX_COUNT];
};

class FieryLedLampEffectFontain: public FieryLedLampEffect
{
public:
	FieryLedLampEffectFontain():FieryLedLampEffect(DYNAMIC_DELAY){};
	void setup();
	void updateInner();
private:
	void starfield2Emit(uint8_t i);
	void particlesUpdate2(uint8_t i);

	uint8_t enlargedObjectNUM, deltaValue;
};

class FieryLedLampEffectDrops: public FieryLedLampEffect
{
public:
	FieryLedLampEffectDrops():FieryLedLampEffect(LOW_DELAY){};
	void setup();
	void updateInner();
private:
	uint8_t enlargedObjectNUM;
};

class FieryLedLampEffectRings: public FieryLedLampEffect
{
public:
	FieryLedLampEffectRings():FieryLedLampEffect(DYNAMIC_DELAY){};
	void setup();
	void updateInner();
private:
	uint8_t deltaHue, deltaHue2, deltaValue;
	uint8_t noise3d[LED_HEIGHT]; // начальный оттенок каждого кольца (оттенка из палитры) 0-255
	uint8_t shiftValue[LED_HEIGHT]; // местоположение начального оттенка кольца 0-WIDTH-1
	uint8_t shiftHue[LED_HEIGHT]; // 4 бита на ringHueShift, 4 на ringHueShift2
	uint8_t step;
};

class FieryLedLampEffectComet: public FieryLedLampEffect
{
public:
	FieryLedLampEffectComet():FieryLedLampEffect(DYNAMIC_DELAY){};
	~FieryLedLampEffectComet();
	void setup();
	void updateInner();
private:
	uint32_t noise32_x,noise32_y,noise32_z;
	uint8_t **noise3d;
};

class FieryLedLampEffectCometColor: public FieryLedLampEffect
{
public:
	FieryLedLampEffectCometColor():FieryLedLampEffect(DYNAMIC_DELAY){};
	~FieryLedLampEffectCometColor();
	void setup();
	void updateInner();
private:
	uint32_t noise32_x,noise32_y,noise32_z;
	uint8_t **noise3d;
};

class FieryLedLampEffectCometCount: public FieryLedLampEffect
{
public:
	FieryLedLampEffectCometCount(uint8_t v):FieryLedLampEffect(DYNAMIC_DELAY),count(v){};
	void setup();
	void updateInner();
private:
	uint8_t count, deltaHue;
};

class FieryLedLampEffectContacts: public FieryLedLampEffect
{
public:
	FieryLedLampEffectContacts():FieryLedLampEffect(DYNAMIC_DELAY){};
	void setup();
	void updateInner();
private:
};

class FieryLedLampEffectSparkles: public FieryLedLampEffect
{
public:
	FieryLedLampEffectSparkles():FieryLedLampEffect(DYNAMIC_DELAY){};
	void setup();
	void updateInner();
private:
};

class FieryLedLampEffectCube2D: public FieryLedLampEffect
{
public:
	FieryLedLampEffectCube2D():FieryLedLampEffect(DYNAMIC_DELAY){};
	void setup();
	void updateInner();
private:
	uint8_t step, deltaValue;
	uint8_t razmerX, razmerY; // размеры ячеек по горизонтали / вертикали
	uint8_t deltaHue,deltaHue2;
	int8_t globalShiftX, globalShiftY; // нужно ли сдвинуть всё поле по окончаии цикла и в каком из направлений (-1, 0, +1)
	uint8_t shtukX, shtukY; // количество ячеек по горизонтали / вертикали
	uint8_t poleX, poleY; // размер всего поля по горизонтали / вертикали (в том числе 1 дополнительная пустая дорожка-разделитель с какой-то из сторон)
	bool seamlessX; // получилось ли сделать поле по Х бесшовным
	bool krutimVertikalno; // направление вращения в данный момент

	uint8_t noise3d[LED_WIDTH][LED_HEIGHT];
};

class FieryLedLampEffectLava: public FieryLedLampEffect
{
public:
	FieryLedLampEffectLava():FieryLedLampEffect(HIGH_DELAY){};
	void setup();
	void updateInner();
private:
};

class FieryLedLampEffectLavaLamp: public FieryLedLampEffect
{
public:
	FieryLedLampEffectLavaLamp():FieryLedLampEffect(LOW_DELAY){};
	void setup();
	void updateInner();
private:
	void LavaLampGetspeed(uint8_t l);
	void drawBlob(uint8_t l, CRGB color);

	uint8_t enlargedObjectNUM;
};

class FieryLedLampEffectButterflyLamp: public FieryLedLampEffect
{
public:
	FieryLedLampEffectButterflyLamp(bool colored):FieryLedLampEffect(LOW_DELAY),isColored(colored){};
	void setup();
	void updateInner();
private:
	bool isColored;
	float speedfactor;
	uint8_t deltaValue, step, deltaHue;
};

class FieryLedLampEffectForest: public FieryLedLampEffect
{
public:
	FieryLedLampEffectForest():FieryLedLampEffect(HIGH_DELAY){};
	void setup();
	void updateInner();
private:
};

class FieryLedLampEffectLumenjer: public FieryLedLampEffect
{
public:
	FieryLedLampEffectLumenjer():FieryLedLampEffect(DYNAMIC_DELAY){};
	void setup();
	void updateInner();
private:
	uint8_t deltaHue,deltaHue2, step;
};

class FieryLedLampEffectLeapers: public FieryLedLampEffect
{
public:
	FieryLedLampEffectLeapers():FieryLedLampEffect(DYNAMIC_DELAY){};
	void setup();
	void updateInner();
protected:
	void move(uint8_t l);
	void restart(uint8_t l);

	uint8_t enlargedObjectNUM;
};

class FieryLedLampEffectMagma: public FieryLedLampEffectLeapers
{
public:
	FieryLedLampEffectMagma(){};
	void setup();
	void updateInner();
private:
	uint8_t deltaValue, deltaHue, shiftHue[LED_HEIGHT], ff_y,ff_z;
};

class FieryLedLampEffectOilPaints: public FieryLedLampEffect
{
public:
	FieryLedLampEffectOilPaints():FieryLedLampEffect(DYNAMIC_DELAY){};
	void setup();
	void updateInner();
private:
	uint8_t step,deltaValue, deltaHue, deltaHue2;
	uint16_t max_val;
};

class FieryLedLampEffectMatrix: public FieryLedLampEffect
{
public:
	FieryLedLampEffectMatrix():FieryLedLampEffect(DYNAMIC_DELAY){};
	void setup();
	void updateInner();
private:
};

class FieryLedLampEffectTwinkles: public FieryLedLampEffect
{
public:
	FieryLedLampEffectTwinkles():FieryLedLampEffect(DYNAMIC_DELAY){};
	void setup();
	void updateInner();
private:
	uint8_t deltaValue;
};

class FieryLedLampEffectMetaballs: public FieryLedLampEffect
{
public:
	FieryLedLampEffectMetaballs():FieryLedLampEffect(LOW_DELAY){};
	void setup();
	void updateInner();
private:
	float speedfactor;
};

class FieryLedLampEffectWebTool: public FieryLedLampEffect
{
public:
	FieryLedLampEffectWebTool():FieryLedLampEffect(SOFT_DELAY){};
	void setup();
	void updateInner();
private:
	int getRandomPos(uint8_t STEP);
	int getHue(uint8_t x, uint8_t y);
	uint8_t getSaturationStep();
	uint8_t getBrightnessStep();
	void drawPalette(int posX, int posY, uint8_t _STEP);

	float speedfactor;
	uint8_t step;
};

class FieryLedLampEffectMosaic: public FieryLedLampEffect
{
public:
	FieryLedLampEffectMosaic():FieryLedLampEffect(DYNAMIC_DELAY){};
	void setup();
	void updateInner();
private:
	uint8_t poleX,poleY,line[LED_WIDTH / 3U + 1U], shiftValue[LED_HEIGHT / 3U + 1U];
};

class FieryLedLampEffectBballs: public FieryLedLampEffect
{
public:
	FieryLedLampEffectBballs():FieryLedLampEffect(LOW_DELAY){};
	void setup();
	void updateInner();
private:
	uint8_t enlargedObjectNUM, deltaHue, deltaValue;
	unsigned long enlargedObjectTime[enlargedOBJECT_MAX_COUNT];
};

class FieryLedLampEffectBallsBounce: public FieryLedLampEffect
{
public:
	FieryLedLampEffectBallsBounce():FieryLedLampEffect(LOW_DELAY){};
	~FieryLedLampEffectBallsBounce();
	void setup();
	void updateInner();
private:
	uint8_t enlargedObjectNUM;
	Boid *boids;
};

class FieryLedLampEffectChristmasTree: public FieryLedLampEffect
{
public:
	FieryLedLampEffectChristmasTree():FieryLedLampEffect(DYNAMIC_DELAY){};
	void setup();
	void updateInner();
private:
};

class FieryLedLampEffectFireFlyTop: public FieryLedLampEffect
{
public:
	FieryLedLampEffectFireFlyTop():FieryLedLampEffect(DYNAMIC_DELAY){};
	~FieryLedLampEffectFireFlyTop();
	void setup();
	void updateInner();
private:
	uint8_t **noise3d;
	uint16_t noise32_x,noise32_y,noise32_z, scale32_x,scale32_y;
};

class FieryLedLampEffectList
{
public:
	void set_effect(uint16_t index);

	void update();
private:
	//FieryLedLampEffect effects
};
#endif