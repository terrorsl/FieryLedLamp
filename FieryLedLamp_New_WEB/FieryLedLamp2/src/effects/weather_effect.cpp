#include"effect.h"
#include"effects/weather_effect.h"

// ============= ЭФФЕКТ ВИХРИ ===============
// https://github.com/pixelmatix/aurora/blob/master/PatternFlowField.h
// Адаптация (c) SottNick
// используются переменные эффекта Стая. Без него работать не будет.

static const uint8_t ff_speed = 1; // чем выше этот параметр, тем короче переходы (градиенты) между цветами. 1 - это самое красивое
static const uint8_t ff_scale = 26; // чем больше этот параметр, тем больше "языков пламени" или как-то так. 26 - это норм

void FieryLedLampEffectWhirl::setup()
{
	setCurrentPalette();

    ff_x = random16();
    ff_y = random16();
    ff_z = random16();

    for (uint8_t i = 0; i < AVAILABLE_BOID_COUNT; i++)
    {
        boids[i] = Boid(random8(LED_WIDTH), 0);
    }
}
void FieryLedLampEffectWhirl::updateInner()
{
	dimAll(240);

    for (uint8_t i = 0; i < AVAILABLE_BOID_COUNT; i++)
    {
        Boid * boid = &boids[i];

    	int ioffset = ff_scale * boid->location.x;
    	int joffset = ff_scale * boid->location.y;

    	byte angle = inoise8(ff_x + ioffset, ff_y + joffset, ff_z);

    	boid->velocity.x = (float) sin8(angle) * 0.0078125 - 1.0;
    	boid->velocity.y = -((float)cos8(angle) * 0.0078125 - 1.0);
    	boid->update();

    	if (oneColor)	
      		drawPixelXYF(boid->location.x, boid->location.y, CHSV(scale * 2.55, (scale == 100) ? 0U : 255U, 255U)); // цвет белый для .Scale=100
    	else
      		drawPixelXYF(boid->location.x, boid->location.y, ColorFromPalette(*curPalette, angle + hue)); // + hue постепенно сдвигает палитру по кругу

        if (boid->location.x < 0 || boid->location.x >= LED_WIDTH || boid->location.y < 0 || boid->location.y >= LED_HEIGHT)
        {
            boid->location.x = random(LED_WIDTH);
      		boid->location.y = 0;
        }
    }

    EVERY_N_MILLIS(200) {
    	hue++;
  	}

  	ff_x += ff_speed;
  	ff_y += ff_speed;
  	ff_z += ff_speed;
}

void rain(uint32_t **noise3d, uint8_t *line, byte backgroundDepth, byte maxBrightness, byte spawnFreq, byte tailLength, CRGB rainColor, bool splashes, bool clouds, bool storm)
{
    uint16_t ff_x = random16();
    uint16_t ff_y = random16();
    uint16_t ff_z = random16();

    CRGB lightningColor = CRGB(72, 72, 80);
    CRGBPalette16 rain_p( CRGB::Black, rainColor );

    CRGBPalette16 rainClouds_p( CRGB::Black, CRGB(15, 24, 24), CRGB(9, 15, 15), CRGB::Black );
    dimAll(tailLength);

    // Loop for each column individually
    for (uint8_t x = 0; x < LED_WIDTH; x++)
    {
        // Step 1.  Move each dot down one cell
        for (uint8_t i = 0; i < LED_HEIGHT; i++)
        {
            if (noise3d[x][i] >= backgroundDepth) {  // Don't move empty cells
                if (i > 0)
                    noise3d[x][i - 1] = noise3d[x][i];
                noise3d[x][i] = 0;
            }
        }

        // Step 2.  Randomly spawn new dots at top
        if (random8() < spawnFreq) {
            noise3d[x][LED_HEIGHT - 1] = random(backgroundDepth, maxBrightness);
        }

        // Step 3. Map from tempMatrix cells to LED colors
        for (uint8_t y = 0; y < LED_HEIGHT; y++) {
            if (noise3d[x][y] >= backgroundDepth) {  // Don't write out empty cells
                leds[XY(x, y)] = ColorFromPalette(rain_p, noise3d[x][y]);
            }
        }
        // Step 4. Add splash if called for
        if (splashes) {
            byte j = line[x];
            byte v = noise3d[x][0];

            if(x>=2 && x<LED_WIDTH-2)
            {
                if (j >= backgroundDepth)
                {
                    leds[XY(x - 2, 0)] = ColorFromPalette(rain_p, j / 3);
                    leds[XY(x + 2, 0)] = ColorFromPalette(rain_p, j / 3);
                    line[x] = 0; // Reset splash
                }
            }

            if (x >= 1 && x < LED_WIDTH - 1)
            {
                if (v >= backgroundDepth)
                {
                    leds[XY(x - 1, 1)] = ColorFromPalette(rain_p, v / 2);
                    leds[XY(x + 1, 1)] = ColorFromPalette(rain_p, v / 2);
                    line[x] = v; // Prep splash for next frame
                }
            }
        }
#if 0
        // Step 5. Add lightning if called for
        if (storm)
        {
            // uint8_t lightning[WIDTH][HEIGHT];
            //  ESP32 does not like static arrays  https://github.com/espressif/arduino-esp32/issues/2567
            uint8_t *lightning = (uint8_t *)malloc(LED_WIDTH * LED_HEIGHT);

            if (random16() < 72)
            {                                                                                     // Odds of a lightning bolt
                lightning[scale8(random8(), LED_WIDTH - 1) + (LED_HEIGHT - 1) * LED_WIDTH] = 255; // Random starting location
                for (uint8_t ly = LED_HEIGHT - 1; ly > 1; ly--)
                {
                    for (uint8_t lx = 1; lx < LED_WIDTH - 1; lx++)
                    {
                        if (lightning[lx + ly * LED_WIDTH] == 255)
                        {
                            lightning[lx + ly * LED_WIDTH] = 0;
                            uint8_t dir = random8(4);
                            switch (dir)
                            {
                            case 0:
                                leds[XY(lx + 1, ly - 1)] = lightningColor;
                                lightning[(lx + 1) + (ly - 1) * LED_WIDTH] = 255; // move down and right
                                break;
                            case 1:
                                leds[XY(lx, ly - 1)] = CRGB(128, 128, 128); // я без понятия, почему у верхней молнии один оттенок, а у остальных - другой
                                lightning[lx + (ly - 1) * LED_WIDTH] = 255;     // move down
                                break;
                            case 2:
                                leds[XY(lx - 1, ly - 1)] = CRGB(128, 128, 128);
                                lightning[(lx - 1) + (ly - 1) * LED_WIDTH] = 255; // move down and left
                                break;
                            case 3:
                                leds[XY(lx - 1, ly - 1)] = CRGB(128, 128, 128);
                                lightning[(lx - 1) + (ly - 1) * LED_WIDTH] = 255; // fork down and left
                                leds[XY(lx - 1, ly - 1)] = CRGB(128, 128, 128);
                                lightning[(lx + 1) + (ly - 1) * LED_WIDTH] = 255; // fork down and right
                                break;
                            }
                        }
                    }
                }
            }
            free(lightning);
        }

        // Step 6. Add clouds if called for
        if (clouds)
        {
            uint16_t noiseScale = 250;  // A value of 1 will be so zoomed in, you'll mostly see solid colors. A value of 4011 will be very zoomed out and shimmery
            const uint8_t cloudHeight = LED_HEIGHT * 0.4 + 1; // это уже 40% c лишеним, но на высоких матрицах будет чуть меньше

            // This is the array that we keep our computed noise values in
            uint8_t *noise = (uint8_t *) malloc(LED_WIDTH * cloudHeight);
            //int xoffset = noiseScale * x + hue;
            int xoffset = noiseScale * x;

            for (uint8_t z = 0; z < cloudHeight; z++)
            {
                //int yoffset = noiseScale * z - hue;
                int yoffset = noiseScale * z;
                uint8_t dataSmoothing = 192;
                uint8_t noiseData = qsub8(inoise8(ff_x + xoffset, ff_y + yoffset, ff_z), 16);
                noiseData = qadd8(noiseData, scale8(noiseData, 39));
                noise[x * cloudHeight + z] = scale8( noise[x * cloudHeight + z], dataSmoothing) + scale8( noiseData, 256 - dataSmoothing);
                nblend(leds[XY(x, LED_HEIGHT - z - 1)], ColorFromPalette(rainClouds_p, noise[x * cloudHeight + z]), (cloudHeight - z) * (250 / cloudHeight));
            }
            ff_z ++;
            free(noise);
        }
#endif
    }
}
uint8_t myScale8(uint8_t x) { // даёт масштабировать каждые 8 градаций (от 0 до 7) бегунка Масштаб в значения от 0 до 255 по типа синусоиде
    uint8_t x8 = x % 8U;
    uint8_t x4 = x8 % 4U;
    if (x4 == 0U)
        if (x8 == 0U)
            return 0U;
        else
            return 255U;
    else
        if (x8 < 4U)
            return (1U   + x4 * 72U); // всего 7шт по 36U + 3U лишних = 255U (чтобы восхождение по синусоиде не было зеркально спуску)
    return (253U - x4 * 72U); // 253U = 255U - 2U
}

void FieryLedLampEffectColorRain::setup()
{
    memset(line, 0, sizeof(line));
};
void FieryLedLampEffectColorRain::updateInner()
{
   /* CRGB solidRainColor = CRGB(60, 80, 90);
    // я хз, как прикрутить а 1 регулятор и длину хвостов и цвет капель
    // ( Depth of dots, maximum brightness, frequency of new dots, length of tails, color, splashes, clouds, ligthening )
    //rain(60, 200, map8(intensity,5,100), 195, CRGB::Green, false, false, false); // было CRGB::Green
    if (scale > 247U)
        rain((uint32_t**)cell, line, 60, 200, map8(42, 5, 100), myScale8(scale), solidRainColor, false, false, false);
    else
        rain((uint32_t**)cell, line, 60, 200, map8(42, 5, 100), myScale8(scale), CHSV(scale, 255U, 255U), false, false, false);
        */
};

void FieryLedLampEffectClouds::setup()
{
    curPalette = &CloudColors_p;
    colorLoop = 0;
};
void FieryLedLampEffectClouds::updateInner()
{
    fillNoiseLED(curPalette);
};

void FieryLedLampEffectOcean::setup()
{
    curPalette = &OceanColors_p;
    colorLoop = 0;
};
void FieryLedLampEffectOcean::updateInner()
{
    fillNoiseLED(curPalette);
}

void FieryLedLampEffectRainbowStripe::setup()
{
    curPalette = &RainbowStripeColors_p;
    colorLoop = 1;
};
void FieryLedLampEffectRainbowStripe::updateInner()
{
    fillNoiseLED(curPalette);
}

void FieryLedLampEffectStormyRain::setup()
{

};
void FieryLedLampEffectStormyRain::updateInner()
{
    CRGB solidRainColor = CRGB(60, 80, 90);
    // ( Depth of dots, maximum brightness, frequency of new dots, length of tails, color, splashes, clouds, ligthening )
    rain(dots, line, 60, 160, scale, 30, solidRainColor, true, true, true);
};

void FieryLedLampEffectSimpleRain::setup()
{

};
void FieryLedLampEffectSimpleRain::updateInner()
{
    CRGB solidRainColor = CRGB(60, 80, 90);
    // ( Depth of dots, maximum brightness, frequency of new dots, length of tails, color, splashes, clouds, ligthening )
    // rain(60, 200, map8(intensity,2,60), 10, solidRainColor, true, true, false);
    rain(dots, line, 60, 180, scale, 30, solidRainColor, true, true, false);
}

#if 0
// --------- Эффект "Северное Сияние"
// (c) kostyamat 05.02.2021
// идеи подсмотрены тут https://www.reddit.com/r/FastLED/comments/jyly1e/challenge_fastled_sketch_that_fits_entirely_in_a/
// особая благодарность https://www.reddit.com/user/ldirko/ Yaroslaw Turbin aka ldirko

// вместо набора палитр в оригинальном эффекте сделан генератор палитр
#define AURORA_COLOR_RANGE 10 // (+/-10 единиц оттенка) диапазон, в котором плавает цвет сияния относительно выбранного оттенка 
#define AURORA_COLOR_PERIOD 2 // (2 раза в минуту) частота, с которой происходит колебание выбранного оттенка в разрешённом диапазоне

// генератор палитр для Северного сияния (c) SottNick
static const uint8_t MBAuroraColors_arr[5][4] PROGMEM = // палитра в формате CHSV
{ //№, цвет, насыщенность, яркость
  {0  , 0 , 255,   0},// black
  {80 , 0 , 255, 255},
  {130, 25, 220, 255},
  {180, 25, 185, 255},
  {255, 25, 155, 255} //245
};
/*
  {
  {0  , 0 , 255,   0},// black
  {60 , 1 , 255, 222},
  {80 , 1 , 210, 255},
  {180, 11, 175, 255},
  {255, 11 ,135, 255} //245
  };
*/
//CRGBPalette16 myPal; уже есть эта переменная в эффекте Жидкая лампа

void fillMyPal16_2(uint8_t hue, bool isInvert = false) {
  // я бы, конечно, вместо копии функции генерации палитры "_2"
  // лучше бы сделал её параметром указатель на массив с базовой палитрой,
  // но я пониятия не имею, как это делается с грёбаным PROGMEM

  int8_t lastSlotUsed = -1;
  uint8_t istart8, iend8;
  CRGB rgbstart, rgbend;

  // начинаем с нуля
  if (isInvert)
    //с неявным преобразованием оттенков цвета получаются, как в фотошопе, но для данного эффекта не красиво выглядят
    //rgbstart = CHSV(256 + hue - pgm_read_byte(&MBAuroraColors_arr[0][1]), pgm_read_byte(&MBAuroraColors_arr[0][2]), pgm_read_byte(&MBAuroraColors_arr[0][3])); // начальная строчка палитры с инверсией
    hsv2rgb_spectrum(CHSV(256 + hue - pgm_read_byte(&MBAuroraColors_arr[0][1]), pgm_read_byte(&MBAuroraColors_arr[0][2]), pgm_read_byte(&MBAuroraColors_arr[0][3])), rgbstart);
  else
    //rgbstart = CHSV(hue + pgm_read_byte(&MBAuroraColors_arr[0][1]), pgm_read_byte(&MBAuroraColors_arr[0][2]), pgm_read_byte(&MBAuroraColors_arr[0][3])); // начальная строчка палитры
    hsv2rgb_spectrum(CHSV(hue + pgm_read_byte(&MBAuroraColors_arr[0][1]), pgm_read_byte(&MBAuroraColors_arr[0][2]), pgm_read_byte(&MBAuroraColors_arr[0][3])), rgbstart);
  int indexstart = 0; // начальный индекс палитры
  for (uint8_t i = 1U; i < 5U; i++) { // в палитре @obliterator всего 5 строчек
    int indexend = pgm_read_byte(&MBAuroraColors_arr[i][0]);
    if (isInvert)
      hsv2rgb_spectrum(CHSV(hue + pgm_read_byte(&MBAuroraColors_arr[i][1]), pgm_read_byte(&MBAuroraColors_arr[i][2]), pgm_read_byte(&MBAuroraColors_arr[i][3])), rgbend);
    else
      hsv2rgb_spectrum(CHSV(256 + hue - pgm_read_byte(&MBAuroraColors_arr[i][1]), pgm_read_byte(&MBAuroraColors_arr[i][2]), pgm_read_byte(&MBAuroraColors_arr[i][3])), rgbend);
    istart8 = indexstart / 16;
    iend8   = indexend   / 16;
    if ((istart8 <= lastSlotUsed) && (lastSlotUsed < 15)) {
      istart8 = lastSlotUsed + 1;
      if (iend8 < istart8)
        iend8 = istart8;
    }
    lastSlotUsed = iend8;
    fill_gradient_RGB( myPal, istart8, rgbstart, iend8, rgbend);
    indexstart = indexend;
    rgbstart = rgbend;
  }
}

//float adjastHeight; // используем emitterX
//uint16_t adjScale; // используем ff_y

void FieryLedLampEffectNorthernLights::setup()
{
    emitterX = 400. / LED_HEIGHT; // а это - максимум без яркой засветки крайних рядов матрицы (сверху и снизу)

    ff_y = map(LED_WIDTH, 8, 64, 310, 63);
    ff_z = ff_y;
    speedfactor = map(speed, 1, 255, 128, 16); // _speed = map(speed, 1, 255, 128, 16);
};
void FieryLedLampEffectNorthernLights::updateInner()
{
    if (scale == 100)
    {
        //if (hue2++ & 0x01 && deltaHue++ & 0x01 && deltaHue2++ & 0x01)
        //    hue++; // это ж бред, но я хз. как с 60ю кадрами в секунду можно эффективно скорость замедлять...
        fillMyPal16_2(scale + hue, scale & 0x01);
    }
    else
        fillMyPal16_2(scale + AURORA_COLOR_RANGE - beatsin8(AURORA_COLOR_PERIOD, 0U, AURORA_COLOR_RANGE + AURORA_COLOR_RANGE), scale & 0x01);

    for (byte x = 0; x < LED_WIDTH; x++)
    {
        for (byte y = 0; y < LED_HEIGHT; y++)
        {
            polarTimer++;
            leds[XY(x, y)] =
                ColorFromPalette(myPal,
                                 qsub8(
                                     inoise8(polarTimer % 2 + x * ff_z,
                                             y * 16 + polarTimer % 16,
                                             polarTimer / speedfactor),
                                     fabs((float)LED_HEIGHT / 2 - (float)y) * emitterX));
        }
    }
}
#endif