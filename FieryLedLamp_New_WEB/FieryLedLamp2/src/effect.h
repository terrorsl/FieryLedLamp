#ifndef EFFECT_FILE
#define EFFECT_FILE

#include"platform.h"
#include"Constants.h"
#include<FastLED.h>

typedef enum{
	WhiteColor, // Бeлый cвeт
	WaterColor, // Акварель
	FlowerRuta, // Аленький цветочек
	Pool, // Бассейн
	Bamboo, // Бамбук
	/*Madness, // Безумие
#define EFF_BALLROUTINE         (  6U)    // Блуждающий кубик
	Waterfall, // Водопад
#define EFF_WATERFALL_4IN1      (  8U)    // Водопад 4в1
	Waves, // Волны
#define EFF_MAGICLANTERN        ( 10U)    // Волшебный Фонарик
	Wine, // Вино
#define EFF_WHIRL               ( 12U)    // Вихри пламени
#define EFF_WHIRL_MULTI         ( 13U)    // Вихри разноцветные
#define EFF_STARFALL            ( 14U)    // Вьюга
#define EFF_STORMY_RAIN         ( 15U)    // Гроза в банке
#define EFF_DNA                 ( 16U)    // ДНК
	Smoke, // Дым
	SmokeColor, // Дым разноцветный
#define EFF_SMOKEBALLS          ( 19U)    // Дымовые шашки
#define EFF_LIQUIDLAMP          ( 20U)    // Жидкая лампа
#define EFF_LIQUIDLAMP_AUTO     ( 21U)    // Жидкая лампа авто
#define EFF_SWIRL               ( 22U)    // Завиток
#define EFF_STARS               ( 23U)    // Звезды
#define EFF_ZEBRA               ( 24U)    // Зебра
#define EFF_TIXYLAND            ( 25U)    // Земля Тикси
#define EFF_SNAKES              ( 26U)    // Змейки
#define EFF_FOUNTAIN            ( 27U)    // Источник
#define EFF_DROP_IN_WATER       ( 28U)    // Капли на воде
#define EFF_DROPS               ( 29U)    // Капли на стекле
#define EFF_LLAND               ( 30U)    // Кипение
#define EFF_RINGS               ( 31U)    // Кодовый замок
#define EFF_COMET               ( 32U)    // Комета
#define EFF_COMET_COLOR         ( 33U)    // Комета одноцветная
#define EFF_COMET_TWO           ( 34U)    // Комета двойная
#define EFF_COMET_THREE         ( 35U)    // Комета тройная
#define EFF_CONTACTS            ( 36U)    // Контакты
#define EFF_SPARKLES            ( 37U)    // Конфетти
#define EFF_CUBE2D              ( 38U)    // Кубик Рубика
	Lava, // Лава
#define EFF_LAVALAMP            ( 40U)    // Лавовая лампа
#define EFF_BUTTERFLYS_LAMP     ( 41U)    // Лампа с мотыльками
#define EFF_FOREST              ( 42U)    // Лес
#define EFF_LUMENJER            ( 43U)    // Люмeньep
#define EFF_MAGMA               ( 44U)    // Магма
#define EFF_PAINTS              ( 45U)    // Масляные краски
#define EFF_MATRIX              ( 46U)    // Матрица
#define EFF_TWINKLES            ( 47U)    // Мерцание
#define EFF_METABALLS           ( 48U)    // Метоболз
#define EFF_WEB_TOOLS           ( 49U)    // Мечта дизайнера
#define EFF_MOSAIC              ( 50U)    // Мозайка
#define EFF_BUTTERFLYS          ( 51U)    // Moтыльки
#define EFF_BBALLS              ( 52U)    // Мячики
#define EFF_BALLS_BOUNCE        ( 53U)    // Мячики без границ
#define EFF_CHRISTMAS_TREE      ( 54U)    // Новогодняя Елка
#define EFF_FIRE                ( 55U)    // Огонь
#define EFF_FIRE_2012           ( 56U)    // Огонь 2012
#define EFF_FIRE_2018           ( 57U)    // Огонь 2018
#define EFF_FIRE_2020           ( 58U)    // Огонь 2020
#define EFF_FIRE_2021           ( 59U)    // Огонь 2021
#define EFF_FIREFLY_TOP         ( 60U)    // Огoнь верховой
#define EFF_FIREFLY             ( 61U)    // Огoнь парящий
#define EFF_FIRESPARKS          ( 62U)    // Огонь с искрами
#define EFF_COLOR_RAIN          ( 63U)    // Осадки
#define EFF_OSCILLATING         ( 64U)    // Осциллятор
#define EFF_CLOUDS              ( 65U)    // Облака
#define EFF_OCEAN               ( 66U)    // Океан
#define EFF_OCTOPUS             ( 67U)    // Осьминог
#define EFF_RAINBOW_STRIPE      ( 68U)    // Павлин
#define EFF_HOURGLASS           ( 69U)    // Песочные часы
#define EFF_PAINTBALL           ( 70U)    // Пейнтбол
#define EFF_PICASSO             ( 71U)    // Пикассо
#define EFF_PLASMA              ( 72U)    // Плазма
#define EFF_SPIDER              ( 73U)    // Плазменная лампа
#define EFF_PLASMA_WAVES        ( 74U)    // Плазменные волны
#define EFF_FLAME               ( 75U)    // Пламя
#define EFF_PLANETEARTH         ( 76U)    // Планета Земля
#define EFF_BY_EFFECT           ( 77U)    // Побочный эффект
#define EFF_POPCORN             ( 78U)    // Попкорн
#define EFF_PRISMATA            ( 79U)    // Призмата
#define EFF_ATTRACT             ( 80U)    // Притяжение
#define EFF_LEAPERS             ( 81U)    // Пpыгyны
#define EFF_PULSE               ( 82U)    // Пульс
#define EFF_PULSE_WHITE         ( 83U)    // Пульс белый
#define EFF_PULSE_RAINBOW       ( 84U)    // Пульс радужный
#define EFF_RADIAL_WAWE         ( 85U)    // Радиальная волна
#define EFF_RAINBOW_VER         ( 86U)    // Радуга
#define EFF_RAINBOW             ( 87U)    // Радуга 3D
#define EFF_SNAKE               ( 88U)    // Радужный змей
#define EFF_DANDELIONS          ( 89U)    // Разноцветные одуванчики
#define EFF_RAIN                ( 90U)    // Разноцветный дождь
#define EFF_RIVERS              ( 91U)    // Реки Ботсваны
#define EFF_LIGHTERS            ( 92U)    // Светлячки
#define EFF_LIGHTER_TRACES      ( 93U)    // Светлячки со шлейфом
#define EFF_FEATHER_CANDLE      ( 94U)    // Свеча
#define EFF_AURORA              ( 95U)    // Северное сияние
#define EFF_SERPENTINE          ( 96U)    // Серпантин
#define EFF_SINUSOID3           ( 97U)    // Синусоид
#define EFF_COLORS              ( 98U)    // Смена цвета
#define EFF_SNOW                ( 99U)    // Снегопад
#define EFF_SPECTRUM            (100U)    // Спектрум
#define EFF_SPIRO               (101U)    // Спирали
#define EFF_FLOCK               (102U)    // Стая
#define EFF_FLOCK_N_PR          (103U)    // Стая и хищник
#define EFF_ARROWS              (104U)    // Стрелки
#define EFF_STROBE              (105U)    // Строб.Хаос.Дифузия
#define EFF_SHADOWS             (106U)    // Тени
#define EFF_PACIFIC             (107U)    // Тихий океан
#define EFF_TORNADO             (108U)    // Торнадо
#define EFF_SIMPLE_RAIN         (109U)    // Tyчкa в банке
#define EFF_FIREWORK            (110U)    // Фейерверк
#define EFF_FIREWORK_2          (111U)    // Фейерверк 2
#define EFF_FAIRY               (112U)    // Фея
#define EFF_COLOR               (113U)    // Цвет
#define EFF_EFF_COLORED_PYTHON  (114U)    // Цветной Питон
#define EFF_EFF_SAND            (115U)    // Цветные драже
#define EFF_COLOR_FRIZZLES      (116U)    // Цветные кудри
#define EFF_EFF_LOTUS           (117U)    // Цветок лотоса
#define EFF_TURBULENCE          (118U)    // Цифровая турбулентность
#define EFF_SPHERES             (119U)    // Шapы
#define EFF_NEXUS               (120U)    // Nexus
#define EFF_CLOCK               (121U)    // Часы*/
}FieryLedLampEffectTypes;

#define HIGH_DELAY 0
#define LOW_DELAY 1
#define DYNAMIC_DELAY 2

class FieryLedLampEffect
{
public:
	FieryLedLampEffect(uint8_t delayType):delat_type(delayType){};
	virtual ~FieryLedLampEffect(){}

	virtual void setup() = 0;
	virtual void updateInner() = 0;

	void update();
protected:
	// получить номер пикселя в ленте по координатам
	// библиотека FastLED тоже использует эту функцию
	uint16_t XY(uint8_t x, uint8_t y)
	{
		uint8_t THIS_X;
		uint8_t THIS_Y;
		uint8_t _WIDTH = WIDTH;
		
		switch (ORIENTATION)
		{
		case 0:
			THIS_X = x;                   //CONNECTION_ANGLE == 0 && STRIP_DIRECTION == 0
			THIS_Y =y;
			break;
		case 1:
			_WIDTH = HEIGHT;              //CONNECTION_ANGLE == 0 && STRIP_DIRECTION == 1
			THIS_X = y;
			THIS_Y = x;
			break;
		case 2:
			THIS_X = x;                   //CONNECTION_ANGLE == 1 && STRIP_DIRECTION == 0
			THIS_Y = (HEIGHT - y - 1);
			break;
		case 3:
			_WIDTH = HEIGHT;              //CONNECTION_ANGLE == 1 && STRIP_DIRECTION == 3
			THIS_X = (HEIGHT - y - 1);
			THIS_Y = x;
			break;
		case 4:
			THIS_X = (WIDTH - x - 1);     //CONNECTION_ANGLE == 2 && STRIP_DIRECTION == 2
			THIS_Y = (HEIGHT - y - 1);
			break;
		case 5:
			_WIDTH = HEIGHT;              //CONNECTION_ANGLE == 2 && STRIP_DIRECTION == 3
			THIS_X = (HEIGHT - y - 1);
			THIS_Y = (WIDTH - x - 1);
			break;
		case 6:
			THIS_X = (WIDTH - x - 1);     //CONNECTION_ANGLE == 3 && STRIP_DIRECTION == 2
			THIS_Y =y;
			break;
		case 7:
			_WIDTH = HEIGHT;              //CONNECTION_ANGLE == 3 && STRIP_DIRECTION == 1
			THIS_X = y;
			THIS_Y = (WIDTH - x - 1);
			break;
		default:
			THIS_X = x;                 // !! смотрите инструкцию: https://alexgyver.ru/wp-content/uploads/2018/11/scheme3.jpg
			THIS_Y =y;                  // !! такого сочетания CONNECTION_ANGLE и STRIP_DIRECTION не бывает
			break;
		}
		
		if (!(THIS_Y & 0x01) || MATRIX_TYPE)               // Even rows run forwards
			return (THIS_Y * _WIDTH + THIS_X);
		else                                                  
			return (THIS_Y * _WIDTH + _WIDTH - THIS_X - 1);  // Odd rows run backwards
	}
	// функция отрисовки точки по координатам X Y
	void drawPixelXY(int8_t x, int8_t y, CRGB color);
	//по мотивам
	//https://gist.github.com/sutaburosu/32a203c2efa2bb584f4b846a91066583
	void drawPixelXYF(float x, float y, CRGB color);
	// функция получения цвета пикселя по его номеру
	uint32_t getPixColor(uint32_t thisSegm);
	// функция получения цвета пикселя в матрице по его координатам
	uint32_t getPixColorXY(uint8_t x, uint8_t y)
	{
		return getPixColor(XY(x, y));
	}
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

	uint8_t delat_type;
	uint8_t speed, scale;
	uint8_t hue,hue2;

	unsigned long effectTimer;
};

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

	uint8_t step, deltaValue, deltaHue;
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

class FieryLedLampEffectList
{
public:
	void set_effect(uint16_t index);

	void update();
private:
	//FieryLedLampEffect effects
};
#endif