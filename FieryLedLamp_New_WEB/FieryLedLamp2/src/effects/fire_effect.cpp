#include"effect.h"
#include"effects/fire_effect.h"

extern CRGB leds[NUM_LEDS];
extern uint16_t XY(uint8_t x, uint8_t y);

uint8_t wrapX(int8_t x);
uint8_t wrapY(int8_t y);

// Randomly generate the next line (matrix row)
void FieryLedLampEffectFire::generateLine()
{
	for (uint8_t x = 0U; x < LED_WIDTH; x++) {
    	line[x] = random(127, 255);                             // заполнение случайным образом нижней линии (127, 255) - менее контрастное, (64, 255) - оригинал
  	}
}

void FieryLedLampEffectFire::shiftUp()
{
#if 0
	//подъем кадра
  	for (uint8_t y = LED_HEIGHT - 1U; y > 0U; y--)
	{
    	for (uint8_t x = 0U; x < LED_WIDTH; x++)
		{
      		uint8_t newX = x % 16U;                               // сократил формулу без доп. проверок
      		if (y > 7U) continue;
      		matrixValue[y][newX] = matrixValue[y - 1U][newX];     //смещение пламени (только для зоны очага)
    	}
  	}

  	for (uint8_t x = 0U; x < LED_WIDTH; x++) {                    // прорисовка новой нижней линии
    	uint8_t newX = x % 16U;                                 // сократил формулу без доп. проверок
    	matrixValue[0U][newX] = line[newX];
  	}
#else
	//подъем кадра
  	for (uint8_t y = LED_HEIGHT - 1; y > 0; y--)
	{
		if (y > 7)
			continue;
    	for (uint8_t x = 0; x < LED_WIDTH; x++)
		{
      		matrixValue[y][x] = matrixValue[y - 1][x];     //смещение пламени (только для зоны очага)
    	}
  	}

  	for (uint8_t x = 0; x < LED_WIDTH; x++)
	{                    // прорисовка новой нижней линии
    	matrixValue[0][x] = line[x];
  	}
#endif
}

// draw a frame, interpolating between 2 "key frames"
// @param pcnt percentage of interpolation

//these values are substracetd from the generated values to give a shape to the animation
static const uint8_t valueMask[8][16] PROGMEM =
{
  {0  , 0  , 0  , 32 , 32 , 0  , 0  , 0  , 0  , 0  , 0  , 32 , 32 , 0  , 0  , 0  },
  {0  , 0  , 0  , 64 , 64 , 0  , 0  , 0  , 0  , 0  , 0  , 64 , 64 , 0  , 0  , 0  },
  {0  , 0  , 32 , 96 , 96 , 32 , 0  , 0  , 0  , 0  , 32 , 96 , 96 , 32 , 0  , 0  },
  {0  , 32 , 64 , 128, 128, 64 , 32 , 0  , 0  , 32 , 64 , 128, 128, 64 , 32 , 0  },
  {32 , 64 , 96 , 160, 160, 96 , 64 , 32 , 32 , 64 , 96 , 160, 160, 96 , 64 , 32 },
  {64 , 96 , 128, 192, 192, 128, 96 , 64 , 64 , 96 , 128, 192, 192, 128, 96 , 64 },
  {96 , 128, 160, 255, 255, 160, 128, 96 , 96 , 128, 160, 255, 255, 160, 128, 96 },
  {128, 160, 192, 255, 255, 192, 160, 128, 128, 160, 192, 255, 255, 192, 160, 128}
};
//these are the hues for the fire,
//should be between 0 (red) to about 25 (yellow)
static const uint8_t hueMask[8][16] PROGMEM =
{
  {25, 22, 11, 1 , 1 , 11, 19, 25, 25, 22, 11, 1 , 1 , 11, 19, 25 },
  {25, 19, 8 , 1 , 1 , 8 , 13, 19, 25, 19, 8 , 1 , 1 , 8 , 13, 19 },
  {19, 16, 8 , 1 , 1 , 8 , 13, 16, 19, 16, 8 , 1 , 1 , 8 , 13, 16 },
  {13, 13, 5 , 1 , 1 , 5 , 11, 13, 13, 13, 5 , 1 , 1 , 5 , 11, 13 },
  {11, 11, 5 , 1 , 1 , 5 , 11, 11, 11, 11, 5 , 1 , 1 , 5 , 11, 11 },
  {8 , 5 , 1 , 0 , 0 , 1 , 5 , 8 , 8 , 5 , 1 , 0 , 0 , 1 , 5 , 8  },
  {5 , 1 , 0 , 0 , 0 , 0 , 1 , 5 , 5 , 1 , 0 , 0 , 0 , 0 , 1 , 5  },
  {1 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 1  }
};
#define SPARKLES              (1U)                     // вылетающие угольки вкл выкл

void FieryLedLampEffectFire::drawFrame(uint8_t pcnt, bool isColored)
{
	// прорисовка нового кадра
	int32_t nextv;
#ifdef UNIVERSE_FIRE // если определен универсальный огонь  
	  //  uint8_t baseHue = (float)modes[currentMode].Scale * 2.55;
	uint8_t baseHue = (float)(scale - 1U) * 2.6;
#else
  	uint8_t baseHue = isColored ? 255U : 0U;
#endif
  	uint8_t baseSat = (scale < 100) ? 255U : 0U;  // вычисление базового оттенка


  	//first row interpolates with the "next" line
  	uint8_t deltaHue = random(0U, 2U) ? constrain(shiftHue[0] + random(0U, 2U) - random(0U, 2U), 15U, 17U) : shiftHue[0]; // random(0U, 2U)= скорость смещения языков чем больше 2U - тем медленнее
  	// 15U, 17U - амплитуда качания -1...+1 относительно 16U
  	// высчитываем плавную дорожку смещения всполохов для нижней строки
  	// так как в последствии координаты точки будут исчисляться из остатка, то за базу можем принять кратную ширину матрицы hueMask
  	// ширина матрицы hueMask = 16, поэтому нам нужно получить диапазон чисел от 15 до 17
  	// далее к предыдущему значению прибавляем случайную 1 и отнимаем случайную 1 - это позволит плавным образом менять значение смещения
  	shiftHue[0] = deltaHue; // заносим это значение в стэк

  	uint8_t deltaValue = random(0U, 3U) ? constrain(shiftValue[0] + random(0U, 2U) - random(0U, 2U), 15U, 17U) : shiftValue[0]; // random(0U, 3U)= скорость смещения очага чем больше 3U - тем медленнее
  	// 15U, 17U - амплитуда качания -1...+1 относительно 16U
  	shiftValue[0] = deltaValue;
#if 0
	for (uint8_t x = 0U; x < LED_WIDTH; x++)
	{                                          // прорисовка нижней строки (сначала делаем ее, так как потом будем пользоваться ее значением смещения)
    	uint8_t newX = x % 16;                                                        // сократил формулу без доп. проверок
    	nextv =                                                               // расчет значения яркости относительно valueMask и нижерасположенной строки.
      		(((100.0 - pcnt) * matrixValue[0][newX] + pcnt * line[newX]) / 100.0)
      		- pgm_read_byte(&valueMask[0][(x + deltaValue) % 16U]);
    	CRGB color = CHSV(                                                            // вычисление цвета и яркости пикселя
                   baseHue + pgm_read_byte(&hueMask[0][(x + deltaHue) % 16U]),    // H - смещение всполохов
                   baseSat,                                                       // S - когда колесо масштаба =100 - белый огонь (экономим на 1 эффекте)
                   (uint8_t)max(0, nextv)                                         // V
                 );
    	leds[XY(x, 0)] = color;                                            // прорисовка цвета очага
  	}
#else
	for (uint8_t x = 0U; x < LED_WIDTH; x++)
	{                                          // прорисовка нижней строки (сначала делаем ее, так как потом будем пользоваться ее значением смещения)
    	nextv =                                                               // расчет значения яркости относительно valueMask и нижерасположенной строки.
      		(((100.0 - pcnt) * matrixValue[0][x] + pcnt * line[x]) / 100.0)
      		- pgm_read_byte(&valueMask[0][(x + deltaValue) % 16U]);
    	CRGB color = CHSV(                                                            // вычисление цвета и яркости пикселя
                   baseHue + pgm_read_byte(&hueMask[0][(x + deltaHue) % 16U]),    // H - смещение всполохов
                   baseSat,                                                       // S - когда колесо масштаба =100 - белый огонь (экономим на 1 эффекте)
                   (uint8_t)max(0, nextv)                                         // V
                 );
    	leds[XY(x, 0)] = color;                                            // прорисовка цвета очага
  	}
#endif

  	//each row interpolates with the one before it
  	for (uint8_t y = LED_HEIGHT - 1U; y > 0U; y--) {                                      // прорисовка остальных строк с учетом значения низлежащих
    	deltaHue = shiftHue[y];                                                         // извлекаем положение
    	shiftHue[y] = shiftHue[y - 1];                                                  // подготавлеваем значение смешения для следующего кадра основываясь на предыдущем
    	deltaValue = shiftValue[y];                                                     // извлекаем положение
    	shiftValue[y] = shiftValue[y - 1];                                              // подготавлеваем значение смешения для следующего кадра основываясь на предыдущем

    	if (y > 8U) {                                                                   // цикл стирания текущей строоки для искр
    		for (uint8_t _x = 0U; _x < LED_WIDTH; _x++) {                                     // стираем строчку с искрами (очень не оптимально)
        		drawPixelXY(_x, y, 0U);
      		}
    	}
	    for (uint8_t x = 0U; x < LED_WIDTH; x++) {                                          // пересчет координаты x для текущей строки
    		uint8_t newX = x % 16U;                                                       // функция поиска позиции значения яркости для матрицы valueMask
      		if (y < 8U) {                                                                 // если строка представляет очаг
        		nextv =                                                                     // расчет значения яркости относительно valueMask и нижерасположенной строки.
          			(((100.0 - pcnt) * matrixValue[y][newX]
            		+ pcnt * matrixValue[y - 1][newX]) / 100.0)
          			- pgm_read_byte(&valueMask[y][(x + deltaValue) % 16U]);

        		CRGB color = CHSV(                                                                  // определение цвета пикселя
                       baseHue + pgm_read_byte(&hueMask[y][(x + deltaHue) % 16U ]),         // H - смещение всполохов
                       baseSat,                                                             // S - когда колесо масштаба =100 - белый огонь (экономим на 1 эффекте)
                       (uint8_t)max(0, nextv)                                               // V
                     );
        		leds[XY(x, y)] = color;
      		}
      		else if (y == 8U && SPARKLES) {                                               // если это самая нижняя строка искр - формитуем искорку из пламени
        		if (random(0, 20) == 0 && getPixelColorXY(x, y - 1U) != 0U) drawPixelXY(x, y, getPixelColorXY(x, y - 2U));  // 20 = обратная величина количества искр
        		else drawPixelXY(x, y, 0U);
      		}
      		else if (SPARKLES) {                                                          // если это не самая нижняя строка искр - перемещаем искорку выше
        		// старая версия для яркости
        		newX = (random(0, 4)) ? x : (x + LED_HEIGHT + random(0U, 2U) - random(0U, 2U)) % LED_HEIGHT ;   // с вероятностью 1/3 смещаем искорку влево или вправо
        		if (getPixelColorXY(x, y - 1U) > 0U) drawPixelXY(newX, y, getPixelColorXY(x, y - 1U));    // рисуем искорку на новой строчке
      		}
    	}
  	}
}

void FieryLedLampEffectFire::setup()
{
	generateLine();
    pcnt = 0;
}

void FieryLedLampEffectFire::updateInner()
{
	if (pcnt >= 30) {                                         // внутренний делитель кадров для поднимающегося пламени
    	shiftUp();                                              // смещение кадра вверх
    	generateLine();                                         // перерисовать новую нижнюю линию случайным образом
    	pcnt = 0;
  	}
  	drawFrame(pcnt, true);
  	pcnt += 25;  // делитель кадров: задает скорость подъема пламени 25/100 = 1/4
}

// ============= ЭФФЕКТ ОГОНЬ 2012 ===============
// там выше есть его копии для эффектов Водопад и Водопад 4 в 1
// по идее, надо бы объединить и оптимизировать, но мелких отличий довольно много
// based on FastLED example Fire2012WithPalette: https://github.com/FastLED/FastLED/blob/master/examples/Fire2012WithPalette/Fire2012WithPalette.ino

void FieryLedLampEffectFire2012::setup()
{
	if (scale > 100)
		scale = 100; // чтобы не было проблем при прошивке без очистки памяти
    if (scale > 50)
      	curPalette = firePalettes[(uint8_t)((scale - 50) / 50.0F * ((sizeof(firePalettes) / sizeof(TProgmemRGBPalette16 *)) - 0.01F))];
    else
      	curPalette = palette_arr[(uint8_t)(scale / 50.0F * ((sizeof(palette_arr) / sizeof(TProgmemRGBPalette16 *)) - 0.01F))];
}

void FieryLedLampEffectFire2012::updateInner()
{
#if LED_HEIGHT/6 > 6
#define FIRE_BASE 6
#else
#define FIRE_BASE LED_HEIGHT/6+1
#endif
  // COOLING: How much does the air cool as it rises?
  // Less cooling = taller flames.  More cooling = shorter flames.
#define cooling 70U
  // SPARKING: What chance (out of 255) is there that a new spark will be lit?
  // Higher chance = more roaring fire.  Lower chance = more flickery fire.
#define sparking 130U
  // SMOOTHING; How much blending should be done between frames
  // Lower = more blending and smoother flames. Higher = less blending and flickery flames
#define fireSmoothing 80U
  // Add entropy to random number generator; we use a lot of it.
  	random16_add_entropy(random(256));

  	// Loop for each column individually
  	for (uint8_t x = 0; x < LED_WIDTH; x++) {
    	// Step 1.  Cool down every cell a little
    	for (uint8_t i = 0; i < LED_HEIGHT; i++) {
      		noise3d[x][i] = qsub8(noise3d[x][i], random(0, ((cooling * 10) / LED_HEIGHT) + 2));
    	}
		// Step 2.  Heat from each cell drifts 'up' and diffuses a little
    	for (uint8_t k = LED_HEIGHT - 1; k > 0; k--) { // fixed by SottNick
      		noise3d[x][k] = (noise3d[x][k - 1] + noise3d[x][k - 1] + noise3d[x][wrapY(k - 2)]) / 3; // fixed by SottNick
    	}
		// Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    	if (random8() < sparking) {
      		uint8_t j = random8(FIRE_BASE);
      		noise3d[x][j] = qadd8(noise3d[x][j], random(160, 255));
    	}
    	// Step 4.  Map from heat cells to LED colors
    	// Blend new data with previous frame. Average data between neighbouring pixels
    	for (uint8_t y = 0; y < LED_HEIGHT; y++)
      		nblend(leds[XY(x, y)], ColorFromPalette(*curPalette, ((noise3d[x][y] * 0.7) + (noise3d[wrapX(x + 1)][y] * 0.3))), fireSmoothing);
	}
}

// ============= ЭФФЕКТ ОГОНЬ 2018 ===============
// https://gist.github.com/StefanPetrick/819e873492f344ebebac5bcd2fdd8aa8
// https://gist.github.com/StefanPetrick/1ba4584e534ba99ca259c1103754e4c5
// Адаптация от (c) SottNick

// parameters and buffer for the noise array
// (вместо закомментированных строк используются массивы и переменные от эффекта Кометы для экономии памяти)
//define NUM_LAYERS 2 // менять бесполезно, так как в коде чётко использовано 2 слоя
//uint32_t noise32_x[NUM_LAYERSMAX];
//uint32_t noise32_y[NUM_LAYERSMAX];
//uint32_t noise32_z[NUM_LAYERSMAX];
//uint32_t scale32_x[NUM_LAYERSMAX];
//uint32_t scale32_y[NUM_LAYERSMAX];
//uint8_t noise3d[NUM_LAYERSMAX][WIDTH][HEIGHT];
//uint8_t fire18heat[NUM_LEDS]; будем использовать вместо него ledsbuff[NUM_LEDS].r
// this finds the right index within a serpentine matrix

void FieryLedLampEffectFire2018::setup()
{
}

void FieryLedLampEffectFire2018::updateInner()
{
	// some changing values
  	uint16_t ctrl1 = inoise16(11 * millis(), 0, 0);
  	uint16_t ctrl2 = inoise16(13 * millis(), 100000, 100000);
  	uint16_t  ctrl = ((ctrl1 + ctrl2) / 2);

  	// parameters for the heatmap
  	uint16_t speed = 25;
  	noise32_x[0] = 3 * ctrl * speed;
  	noise32_y[0] = 20 * millis() * speed;
  	noise32_z[0] = 5 * millis() * speed ;
  	scale32_x[0] = ctrl1 / 2;
  	scale32_y[0] = ctrl2 / 2;

  	//calculate the noise data
  	uint8_t layer = 0;

  	for (uint8_t i = 0; i < LED_WIDTH; i++) {
    	uint32_t ioffset = scale32_x[layer] * (i - CENTER_X_MAJOR);
    	for (uint8_t j = 0; j < LED_HEIGHT; j++) {
      		uint32_t joffset = scale32_y[layer] * (j - CENTER_Y_MAJOR);
      		uint16_t data = ((inoise16(noise32_x[layer] + ioffset, noise32_y[layer] + joffset, noise32_z[layer])) + 1);
      		noise3d[layer][i][j] = data >> 8;
    	}
  	}

  	// parameters for te brightness mask
  	speed = 20;
  	noise32_x[1] = 3 * ctrl * speed;
  	noise32_y[1] = 20 * millis() * speed;
  	noise32_z[1] = 5 * millis() * speed ;
  	scale32_x[1] = ctrl1 / 2;
  	scale32_y[1] = ctrl2 / 2;

  	//calculate the noise data
  	layer = 1;
  	for (uint8_t i = 0; i < LED_WIDTH; i++) {
    	uint32_t ioffset = scale32_x[layer] * (i - CENTER_X_MAJOR);
    	for (uint8_t j = 0; j < LED_HEIGHT; j++) {
      		uint32_t joffset = scale32_y[layer] * (j - CENTER_Y_MAJOR);
      		uint16_t data = ((inoise16(noise32_x[layer] + ioffset, noise32_y[layer] + joffset, noise32_z[layer])) + 1);
      		noise3d[layer][i][j] = data >> 8;
    	}
  	}

  	// draw lowest line - seed the fire
  	for (uint8_t x = 0; x < LED_WIDTH; x++) {
    	leds[XY(x, LED_HEIGHT - 1)].r =  noise3d[0][LED_WIDTH - 1 - x][CENTER_Y_MAJOR - 1]; // хз, почему взято с середины. вожможно, нужно просто с 7 строки вне зависимости от высоты матрицы
  	}

  	//copy everything one line up
  	for (uint8_t y = 0; y < LED_HEIGHT - 1; y++) {
    	for (uint8_t x = 0; x < LED_WIDTH; x++) {
      		leds[XY(x, y)].r = leds[XY(x, y + 1)].r;
    	}
  	}
	//dim
  	for (uint8_t y = 0; y < LED_HEIGHT - 1; y++) {
    	for (uint8_t x = 0; x < LED_WIDTH; x++) {
      		uint8_t dim = noise3d[0][x][y];
      		// high value = high flames
      		dim = dim / 1.7;
      		dim = 255 - dim;
      		leds[XY(x, y)].r = scale8(leds[XY(x, y)].r , dim);
    	}
  	}

  	for (uint8_t y = 0; y < LED_HEIGHT; y++) {
    	for (uint8_t x = 0; x < LED_WIDTH; x++) {
      		// map the colors based on heatmap
      		leds[XY(x, LED_HEIGHT - 1 - y)] = CRGB( leds[XY(x, y)].r, (float)leds[XY(x, y)].r * scale * 0.01, 0);
      		// dim the result based on 2nd noise layer
      		leds[XY(x, LED_HEIGHT - 1 - y)].nscale8(noise3d[1][x][y]);
    	}
  	}
}

// ============= Огонь 2020 ===============
// (c) SottNick
//сильно по мотивам https://pastebin.com/RG0QGzfK
//Perlin noise fire procedure by Yaroslaw Turbin
//https://www.reddit.com/r/FastLED/comments/hgu16i/my_fire_effect_implementation_based_on_perlin/

#define SPARKLES_NUM  (LED_WIDTH / 8U) // не более чем  enlargedOBJECT_MAX_COUNT (WIDTH * 2)
//float   trackingObjectPosX[SPARKLES_NUM]; // это для искорок. по идее должны быть uint8_t, но были только такие
//float   trackingObjectPosY[SPARKLES_NUM];
//uint8_t shiftHue[HEIGHT];
//uint16_t ff_y, ff_z; используем для сдвига нойза переменные из общих
//uint8_t deltaValue;

void FieryLedLampEffectFire2020::setup()
{
	ff_y=ff_z=0;
	if (scale > 100U) scale = 100U; // чтобы не было проблем при прошивке без очистки памяти
    deltaValue = scale * 0.0899;// /100.0F * ((sizeof(palette_arr) /sizeof(TProgmemRGBPalette16 *))-0.01F));
    if (deltaValue == 3U || deltaValue == 4U)
      	curPalette =  palette_arr[deltaValue]; // (uint8_t)(modes[currentMode].Scale/100.0F * ((sizeof(palette_arr) /sizeof(TProgmemRGBPalette16 *))-0.01F))];
    else
      	curPalette = firePalettes[deltaValue]; // (uint8_t)(modes[currentMode].Scale/100.0F * ((sizeof(firePalettes)/sizeof(TProgmemRGBPalette16 *))-0.01F))];
    deltaValue = (((scale - 1U) % 11U + 1U) << 4U) - 8U; // ширина языков пламени (масштаб шума Перлина)
    deltaHue = map(deltaValue, 8U, 168U, 8U, 84U); // высота языков пламени должна уменьшаться не так быстро, как ширина
    step = map(255U - deltaValue, 87U, 247U, 4U, 32U); // вероятность смещения искорки по оси ИКС
    for (uint8_t j = 0; j < LED_HEIGHT; j++) {
    	shiftHue[j] = (LED_HEIGHT - 1 - j) * 255 / (LED_HEIGHT - 1); // init colorfade table
    }
    for (uint8_t i = 0; i < SPARKLES_NUM; i++) {
      	trackingObjectPosY[i] = random8(LED_HEIGHT);
      	trackingObjectPosX[i] = random8(LED_WIDTH);
    }
}

void FieryLedLampEffectFire2020::updateInner()
{
  	for (uint8_t i = 0; i < LED_WIDTH; i++) {
    	for (uint8_t j = 0; j < LED_HEIGHT; j++) {
      		nblend(leds[XY(i, LED_HEIGHT - 1U - j)], ColorFromPalette(*curPalette, qsub8(inoise8(i * deltaValue, (j + ff_y + random8(2)) * deltaHue, ff_z), shiftHue[j]), 255U), 160U);
    	}
  	}
	//вставляем искорки из отдельного массива
  	for (uint8_t i = 0; i < SPARKLES_NUM; i++) {
    	if (trackingObjectPosY[i] > 3U) {
      		leds[XY(trackingObjectPosX[i], trackingObjectPosY[i])] = leds[XY(trackingObjectPosX[i], 3U)];
      		leds[XY(trackingObjectPosX[i], trackingObjectPosY[i])].fadeToBlackBy( trackingObjectPosY[i] * 2U );
    	}
    	trackingObjectPosY[i]++;
    	if (trackingObjectPosY[i] >= LED_HEIGHT) {
      		trackingObjectPosY[i] = random8(4U);
      		trackingObjectPosX[i] = random8(LED_WIDTH);
    	}
    	if (!random8(step))
      		trackingObjectPosX[i] = (LED_WIDTH + (uint8_t)trackingObjectPosX[i] + 1U - random8(3U)) % LED_WIDTH;
  	}
  	ff_y++;
  	if (ff_y & 0x01)
    	ff_z++;
}

// ============= Эффект Огонь 2021 ===============
// (c) SottNick
// На основе алгоритма https://editor.soulmatelights.com/gallery/546-fire
// by Stepko

#define FIXED_SCALE_FOR_Y 4U // менять нельзя. корректировка скорости ff_x =... подогнана под него

void FieryLedLampEffectFire2021::setup()
{
	if (scale > 100U) scale = 100U; // чтобы не было проблем при прошивке без очистки памяти
    deltaValue = scale * 0.0899;// /100.0F * ((sizeof(palette_arr) /sizeof(TProgmemRGBPalette16 *))-0.01F));
    if (deltaValue == 3U || deltaValue == 4U)
      	curPalette =  palette_arr[deltaValue]; // (uint8_t)(modes[currentMode].Scale/100.0F * ((sizeof(palette_arr) /sizeof(TProgmemRGBPalette16 *))-0.01F))];
    else
      	curPalette = firePalettes[deltaValue]; // (uint8_t)(modes[currentMode].Scale/100.0F * ((sizeof(firePalettes)/sizeof(TProgmemRGBPalette16 *))-0.01F))];
    deltaValue = (scale - 1U) % 11U + 1U;
    if (speed & 0x01) {
    	ff_x = speed;
      	deltaHue2 = FIXED_SCALE_FOR_Y;
    }
    else {
      	if (deltaValue > FIXED_SCALE_FOR_Y)
        	speedfactor = .4 * (deltaValue - FIXED_SCALE_FOR_Y) + FIXED_SCALE_FOR_Y;
      	else
        	speedfactor = deltaValue;
      	ff_x = round(speed * 64. / (0.1686 * speedfactor * speedfactor * speedfactor - 1.162 * speedfactor * speedfactor + 3.6694 * speedfactor + 56.394)); // Ааааа! это тупо подбор коррекции. очень приблизитеьный
      	deltaHue2 = deltaValue;
    }
    if (ff_x > 255U)
    	ff_x = 255U;
    if (ff_x == 0U)
    	ff_x = 1U;
    step = map(ff_x * ff_x, 1U, 65025U, (deltaHue2 - 1U) / 2U + 1U, deltaHue2 * 18U + 44);
    pcnt = map(step, 1U, 255U, 20U, 128U); // nblend 3th param
    deltaValue = 0.7 * deltaValue * deltaValue + 31.3; // ширина языков пламени (масштаб шума Перлина)
    deltaHue2 = 0.7 * deltaHue2 * deltaHue2 + 31.3; // высота языков пламени (масштаб шума Перлина)
}

void FieryLedLampEffectFire2021::updateInner()
{
	ff_y += step;
  	for (byte x = 0; x < LED_WIDTH; x++)
	{
    	for (byte y = 0; y < LED_HEIGHT; y++)
		{
      		int16_t Bri = inoise8(x * deltaValue, (y * deltaHue2) - ff_y, ff_z) - (y * (255 / LED_HEIGHT));
      		byte Col = Bri;//inoise8(x * deltaValue, (y * deltaValue) - ff_y, ff_z) - (y * (255 / HEIGHT));
      		if (Bri < 0)
        		Bri = 0;
      		if (Bri != 0)
        		Bri = 256 - (Bri * 0.2);
      		nblend(leds[XY(x, y)], ColorFromPalette(*curPalette, Col, Bri), pcnt);
    	}
  	}
  	if (!random8())
    	ff_z++;
}

void FieryLedLampEffectFireFly::setup()
{
};
void FieryLedLampEffectFireFly::updateInner()
{
    blurScreen(20); // без размытия как-то пиксельно, по-моему...
    //dimAll(160); // < -- затухание эффекта для последующего кадров
    dimAll(255U - scale * 2);

    for (uint8_t i = 1; i < LED_WIDTH; i += 3) {
        leds[XY( i, CENTER_Y_MINOR)] += CHSV(i * 2 , 255, 255);
    }
	#if 0
    // Noise
    noise32_x[0] += 3000;
    noise32_y[0] += 3000;
    noise32_z[0] += 3000;
    scale32_x[0] = 8000;
    scale32_y[0] = 8000;
    FillNoise(0);
    MoveFractionalNoiseY(3);
    MoveFractionalNoiseX(3);
#endif
};

// ============ Огонь с искрами =============
//               © Stepko
//    updated with Sparks © kostyamat
//             EFF_FIRE_SPARK
//            Fire with Sparks
//---------------------------------------
uint16_t RGBweight(uint16_t idx) {
  return (leds[idx].r + leds[idx].g + leds[idx].b);
}

void Spark::reset()
{
    uint32_t peak = 0;
    speedy = (float)random(5, 30) / 10;
    y = random(LED_HEIGHT / 4, LED_HEIGHT / 2);
    for (uint8_t i = 0; i < LED_WIDTH; i++)
    {
        uint32_t temp = RGBweight(XY(i, y));
        if (temp > peak)
        {
            x = i;
            peak = temp;
        }
    }
    color = leds[XY(x, y)];
}

void FieryLedLampEffectFireSparks::setup()
{
    const byte sparksCount = LED_WIDTH / 4;
    //FPSdelay = DYNAMIC;
    for (byte i = 0; i < sparksCount; i++)
        sparks[i].reset();
    t=0;
};
void FieryLedLampEffectFireSparks::updateInner()
{
    const byte sparksCount = LED_WIDTH / 4;
    const uint8_t spacer = LED_HEIGHT / 4;
    bool withSparks = scale >= 50;
    t += speed;

    if (withSparks)
    {
        for (byte i = 0; i < sparksCount; i++)
        {
            sparks[i].addXY((float)random(-1, 2) / 2, 0.75);
            if(sparks[i].getY() > LED_HEIGHT and !random(0, 50))
                sparks[i].reset();
            else
                sparks[i].draw(this);
        }
    }

    for (byte x = 0; x < LED_WIDTH; x++)
    {
        for (byte y = 0; y < LED_HEIGHT; y++)
        {
            int16_t Bri = inoise8(x * scale, (y * scale) - t) - ((withSparks ? y + spacer : y) * (255 / LED_HEIGHT));
            byte Col = Bri;
            if (Bri < 0)
                Bri = 0;
            if (Bri != 0)
                Bri = 256 - (Bri * 0.2);
            nblend(leds[XY(x, y)], ColorFromPalette(HeatColors_p, Col, Bri), speed);
        }
    }
};

void FieryLedLampEffectPlasma::setup()
{
};
void FieryLedLampEffectPlasma::updateInner()
{
	fillNoiseLED(&PartyColors_p);
};

// ============= Эффект Плазменная лампа ===============
// эффект Паук (c) stepko
// плюс выбор палитры и багфикс (c) SottNick

float fmap(const float x, const float in_min, const float in_max, const float out_min, const float out_max);

void FieryLedLampEffectSpider::setup()
{
	setCurrentPalette();
	pcnt = (scale - 1U) % 11U + 1U; // количество линий от 1 до 11 для каждой из 9 палитр
	speedfactor = fmap(speed, 1, 255, 20., 2.); 
};
void FieryLedLampEffectSpider::updateInner()
{
	//if (hue2++ & 0x01 && deltaHue++ & 0x01 && deltaHue2++ & 0x01)
	//	hue++; // хз. как с 60ю кадрами в секунду скорость замедлять...
	dimAll(205);
	float time_shift = millis() & 0x7FFFFF; // overflow protection proper by SottNick
	time_shift /= speedfactor;
	for (uint8_t c = 0; c < pcnt; c++)
	{
		float xx = 2. + sin8(time_shift + 6000 * c) / 12.;
		float yy = 2. + cos8(time_shift + 9000 * c) / 12.;
		if(xx < 0.f)
			DBG_PRINT("%f %f\n", xx, time_shift);
		DrawLineF(xx, yy, (float)LED_WIDTH - xx - 1, (float)LED_HEIGHT - yy - 1, ColorFromPalette(*curPalette, hue + c * (255 / pcnt)));
	}
}

// ============ Plasma Waves ============
//              © Stepko
//        Adaptation © alvikskor
//             Плазменные Волны
// --------------------------------------

void FieryLedLampEffectPlasmaWaves::setup()
{
    hue = scale / 10;
	frameCount = 0;
};

extern uint8_t exp_gamma[];
void FieryLedLampEffectPlasmaWaves::updateInner()
{
	#if 1
	FPSdelay = 1; // 64 - modes[currentMode].Speed / 4;

	frameCount++;
	uint8_t t1 = cos8((42 * frameCount) / (132 - speed / 2));
	uint8_t t2 = cos8((35 * frameCount) / (132 - speed / 2));
	uint8_t t3 = cos8((38 * frameCount) / (132 - speed / 2));

	for (uint16_t y = 0; y < LED_HEIGHT; y++)
	{
		for (uint16_t x = 0; x < LED_WIDTH; x++)
		{
			// Calculate 3 seperate plasma waves, one for each color channel
			uint8_t r = cos8((x << 3) + (t1 >> 1) + cos8(t2 + (y << 3) + scale));
			uint8_t g = cos8((y << 3) + t1 + cos8((t3 >> 2) + (x << 3)) + scale);
			uint8_t b = cos8((y << 3) + t2 + cos8(t1 + x + (g >> 2) + scale));

			// uncomment the following to enable gamma correction
			// r = pgm_read_byte_near(exp_gamma + r);
			switch (hue)
			{
			case 0:
				r = pgm_read_byte(&exp_gamma[r]);
				g = pgm_read_byte(&exp_gamma[g]);
				b = pgm_read_byte(&exp_gamma[b]);
				break;
			case 1:
				r = pgm_read_byte(&exp_gamma[r]);
				b = pgm_read_byte(&exp_gamma[g]);
				g = pgm_read_byte(&exp_gamma[b]);
				break;
			case 2:
				g = pgm_read_byte(&exp_gamma[r]);
				r = pgm_read_byte(&exp_gamma[g]);
				b = pgm_read_byte(&exp_gamma[b]);
				break;
			case 3:
				r = pgm_read_byte(&exp_gamma[r]) / 2;
				g = pgm_read_byte(&exp_gamma[g]);
				b = pgm_read_byte(&exp_gamma[b]);
				break;
			case 4:
				r = pgm_read_byte(&exp_gamma[r]);
				g = pgm_read_byte(&exp_gamma[g]) / 2;
				b = pgm_read_byte(&exp_gamma[b]);
				break;
			case 5:
				r = pgm_read_byte(&exp_gamma[r]);
				g = pgm_read_byte(&exp_gamma[g]);
				b = pgm_read_byte(&exp_gamma[b]) / 2;
				break;
			case 6:
				r = pgm_read_byte(&exp_gamma[r]) * 3;
				g = pgm_read_byte(&exp_gamma[g]);
				b = pgm_read_byte(&exp_gamma[b]);
				break;
			case 7:
				r = pgm_read_byte(&exp_gamma[r]);
				g = pgm_read_byte(&exp_gamma[g]) * 3;
				b = pgm_read_byte(&exp_gamma[b]);
				break;
			case 8:
				r = pgm_read_byte(&exp_gamma[r]);
				g = pgm_read_byte(&exp_gamma[g]);
				b = pgm_read_byte(&exp_gamma[b]) * 3;
				break;
			}
			// g = pgm_read_byte_near(exp_gamma + g);
			// b = pgm_read_byte_near(exp_gamma + b);

			leds[XY(x, y)] = CRGB(r, g, b);
		}
		// hue++;
  }
  // blurScreen(beatsin8(3, 64, 80));
  #endif
}

// ============= Эффект Пламя ===============
// (c) SottNick
// По мотивам https://goldenandy.blogspot.com/2021/05/ws2812.html
// by Андрей Локтев

// характеристики языков пламени
//  x, dx; => trackingObjectPosX, trackingObjectSpeedX;
//  y, dy; => trackingObjectPosY, trackingObjectSpeedY;
//  ttl; => trackingObjectState;
//  uint8_t hue; => float   trackingObjectShift
//  uint8_t saturation; => 255U
//  uint8_t value; => trackingObjectHue;

// характеристики изображения CHSV picture[WIDTH][HEIGHT]
//  uint8_t .hue; => noise3d[0][WIDTH][HEIGHT]
//  uint8_t .sat; => shiftValue[HEIGHT] (не хватило двухмерного массива на насыщенность)
//  uint8_t .val; => noise3d[1][WIDTH][HEIGHT]

#define FLAME_MAX_DY        256 // максимальная вертикальная скорость перемещения языков пламени за кадр.  имеется в виду 256/256 =   1 пиксель за кадр
#define FLAME_MIN_DY        128 // минимальная вертикальная скорость перемещения языков пламени за кадр.   имеется в виду 128/256 = 0.5 пикселя за кадр
#define FLAME_MAX_DX         32 // максимальная горизонтальная скорость перемещения языков пламени за кадр. имеется в виду 32/256 = 0.125 пикселя за кадр
#define FLAME_MIN_DX       (-FLAME_MAX_DX)
#define FLAME_MAX_VALUE     255 // максимальная начальная яркость языка пламени
#define FLAME_MIN_VALUE     176 // минимальная начальная яркость языка пламени

//пришлось изобрести очередную функцию субпиксельной графики. на этот раз бесшовная по ИКСу, работающая в цветовом пространстве HSV и без смешивания цветов
void FieryLedLampEffectFlame::wu_pixel_maxV(uint8_t item)
{
	// uint8_t xx = trackingObjectPosX[item] & 0xff, yy = trackingObjectPosY[item] & 0xff, ix = 255 - xx, iy = 255 - yy;
	uint8_t xx = (trackingObjectPosX[item] - (int)trackingObjectPosX[item]) * 255, yy = (trackingObjectPosY[item] - (int)trackingObjectPosY[item]) * 255, ix = 255 - xx, iy = 255 - yy;
	// calculate the intensities for each affected pixel
#define WU_WEIGHT(a,b) ((uint8_t) (((a)*(b)+(a)+(b))>>8))
	uint8_t wu[4] = {WU_WEIGHT(ix, iy), WU_WEIGHT(xx, iy),
					 WU_WEIGHT(ix, yy), WU_WEIGHT(xx, yy)};
	// multiply the intensities by the colour, and saturating-add them to the pixels
	for (uint8_t i = 0; i < 4; i++)
	{
		uint8_t x1 = (int8_t)(trackingObjectPosX[item] + (i & 1)) % LED_WIDTH; // делаем бесшовный по ИКСу
		uint8_t y1 = (int8_t)(trackingObjectPosY[item] + ((i >> 1) & 1));
		if (y1 < LED_HEIGHT && trackingObjectHue[item] * wu[i] >> 8 >= noise3d[1][x1][y1])
		{
			noise3d[0][x1][y1] = trackingObjectShift[item];
			shiftValue[y1] = 255U; // saturation;
			noise3d[1][x1][y1] = trackingObjectHue[item] * wu[i] >> 8;
		}
  }
}

uint8_t myScale8(uint8_t x);

void FieryLedLampEffectFlame::setup()
{
    enlargedObjectNUM = (speed - 1U) / 254.0 * (trackingOBJECT_MAX_COUNT - 1U) + 1U;
    if (enlargedObjectNUM > enlargedOBJECT_MAX_COUNT)
		enlargedObjectNUM = enlargedOBJECT_MAX_COUNT;
	uint8_t ff_x = LED_WIDTH * 2.4;
	enlargedObjectNUM = (ff_x > enlargedOBJECT_MAX_COUNT) ? enlargedOBJECT_MAX_COUNT : ff_x;
    

    hue = map8(myScale8(scale + 3U), 3, 10); // минимальная живучесть/высота языка пламени ...ttl
    hue2 = map8(myScale8(scale + 3U), 6, 31); // максимальная живучесть/высота языка пламени ...ttl
	for (uint8_t i = 0; i < trackingOBJECT_MAX_COUNT; i++) // чистим массив объектов от того, что не похоже на языки пламени
		if (trackingObjectState[i] > 30U || trackingObjectPosY[i] >= LED_HEIGHT || trackingObjectPosX[i] >= LED_WIDTH || trackingObjectPosY[i] <= 0)
		{
			trackingObjectHue[i] = 0U;
			trackingObjectState[i] = random8(20);
		}
	for (uint8_t i = 0; i < LED_WIDTH; i++) // заполняем массив изображения из массива leds обратным преобразованием, которое нихрена не работает
		for (uint8_t j = 0; j < LED_HEIGHT; j++)
		{
			CHSV tHSV = rgb2hsv_approximate(leds[XY(i, j)]);
			noise3d[0][i][j] = tHSV.hue;
			if (tHSV.val > 100U)
			{ // такая защита от пересвета более-менее достаточна
				shiftValue[j] = tHSV.sat;
				if (tHSV.sat < 100U) // для перехода с очень тусклых эффектов, использующих заливку белым или почти белым светом
					noise3d[1][i][j] = tHSV.val / 3U;
				else
					noise3d[1][i][j] = tHSV.val - 32U;
			}
			else
				noise3d[1][i][j] = 0U;

			// CRGB tRGB = leds[XY(i,j)];
			// if (tRGB.r + tRGB.g + tRGB.b < 100U) // не пригодилось
			//   noise3d[1][i][j] = 0U;
		}
};
void FieryLedLampEffectFlame::updateInner()
{
	int16_t i, j;

	// угасание предыдущего кадра
	for (i = 0; i < LED_WIDTH; i++)
		for (j = 0; j < LED_HEIGHT; j++)
			noise3d[1][i][j] = (uint16_t)noise3d[1][i][j] * 237U >> 8;

	// цикл перебора языков пламени
	for (i = 0; i < enlargedObjectNUM; i++)
	{
		if (trackingObjectState[i])
		{ // если ещё не закончилась его жизнь
			wu_pixel_maxV(i);

			j = trackingObjectState[i];
			trackingObjectState[i]--;

			trackingObjectPosX[i] += trackingObjectSpeedX[i];
			trackingObjectPosY[i] += trackingObjectSpeedY[i];

			trackingObjectHue[i] = (trackingObjectState[i] * trackingObjectHue[i] + j / 2) / j;

			// если вышел за верхнюю границу или потух, то и жизнь закончилась
			if (trackingObjectPosY[i] >= LED_HEIGHT || trackingObjectHue[i] < 2U)
				trackingObjectState[i] = 0;

			// если вылез за край матрицы по горизонтали, перекинем на другую сторону
			if (trackingObjectPosX[i] < 0)
				trackingObjectPosX[i] += LED_WIDTH;
			else if (trackingObjectPosX[i] >= LED_WIDTH)
				trackingObjectPosX[i] -= LED_WIDTH;
		}
		else
		{ // если жизнь закончилась, перезапускаем
			trackingObjectState[i] = random8(hue, hue2);
			trackingObjectShift[i] = (uint8_t)(254U + scale + random8(20U)); // 254 - это шаг в обратную сторону от выбранного пользователем оттенка (стартовый оттенок диапазона)
			// 20 - это диапазон из градиента цвета от выбранного пользователем оттенка (диапазон от 254 до 254+20)
			trackingObjectPosX[i] = (float)random(LED_WIDTH * 255U) / 255.;
			trackingObjectPosY[i] = -.9;
			trackingObjectSpeedX[i] = (float)(FLAME_MIN_DX + random8(FLAME_MAX_DX - FLAME_MIN_DX)) / 256.;
			trackingObjectSpeedY[i] = (float)(FLAME_MIN_DY + random8(FLAME_MAX_DY - FLAME_MIN_DY)) / 256.;
			trackingObjectHue[i] = FLAME_MIN_VALUE + random8(FLAME_MAX_VALUE - FLAME_MIN_VALUE + 1U);
			// saturation = 255U;
		}
	}

	// выводим кадр на матрицу
	for (i = 0; i < LED_WIDTH; i++)
		for (j = 0; j < LED_HEIGHT; j++)
			// hsv2rgb_spectrum(CHSV(noise3d[0][i][j], shiftValue[j], noise3d[1][i][j] * 1.033), leds[XY(i,j)]); // 1.033 - это коэффициент нормализации яркости (чтобы чутка увеличить яркость эффекта в целом)
			hsv2rgb_spectrum(CHSV(noise3d[0][i][j], shiftValue[j], noise3d[1][i][j]), leds[XY(i, j)]);
}

// =========== FeatherCandle ============
//         адаптация © SottNick
//    github.com/mnemocron/FeatherCandle
//      modify & design © SlingMaster
//           EFF_FEATHER_CANDLE
//                Свеча
//---------------------------------------
#include "data7x15flip.h"                       // FeatherCandle animation data
const uint8_t  w    = 7;                        // image width
const uint8_t  h    = 15;                       // image height
uint8_t        img[w * h];                      // Buffer for rendering image
uint8_t        deltaX = CENTER_X_MAJOR - 3;     // position img
uint8_t last_brightness;

void FieryLedLampEffectFeatherCandle::setup()
{
    hue = 0;
    trackingObjectState[0] = 110;
    trackingObjectState[1] = 110;
    trackingObjectState[2] = 110;
    trackingObjectState[4] = CENTER_X_MAJOR;
};
void FieryLedLampEffectFeatherCandle::updateInner()
{
	const uint8_t  level = 160;
	const uint8_t  low_level = 110;
	const uint8_t *ptr  = anim;                     // Current pointer into animation data

	uint8_t a = pgm_read_byte(ptr++); // New frame X1/Y1
	if (a >= 0x90)
	{							  // EOD marker? (valid X1 never exceeds 8)
		ptr = anim;				  // Reset animation data pointer to start
		a = pgm_read_byte(ptr++); // and take first value
	}
	uint8_t x1 = a >> 4;	  // X1 = high 4 bits
	uint8_t y1 = a & 0x0F;	  // Y1 = low 4 bits
	a = pgm_read_byte(ptr++); // New frame X2/Y2
	uint8_t x2 = a >> 4;	  // X2 = high 4 bits
	uint8_t y2 = a & 0x0F;	  // Y2 = low 4 bits

	// Read rectangle of data from anim[] into portion of img[] buffer
	for (uint8_t y = y1; y <= y2; y++)
	{
		for (uint8_t x = x1; x <= x2; x++)
		{
			img[y * w + x] = pgm_read_byte(ptr++);
		}
	}
	int i = 0;
	uint8_t color = scale;

	// draw flame -------------------
	for (uint8_t y = 1; y < h; y++)
	{
		if ((LED_HEIGHT < 15) || (LED_WIDTH < 9))
		{
			// for small matrix -----
			if (y % 2 == 0)
			{
				leds[XY(CENTER_X_MAJOR - 1, 7)] = CHSV(color, 255U, 55 + random8(200));
				leds[XY(CENTER_X_MAJOR, 6)] = CHSV(color, 255U, 160 + random8(90));
				leds[XY(CENTER_X_MAJOR + 1, 6)] = CHSV(color, 255U, 205 + random8(50));
				leds[XY(CENTER_X_MAJOR - 1, 5)] = CHSV(color, 255U, 155 + random8(100));
				leds[XY(CENTER_X_MAJOR, 5)] = CHSV(color - 10U, 255U, 120 + random8(130));
				leds[XY(CENTER_X_MAJOR, 4)] = CHSV(color - 10U, 255U, 100 + random8(120));
				DrawLine(0, 2U, LED_WIDTH - 1, 2U, 0x000000);
			}
		}
		else
		{
			for (uint8_t x = 0; x < w; x++)
			{
				uint8_t brightness = img[i];
				leds[XY(deltaX + x, y)] = CHSV(brightness > 240 ? color : color - 10U, 255U, brightness);
				i++;
			}
		}

		// draw body FeatherCandle ------
		if (y <= 3)
		{
			if (y % 2 == 0)
			{
				gradientVertical(0, 0, LED_WIDTH, 2, color, color, 48, 128, 20U);
			}
		}

		// drops of wax move -------------
		switch (hue)
		{
		case 0:
			if (trackingObjectState[0] < level)
			{
				trackingObjectState[0]++;
			}
			break;
		case 1:
			if (trackingObjectState[0] > low_level)
			{
				trackingObjectState[0]--;
			}
			if (trackingObjectState[1] < level)
			{
				trackingObjectState[1]++;
			}
			break;
		case 2:
			if (trackingObjectState[1] > low_level)
			{
				trackingObjectState[1]--;
			}
			if (trackingObjectState[2] < level)
			{
				trackingObjectState[2]++;
			}
			break;
		case 3:
			if (trackingObjectState[2] > low_level)
			{
				trackingObjectState[2]--;
			}
			else
			{
				hue++;
				// set random position drop of wax
				trackingObjectState[4] = CENTER_X_MAJOR - 3 + random8(6);
			}
			break;
		}

		if (hue > 3)
		{
			hue++;
		}
		else
		{
			if (hue < 2)
			{
				leds[XY(trackingObjectState[4], 2)] = CHSV(50U, 20U, trackingObjectState[0]);
			}
			if ((hue == 1) || (hue == 2))
			{
				leds[XY(trackingObjectState[4], 1)] = CHSV(50U, 15U, trackingObjectState[1]); // - 10;
			}
			if (hue > 1)
			{
				leds[XY(trackingObjectState[4], 0)] = CHSV(50U, 5U, trackingObjectState[2]); // - 20;
			}
		}
	}

	// next -----------------
	if ((trackingObjectState[0] == level) || (trackingObjectState[1] == level) || (trackingObjectState[2] == level))
	{
		hue++;
	}
}