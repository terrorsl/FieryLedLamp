#ifndef EFFECT_FILE
#define EFFECT_FILE

#include"platform.h"
#include"Constants.h"
#include<FastLED.h>

//константы размера матрицы вычисляется только здесь и не меняется в эффектах
#define CENTER_X_MINOR ((WIDTH / 2) -  ((WIDTH - 1) & 0x01)) // центр матрицы по ИКСУ, сдвинутый в меньшую сторону, если ширина чётная
#define CENTER_Y_MINOR ((HEIGHT / 2) - ((HEIGHT - 1) & 0x01)) // центр матрицы по ИГРЕКУ, сдвинутый в меньшую сторону, если высота чётная
const uint8_t CENTER_X_MAJOR =   WIDTH / 2  + (WIDTH % 2);           // центр матрицы по ИКСУ, сдвинутый в большую сторону, если ширина чётная
const uint8_t CENTER_Y_MAJOR =  HEIGHT / 2  + (HEIGHT % 2);          // центр матрицы по ИГРЕКУ, сдвинутый в большую сторону, если высота чётная

typedef enum{
	WhiteColor, // Бeлый cвeт
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
/*#define EFF_STARFALL            ( 14U)    // Вьюга
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
	MaxEffect
}FieryLedLampEffectTypes;

#define HIGH_DELAY 0
#define LOW_DELAY 1
#define DYNAMIC_DELAY 2

class FieryLedLampEffect
{
public:
	FieryLedLampEffect(uint8_t delayType):delat_type(delayType){};
	virtual ~FieryLedLampEffect(){}

  void set_bright(unsigned char val){}
  void set_speed(unsigned char val){speed=val;}
  void set_scale(unsigned char val){scale=val;}

	virtual void setup() = 0;
	virtual void updateInner() = 0;

	void update();
protected:
  // функция отрисовки точки по координатам X Y
	void drawPixelXY(int8_t x, int8_t y, CRGB color);
	//по мотивам
	//https://gist.github.com/sutaburosu/32a203c2efa2bb584f4b846a91066583
	void drawPixelXYF(float x, float y, CRGB color);
	// функция получения цвета пикселя по его номеру
	uint32_t getPixColor(uint32_t thisSegm);
	// функция получения цвета пикселя в матрице по его координатам
	uint32_t getPixColorXY(uint8_t x, uint8_t y);
	// залить все
	void fillAll(CRGB color);
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
	uint8_t noise3d[NUM_LAYERSMAX][WIDTH][HEIGHT];     // двухслойная маска или хранилище свойств в размер всей матрицы
};

class FieryLedLampEffectPool: public FieryLedLampEffect
{
public:
	FieryLedLampEffectPool():FieryLedLampEffect(DYNAMIC_DELAY){};
	void setup();
	void updateInner();
private:
	uint8_t deltaHue,deltaHue2,step;
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

class FieryLedLampEffectWave: public FieryLedLampEffect
{
public:
	FieryLedLampEffectWave():FieryLedLampEffect(DYNAMIC_DELAY){};
	void setup();
	void updateInner();
private:
	uint8_t waveCount;
	uint8_t waveRotation;
};

class FieryLedLampEffectWaterfall: public FieryLedLampEffect
{
public:
	FieryLedLampEffectWaterfall():FieryLedLampEffect(DYNAMIC_DELAY){};
	void setup();
	void updateInner();
private:
	#define NUM_LAYERSMAX 2
	uint8_t noise3d[NUM_LAYERSMAX][WIDTH][HEIGHT];     // двухслойная маска или хранилище свойств в размер всей матрицы
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

	uint8_t noise[WIDTH][WIDTH];
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
      if (location.x < 0) location.x = WIDTH - 1;
      if (location.y < 0) location.y = HEIGHT - 1;
      if (location.x >= WIDTH) location.x = 0;
      if (location.y >= HEIGHT) location.y = 0;
    }

    void avoidBorders() {
      PVector desired = velocity;

      if (location.x < 8) desired = PVector(maxspeed, velocity.y);
      if (location.x >= WIDTH - 8) desired = PVector(-maxspeed, velocity.y);
      if (location.y < 8) desired = PVector(velocity.x, maxspeed);
      if (location.y >= HEIGHT - 8) desired = PVector(velocity.x, -maxspeed);

      if (desired != velocity) {
        PVector steer = desired - velocity;
        steer.limit(maxforce);
        applyForce(steer);
      }

      if (location.x < 0) location.x = 0;
      if (location.y < 0) location.y = 0;
      if (location.x >= WIDTH) location.x = WIDTH - 1;
      if (location.y >= HEIGHT) location.y = HEIGHT - 1;
    }

    bool bounceOffBorders(float bounce) {
      bool bounced = false;

      if (location.x >= WIDTH) {
        location.x = WIDTH - 1;
        velocity.x *= -bounce;
        bounced = true;
      }
      else if (location.x < 0) {
        location.x = 0;
        velocity.x *= -bounce;
        bounced = true;
      }

      if (location.y >= HEIGHT) {
        location.y = HEIGHT - 1;
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