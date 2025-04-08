#include"effect.h"
#include"nature_effect.h"

// =====================================
//             Планета Земля
//              PlanetEarth
//             © SlingMaster
// =====================================

void FieryLedLampEffectPlanetEarth::setup()
{
/*    FPSdelay = 96U;
    FastLED.clear();
    String file_name = (modes[currentMode].Scale < 50) ? "globe0" : (HEIGHT >= 24U) ? "globe_big" : "globe1";
    readBinFile("bin/" + file_name + ".img", 4112 );

    imgW = getSizeValue(binImage, 8 );
    imgH = getSizeValue(binImage, 10 );

    scrollImage(imgW, imgH, 0U);
    ff_x = 1U;*/
};
void FieryLedLampEffectPlanetEarth::updateInner()
{
 /* static uint16_t imgW = 0;
  static uint16_t imgH = 0;
  if (HEIGHT < 16U) {
    return;
  }
  
  if (ff_x < 1) ff_x = (imgW - imgH);
  scrollImage(imgW, imgH, ff_x - 1);
  ff_x--;
  //  if (ff_x < 1) ff_x = (imgW - 1);
  if (ff_x == 0) {
    scrollImage(imgW, imgH, 0U);
    ff_x = imgW;
  } else {
    scrollImage(imgW, imgH, ff_x);
  }
*/
}

// ------------- радуга три в одной -------------
void FieryLedLampEffectRainbow::rainbowHorVertRoutine(bool isVertical)
{
    for (uint8_t i = 0U; i < (isVertical ? LED_WIDTH : LED_HEIGHT); i++)
    {
        // CRGB thisColor;
        // hsv2rgb_spectrum(CHSV(hue + i * (modes[currentMode].Scale % 67U) * 2U, 255U, 255U), thisColor); // так ещё хуже стало на низкой яркости
        CHSV thisColor = CHSV((uint8_t)(hue + i * (scale % 67U) * 2U), 255U, 255U);

        for (uint8_t j = 0U; j < (isVertical ? LED_HEIGHT : LED_WIDTH); j++)
            drawPixelXY((isVertical ? i : j), (isVertical ? j : i), thisColor);
    }
}

void FieryLedLampEffectRainbow::setup()
{

};
void FieryLedLampEffectRainbow::updateInner()
{
    uint8_t maxDim=1;
    hue += 4U;
    if (scale < 34U) // если масштаб до 34
        rainbowHorVertRoutine(false);
    else
    {
        if (scale > 67U) // если масштаб больше 67
            rainbowHorVertRoutine(true);
        else // для масштабов посередине
            for (uint8_t i = 0U; i < LED_WIDTH; i++)
                for (uint8_t j = 0U; j < LED_HEIGHT; j++)
                {
                    float twirlFactor = 9.0F * ((scale - 33) / 100.0F); // на сколько оборотов будет закручена матрица, [0..3]
                    CRGB thisColor = CHSV((uint8_t)(hue + ((float)LED_WIDTH / (float)LED_HEIGHT * i + j * twirlFactor) * ((float)255 / (float)maxDim)), 255U, 255U);
                    drawPixelXY(i, j, thisColor);
                }
    }
}

void FieryLedLampEffectRainbow3D::setup()
{
};
void FieryLedLampEffectRainbow3D::updateInner()
{
    fillNoiseLED(&RainbowColors_p);
}

// ========== Botswana Rivers ===========
//      © SlingMaster | by Alex Dovby
//              EFF_RIVERS
//            Реки Ботсваны

//---------------------------------------
void FieryLedLampEffectBotswanaRivers::flora()
{
    uint32_t FLORA_COLOR = 0x2F1F00;
    uint8_t posX = floor(CENTER_X_MINOR - LED_WIDTH * 0.3);
    uint8_t h = random8(LED_HEIGHT - 6U) + 4U;
    DrawLine(posX + 1, 1U, posX + 1, h - 1, 0x000000);
    DrawLine(posX + 2, 1U, posX + 2, h, FLORA_COLOR);
    drawPixelXY(posX + 2, h - random8(floor(h * 0.5)), random8(2U) == 1 ? 0xFF00E0 : random8(2U) == 1 ? 0xFFFF00
                                                                                                      : 0x00FF00);
    drawPixelXY(posX + 1, h - random8(floor(h * 0.25)), random8(2U) == 1 ? 0xFF00E0 : 0xFFFF00);
    if (random8(2U) == 1)
    {
        drawPixelXY(posX + 1, floor(h * 0.5), random8(2U) == 1 ? 0xEF001F : 0x9FFF00);
    }
    h = floor(h * 0.65);
    if (LED_WIDTH > 8)
    {
        DrawLine(posX - 1, 1U, posX - 1, h - 1, 0x000000);
    }
    DrawLine(posX, 1U, posX, h, FLORA_COLOR);
    drawPixelXY(posX, h - random8(floor(h * 0.5)), random8(2U) == 1 ? 0xFF00E0 : 0xFFFF00);
}

//---------------------------------------
void FieryLedLampEffectBotswanaRivers::animeBobbles()
{
    // сдвигаем всё вверх ----
    for (uint8_t x = CENTER_X_MAJOR; x < LED_WIDTH; x++)
    {
        for (uint8_t y = LED_HEIGHT-1; y > 0U; y--)
        {
            if (getPixelColorXY(x, y - 1) == 0xFFFFF7)
            {
                drawPixelXY(x, y, 0xFFFFF7);
                drawPixelXY(x, y - 1, getPixelColorXY(0, y - 1));
            }
        }
    }
    // ----------------------
    if (step % 4 == 0)
    {
        drawPixelXY(CENTER_X_MAJOR + random8(5), 0U, 0xFFFFF7);
        if (step % 12 == 0)
        {
            drawPixelXY(CENTER_X_MAJOR + 2 + random8(3), 0U, 0xFFFFF7);
        }
    }
}

//---------------------------------------
// gradientDownTop • более плавный градиент в отличие от gradientVertical
// но может некоректно работать на лампах собранных на ленточных светодиодах
//---------------------------------------
void gradientDownTop(uint8_t bottom, CHSV bottom_color, uint8_t top, CHSV top_color)
{
    //  FORWARD_HUES: hue always goes clockwise
    //  BACKWARD_HUES: hue always goes counter-clockwise
    //  SHORTEST_HUES: hue goes whichever way is shortest
    //  LONGEST_HUES: hue goes whichever way is longest
    if (STRIP_DIRECTION < 2)
    {
        // STRIP_DIRECTION to UP ========
        fill_gradient(leds, top * LED_WIDTH, top_color, bottom * LED_WIDTH, bottom_color, SHORTEST_HUES);
    }
    else
    {
        // STRIP_DIRECTION to DOWN ======
        fill_gradient(leds, NUM_LEDS - bottom * LED_WIDTH - 1, bottom_color, NUM_LEDS - top * LED_WIDTH, top_color, SHORTEST_HUES);
    }
}

//---------------------------------------
void FieryLedLampEffectBotswanaRivers::createScene(uint8_t idx)
{
    switch (idx)
    {
    case 0:     // blue green ------
      gradientDownTop(floor((LED_HEIGHT - 1) * 0.5), CHSV(96, 255, 100), LED_HEIGHT, CHSV(160, 255, 255));
      gradientDownTop(0, CHSV(96, 255, 255), CENTER_Y_MINOR, CHSV(96, 255, 100));
      break;
    case 1:     // aquamarine green
      gradientDownTop(floor((LED_HEIGHT - 1) * 0.3), CHSV(96, 255, 100), LED_HEIGHT, CHSV(130, 255, 220));
      gradientDownTop(0, CHSV(96, 255, 255), floor(LED_HEIGHT * 0.3), CHSV(96, 255, 100));
      break;
    case 2:     // blue aquamarine -
      gradientDownTop(floor((LED_HEIGHT - 1) * 0.5), CHSV(170, 255, 100), LED_HEIGHT, CHSV(160, 255, 200));
      gradientDownTop(0, CHSV(100, 255, 255), CENTER_Y_MINOR, CHSV(170, 255, 100));
      break;
    case 3:     // yellow green ----
      gradientDownTop(floor((LED_HEIGHT - 1) * 0.5), CHSV(95, 255, 55), LED_HEIGHT, CHSV(70, 255, 200));
      gradientDownTop(0, CHSV(95, 255, 255), CENTER_Y_MINOR, CHSV(100, 255, 55));
      break;
    case 4:     // sea green -------
      gradientDownTop(floor((LED_HEIGHT - 1) * 0.3), CHSV(120, 255, 55), LED_HEIGHT, CHSV(175, 255, 200));
      gradientDownTop(0, CHSV(120, 255, 255), floor(LED_HEIGHT * 0.3), CHSV(120, 255, 55));
      break;
    default:
      gradientDownTop(floor((LED_HEIGHT - 1) * 0.25), CHSV(180, 255, 85), LED_HEIGHT, CHSV(160, 255, 200));
      gradientDownTop(0, CHSV(80, 255, 255), floor(LED_HEIGHT * 0.25), CHSV(180, 255, 85));
      break;
    }
    flora();
}

//---------------------------------------
/*
void createSceneM(uint8_t idx) {
  switch (idx) {
    case 0:     // blue green ------
      gradientVertical(0, CENTER_Y_MINOR, WIDTH, HEIGHT, 96, 150, 100, 255, 255U);
      gradientVertical(0, 0, WIDTH, CENTER_Y_MINOR, 96, 96, 255, 100, 255U);
      break;
    case 1:     // aquamarine green
      gradientVertical(0, floor(HEIGHT  * 0.3), WIDTH, HEIGHT, 96, 120, 100, 220, 255U);
      gradientVertical(0, 0, WIDTH, floor(HEIGHT  * 0.3), 96, 96, 255, 100, 255U);
      break;
    case 2:     // blue aquamarine -
      gradientVertical(0, CENTER_Y_MINOR, WIDTH, HEIGHT, 170, 160, 100, 200, 255U);
      gradientVertical(0, 0, WIDTH, CENTER_Y_MINOR, 100, 170, 255, 100, 255U);
      break;
    case 3:     // yellow green ----
      gradientVertical(0, CENTER_Y_MINOR, WIDTH, HEIGHT, 95, 65, 55, 200, 255U);
      gradientVertical(0, 0, WIDTH, CENTER_Y_MINOR, 95, 100, 255, 55, 255U);
      break;
    case 4:     // sea green -------
      gradientVertical(0, floor(HEIGHT  * 0.3), WIDTH, HEIGHT, 120, 160, 55, 200, 255U);
      gradientVertical(0, 0, WIDTH, floor(HEIGHT  * 0.3), 120, 120, 255, 55, 255U);
      break;
    default:
      drawRec(0, 0, WIDTH, HEIGHT, 0x000050);
      break;
  }
  flora();
}
*/

void FieryLedLampEffectBotswanaRivers::setup()
{
    step = 0;                                          // чтообы при старте эффекта сразу покрасить лампу
    uint8_t divider = floor((scale - 1) / 20);       // маштаб задает смену палитры воды
    /*if (ALT_GRADIENT)
    {
        createSceneM(divider);
    }
    else*/
    {
        createScene(divider);
    }
};
void FieryLedLampEffectBotswanaRivers::updateInner()
{
    // альтернативный градиент для ламп собраных из лент с вертикальной компоновкой
    // для корректной работы ALT_GRADIENT = true
    // для ламп из лент с горизонтальной компоновкой и матриц ALT_GRADIENT = false
    // ALT_GRADIENT = false более производительный и более плавная растяжка
    //------------------------------------------------------------------------------

    uint8_t divider=floor((scale - 1) / 20);

    if (step >= speed)
    {
        step = 0;
    }

    // restore scene after power on ---------
    if (getPixelColorXY(0, LED_HEIGHT - 2) == CRGB::Black)
    {
        /*if (ALT_GRADIENT)
        {
            createSceneM(divider);
        }
        else*/
        {
            createScene(divider);
        }
    }

    // light at the bottom ------------------
    //if (!ALT_GRADIENT)
    {
        if (step % 2 == 0)
        {
            if (random8(6) == 1)
            {
                // fill_gradient(leds, NUM_LEDS - WIDTH, CHSV(96U, 255U, 200U), NUM_LEDS, CHSV(50U, 255U, 255U), SHORTEST_HUES);
                if (STRIP_DIRECTION < 2)
                {
                    fill_gradient(leds, 0, CHSV(96U, 255U, 190U), random8(LED_WIDTH + random8(6)), CHSV(90U, 200U, 255U), SHORTEST_HUES);
                }
                else
                {
                    fill_gradient(leds, NUM_LEDS - random8(LED_WIDTH + random8(6)), CHSV(96U, 255U, 190U), NUM_LEDS, CHSV(90U, 200U, 255U), SHORTEST_HUES);
                }
            }
            else
            {
                // fill_gradient(leds, NUM_LEDS - WIDTH, CHSV(50U, 128U, 255U), NUM_LEDS, CHSV(90U, 255U, 180U), SHORTEST_HUES);
                if (STRIP_DIRECTION < 2)
                {
                    fill_gradient(leds, 0, CHSV(85U, 128U, 255U), random8(LED_WIDTH), CHSV(90U, 255U, 180U), SHORTEST_HUES);
                }
                else
                {
                    fill_gradient(leds, NUM_LEDS - random8(LED_WIDTH), CHSV(85U, 128U, 255U), NUM_LEDS, CHSV(90U, 255U, 180U), SHORTEST_HUES);
                }
            }
        }
    }

    // LOG.printf_P(PSTR("%02d | hue2 = %03d | min = %03d \n\r"), step, hue2, deltaHue2);
    // -------------------------------------
    animeBobbles();
    /*if (custom_eff == 1)
    {
        blurRows(leds, LED_WIDTH, 3U, 10U);
        // blurScreen(beatsin8(0U, 5U, 0U));
    }*/
    step++;
}

// ============= ЭФФЕКТ ДОЖДЬ ===============
// от @Shaitan
void FieryLedLampEffectRain::setup()
{

};
void FieryLedLampEffectRain::updateInner()
{
    for (uint8_t x = 0; x < LED_WIDTH; x++)
    {
        // заполняем случайно верхнюю строку
        CRGB thisColor = getPixelColorXY(x, LED_HEIGHT - 1U);
        if ((uint32_t)thisColor == 0U)
        {
            if (random8(0, 50) == 0U)
            {
                if (scale == 1)
                {
                    drawPixelXY(x, LED_HEIGHT - 1U, CHSV(random(0, 9) * 28, 255U, 255U)); // Радужный дождь
                }
                else
                {
                    if (scale == 100)
                        drawPixelXY(x, LED_HEIGHT - 1U, 0xE0FFFF - 0x101010 * random(0, 4)); // Снег
                    else
                        drawPixelXY(x, LED_HEIGHT - 1U, CHSV(scale * 2.4 + random(0, 16), 255, 255)); // Цветной дождь
                }
            }
        }
        else
            leds[XY(x, LED_HEIGHT - 1U)] -= CHSV(0, 0, random(96, 128));
    }
    // сдвигаем всё вниз
    for (uint8_t x = 0U; x < LED_WIDTH; x++)
    {
        for (uint8_t y = 0U; y < LED_HEIGHT - 1U; y++)
        {
            drawPixelXY(x, y, getPixelColorXY(x, y + 1U));
        }
    }
}

// =====================================
//              RadialWave
//            Радиальная волна
//               © Stepko
// =====================================

void FieryLedLampEffectRadialWave::setup()
{
    for (int8_t x = -CENTER_X_MAJOR; x < CENTER_X_MAJOR + (LED_WIDTH % 2); x++)
    {
        for (int8_t y = -CENTER_Y_MAJOR; y < CENTER_Y_MAJOR + (LED_HEIGHT % 2); y++)
        {
            noise3d[0][x + CENTER_X_MAJOR][y + CENTER_Y_MAJOR] = (atan2(x, y) / PI) * 128 + 127; // thanks ldirko
            noise3d[1][x + CENTER_X_MAJOR][y + CENTER_Y_MAJOR] = hypot(x, y);                    // thanks Sutaburosu
        }
    }
};
void FieryLedLampEffectRadialWave::updateInner()
{
    uint8_t legs = scale / 10;
    uint16_t color_speed;
    uint8_t step = scale % 10;
    if (step < 5)
        color_speed = scale / (3 - step / 2);
    else
        color_speed = scale * (step / 2 - 1);
    scale++;
    for (uint8_t x = 0; x < LED_WIDTH; x++)
    {
        for (uint8_t y = 0; y < LED_HEIGHT; y++)
        {
            byte angle = noise3d[0][x][y];
            byte radius = noise3d[1][x][y];
            leds[XY(x, y)] = CHSV(color_speed + radius * (255 / LED_WIDTH), 255, sin8(scale * 4 + sin8(scale * 4 - radius * (255 / LED_WIDTH)) + angle * legs));
        }
    }
}

// =====================================
//               Snowfall
//            © SlingMaster
//               Снегопад
// =====================================
#if 0
void Snowfall() {
  static byte divider;
  if (loadingFlag) {
#if defined(USE_RANDOM_SETS_IN_APP) || defined(RANDOM_SETTINGS_IN_CYCLE_MODE)
    if (selectedSettings) {
      //                     scale | speed 100 - 190
      setModeSettings(random8(100U), random8(100U, 190U));
    }
#endif //#if defined(USE_RANDOM_SETS_IN_APP) || defined(RANDOM_SETTINGS_IN_CYCLE_MODE)
    loadingFlag = false;
    clearNoiseArr();
    divider = floor(modes[currentMode].Scale / 25);
  }

  if (divider == 1) {
    dimAll(40);
  } else {
    if (divider == 2) {
      gradientVertical(0, 0, WIDTH, HEIGHT, 160, 160, 255, 128, 255U);
    } else {
      FastLED.clear();
    }
  }
  VirtualSnow(divider);
}
#endif

// ------------ Эффект "Тихий Океан"
//  "Pacifica" перенос кода kostyamat
//  Gentle, blue-green ocean waves.
//  December 2019, Mark Kriegsman and Mary Corey March.
//  For Dan.
// https://raw.githubusercontent.com/FastLED/FastLED/master/examples/Pacifica/Pacifica.ino
// https://github.com/DmytroKorniienko/FireLamp_JeeUI/blob/master/src/effects.cpp

static const TProgmemRGBPalette16 pacifica_palette_1 FL_PROGMEM =
{ 0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117,
  0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x14554B, 0x28AA50
};
static const TProgmemRGBPalette16 pacifica_palette_2 FL_PROGMEM =
{ 0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117,
  0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x0C5F52, 0x19BE5F
};
static const TProgmemRGBPalette16 pacifica_palette_3 FL_PROGMEM =
{ 0x000208, 0x00030E, 0x000514, 0x00061A, 0x000820, 0x000927, 0x000B2D, 0x000C33,
  0x000E39, 0x001040, 0x001450, 0x001860, 0x001C70, 0x002080, 0x1040BF, 0x2060FF
};

// Add one layer of waves into the led array
void pacifica_one_layer(CRGB *leds, const TProgmemRGBPalette16& p, uint16_t cistart, uint16_t wavescale, uint8_t bri, uint16_t ioff)
{
  uint16_t ci = cistart;
  uint16_t waveangle = ioff;
  uint16_t wavescale_half = (wavescale / 2) + 20;
  for ( uint16_t i = 0; i < NUM_LEDS; i++) {
    waveangle += 250;
    uint16_t s16 = sin16( waveangle ) + 32768;
    uint16_t cs = scale16( s16 , wavescale_half ) + wavescale_half;
    ci += cs;
    uint16_t sindex16 = sin16( ci) + 32768;
    uint8_t sindex8 = scale16( sindex16, 240);
    CRGB c = ColorFromPalette( p, sindex8, bri, LINEARBLEND);
    leds[i] += c;
  }
}

// Add extra 'white' to areas where the four layers of light have lined up brightly
void pacifica_add_whitecaps(CRGB *leds)
{
  uint8_t basethreshold = beatsin8( 9, 55, 65);
  uint8_t wave = beat8( 7 );

  for ( uint16_t i = 0; i < NUM_LEDS; i++) {
    uint8_t threshold = scale8( sin8( wave), 20) + basethreshold;
    wave += 7;
    uint8_t l = leds[i].getAverageLight();
    if ( l > threshold) {
      uint8_t overage = l - threshold;
      uint8_t overage2 = qadd8( overage, overage);
      leds[i] += CRGB( overage, overage2, qadd8( overage2, overage2));
    }
  }
}

// Deepen the blues and greens
void pacifica_deepen_colors(CRGB *leds)
{
  for ( uint16_t i = 0; i < NUM_LEDS; i++) {
    leds[i].blue = scale8( leds[i].blue,  145);
    leds[i].green = scale8( leds[i].green, 200);
    leds[i] |= CRGB( 2, 5, 7);
  }
}

void FieryLedLampEffectPacific::setup()
{

};
void FieryLedLampEffectPacific::updateInner()
{
    // Increment the four "color index start" counters, one for each wave layer.
    // Each is incremented at a different speed, and the speeds vary over time.
    static uint16_t sCIStart1, sCIStart2, sCIStart3, sCIStart4;
    static uint32_t sLastms = 0;
    uint32_t ms = GET_MILLIS();
    uint32_t deltams = ms - sLastms;
    sLastms = ms;
    uint16_t speedfactor1 = beatsin16(3, 179, 269);
    uint16_t speedfactor2 = beatsin16(4, 179, 269);
    uint32_t deltams1 = (deltams * speedfactor1) / map(speed, 1, 255, 620, 60);
    uint32_t deltams2 = (deltams * speedfactor2) / map(speed, 1, 255, 620, 60);
    uint32_t deltams21 = (deltams1 + deltams2) / 2;
    sCIStart1 += (deltams1 * beatsin88(1011, 10, 13));
    sCIStart2 -= (deltams21 * beatsin88(777, 8, 11));
    sCIStart3 -= (deltams1 * beatsin88(501, 5, 7));
    sCIStart4 -= (deltams2 * beatsin88(257, 4, 6));

    // Clear out the LED array to a dim background blue-green
    fill_solid(leds, NUM_LEDS, CRGB(2, 6, 10));

    // Render each of four layers, with different scales and speeds, that vary over time
    pacifica_one_layer(&*leds, pacifica_palette_1, sCIStart1, beatsin16(3, 11 * 256, 14 * 256), beatsin8(10, 70, 130), 0 - beat16(301));
    pacifica_one_layer(&*leds, pacifica_palette_2, sCIStart2, beatsin16(4, 6 * 256, 9 * 256), beatsin8(17, 40, 80), beat16(401));
    pacifica_one_layer(&*leds, pacifica_palette_3, sCIStart3, 6 * 256, beatsin8(9, 10, 38), 0 - beat16(503));
    pacifica_one_layer(&*leds, pacifica_palette_3, sCIStart4, 5 * 256, beatsin8(8, 10, 28), beat16(601));

    // Add brighter 'whitecaps' where the waves lines up more
    pacifica_add_whitecaps(&*leds);

    // Deepen the blues and greens a bit
    pacifica_deepen_colors(&*leds);
    blurScreen(20);
};

// =====================================
//           Rainbow Tornado
//  base code © Stepko, © Sutaburosu
//        and © SlingMaster
//   adapted and modifed © alvikskor
//              Торнадо
// =====================================

void FieryLedLampEffectTornado::setup()
{
    const byte OFFSET = 1U;
    const uint8_t H = LED_HEIGHT - OFFSET;
    for (int8_t x = -CENTER_X_MAJOR; x < CENTER_X_MAJOR; x++)
    {
        for (int8_t y = -OFFSET; y < H; y++)
        {
            noise3d[0][x + CENTER_X_MAJOR][y + OFFSET] = 128 * (atan2(y, x) / PI);
            noise3d[1][x + CENTER_X_MAJOR][y + OFFSET] = hypot(x, y); // thanks Sutaburosu
        }
    }
};
void FieryLedLampEffectTornado::updateInner()
{
    scale += speed / 10;
    for (uint8_t x = 0; x < LED_WIDTH; x++)
    {
        for (uint8_t y = 0; y < LED_HEIGHT; y++)
        {
            byte angle = noise3d[0][x][y];
            byte radius = noise3d[1][x][y];
            leds[XY(x, y)] = CHSV((angle * scale / 10) - scale + (radius * scale / 10), constrain((uint16_t)y * 512U / (uint16_t)LED_HEIGHT, 0, 255), (y < (LED_HEIGHT / 8) ? 255 - (((LED_HEIGHT / 8) - y) * 16) : 255));
        }
    }
}

// ============= Эффект Цветные драже ===============
// (c) SottNick
//по мотивам визуала эффекта by Yaroslaw Turbin 14.12.2020
//https://vk.com/ldirko программный код которого он запретил брать

void FieryLedLampEffectSand::setup()
{
    pcnt = 0;
};
void FieryLedLampEffectSand::updateInner()
{
    // если насыпалось уже достаточно, бахаем рандомные песчинки
    uint8_t temp = map8(random8(), scale, 255U);
    if (pcnt >= map8(temp, 2U, LED_HEIGHT - 3U))
    {
        temp = LED_HEIGHT + 1 - pcnt;
        if (!random8(4U)) // иногда песка осыпается до половины разом
            if (random8(2U))
                temp = 2U;
            else
                temp = 3U;
        for (uint8_t y = 0; y < pcnt; y++)
            for (uint8_t x = 0; x < LED_WIDTH; x++)
                if (!random8(temp))
                    leds[XY(x, y)] = 0;
    }

    pcnt = 0;
    // осыпаем всё, что есть на экране
    for (uint8_t y = 1; y < LED_HEIGHT; y++)
    {
        for (uint8_t x = 0; x < LED_WIDTH; x++)
        {
            if (leds[XY(x, y)]) // проверяем для каждой песчинки
            {
                if (!leds[XY(x, y - 1)])
                { // если под нами пусто, просто падаем
                    leds[XY(x, y - 1)] = leds[XY(x, y)];
                    leds[XY(x, y)] = 0;
                }
                else if (x > 0U && !leds[XY(x - 1, y - 1)] && x < LED_WIDTH - 1 && !leds[XY(x + 1, y - 1)])
                { // если под нами пик
                    if (random8(2U))
                        leds[XY(x - 1, y - 1)] = leds[XY(x, y)];
                    else
                        leds[XY(x - 1, y - 1)] = leds[XY(x, y)];
                    leds[XY(x, y)] = 0;
                    pcnt = y - 1;
                }
                else if (x > 0U && !leds[XY(x - 1, y - 1)])
                { // если под нами склон налево
                    leds[XY(x - 1, y - 1)] = leds[XY(x, y)];
                    leds[XY(x, y)] = 0;
                    pcnt = y - 1;
                }
                else if (x < LED_WIDTH - 1 && !leds[XY(x + 1, y - 1)])
                { // если под нами склон направо
                    leds[XY(x + 1, y - 1)] = leds[XY(x, y)];
                    leds[XY(x, y)] = 0;
                    pcnt = y - 1;
                }
                else // если под нами плато
                    pcnt = y;
            }
        }
    }
    // эмиттер новых песчинок
    if (!leds[XY(CENTER_X_MINOR, LED_HEIGHT - 2)] && !leds[XY(CENTER_X_MAJOR, LED_HEIGHT - 2)] && !random8(3))
    {
        temp = random8(2) ? CENTER_X_MINOR : CENTER_X_MAJOR;
        leds[XY(temp, LED_HEIGHT - 1)] = CHSV(random8(), 255U, 255U);
    }
}

// ============ Lotus Flower ============
//             © SlingMaster
//             Цветок Лотоса
//---------------------------------------
void FieryLedLampEffectLotus::drawLotusFlowerFragment(uint8_t posX, byte line)
{
    const uint8_t h = (LED_HEIGHT > 24) ? LED_HEIGHT * 0.9 : LED_HEIGHT-1;
    uint8_t flover_color = 128 + abs(128 - hue);          // 128 -- 255
    uint8_t gleam = 255 - abs(128 - hue2);                // 255 -- 128
    float f_size = (128 - abs(128 - deltaValue)) / 150.0; // 1.0 -- 0.0
    const byte lowBri = 112U;
    // clear -----
    //DrawLine(posX, 0, posX, h * 1.1, CRGB::Black);
    DrawLine(posX, 0, posX, h, CRGB::Black);

    switch (line)
    {
    case 0:
        gradientVertical(posX, 0, posX + 1, h * 0.22, 96, 96, 32, 255, 255U);                        // green leaf c
        gradientVertical(posX, h * 0.9, posX + 1, h * 1.1, 64, 48, 64, 205, gleam);                  // pestle
        gradientVertical(posX, 8, posX + 1, h * 0.6, flover_color, flover_color, 128, lowBri, 255U); // ---
        break;
    case 2:
    case 6:
        gradientVertical(posX, h * 0.2, posX + 1, h - 4, flover_color, flover_color, lowBri, 255, gleam); //  -->
        gradientVertical(posX, h * 0.05, posX + 1, h * 0.15, 96, 96, 32, 255, 255U);                      // green leaf
        break;
    case 3:
    case 5:
        gradientVertical(posX, h * 0.5, posX + 1, h - 2, flover_color, flover_color, lowBri, 255, 255U); // ---->
        break;
    case 4:
        gradientVertical(posX, 1 + h * f_size, posX + 1, h, flover_color, flover_color, lowBri, 255, gleam); // ------>
        break;
    default:
        gradientVertical(posX, h * 0.05, posX + 1, h * 0.2, 80, 96, 160, 64, 255U); // green leaf m
        break;
    }
}

void FieryLedLampEffectLotus::setup()
{
    step = 0;
    deltaSpeed = 0;
    hue2 = 128U;
    deltaValue = 0;
    hue = 224;
    //FPSdelay = SpeedFactor(160);
};
void FieryLedLampEffectLotus::updateInner()
{
    const byte STEP_OBJ = 8;
    
    if (speed > 128U)
    {
        if (scale > 50)
        {
            deltaSpeed = 80 + (128U - abs(128 - deltaValue)) / 1.25;
            //FPSdelay = SpeedFactor(deltaSpeed);
            if (step % 256 == 0U)
                hue += 32; /* color morph */
        }
        else
        {
            //FPSdelay = SpeedFactor(160);
            hue = 28U;
        }
        deltaValue++; /* size morph  */
        /* <==== scroll ===== */
        drawLotusFlowerFragment(LED_WIDTH - 1, (step % STEP_OBJ));
        for (uint8_t y = 0U; y < LED_HEIGHT; y++)
        {
            for (uint8_t x = 0U; x < LED_WIDTH; x++)
            {
                drawPixelXY(x - 1, y, getPixelColorXY(x, y));
            }
        }
    }
    else
    {
        /* <==== morph ===== */
        for (uint8_t x = 0U; x < LED_WIDTH; x++)
        {
            drawLotusFlowerFragment(x, (x % STEP_OBJ));
            if (x % 2U)
            {
                hue2++; /* gleam morph */
            }
        }
        deltaValue++; /* size morph  */
        if (scale > 50)
        {
            hue += 8; /* color morph */
        }
        else
        {
            hue = 28U;
        }
    }
    step++;
}

// ======== Digital Тurbulence =========
//             © SlingMaster
//        Цифровая турбулентность
// =====================================
void FieryLedLampEffectTurbulence::drawRandomCol(uint8_t x, uint8_t y, uint8_t offset, uint32_t count)
{
    const byte STEP = 32;
    const byte D = LED_HEIGHT / 8;
    uint8_t color = floor(y / D) * STEP + offset;

    if (count == 0U)
    {
        drawPixelXY(x, y, CHSV(color, 255, random8(8U) == 0U ? (step % 2U ? 0 : 255) : 0));
    }
    else
    {
        drawPixelXY(x, y, CHSV(color, 255, (bitRead(count, y) == 1U) ? (step % 5U ? 0 : 255) : 0));
    }
}
void FieryLedLampEffectTurbulence::setup()
{
    step = 0;
    deltaValue = 0;
    hue = 0;
};
void FieryLedLampEffectTurbulence::updateInner()
{
    const byte DEPTH = 2;
    CRGB curColor;

    deltaValue++; /* size morph  */

    /* <==== scroll =====> */
    for (uint8_t y = LED_HEIGHT; y > 0; y--)
    {
        drawRandomCol(0, y - 1, hue, count);
        drawRandomCol(LED_WIDTH - 1, y - 1, hue + 128, count);

        // left -----
        for (uint8_t x = CENTER_X_MAJOR - 1; x > 0; x--)
        {
            /* ---> */
            curColor = getPixelColorXY(x - 1, y - 1);
            if (x < CENTER_X_MAJOR - DEPTH / 2)
            {
                drawPixelXY(x, y - 1, curColor);
            }
            else
            {
                if (curColor != 0U)
                    drawPixelXY(x, y - 1, curColor);
            }
        }

        // right -----
        for (uint8_t x = CENTER_X_MAJOR + 1; x < LED_WIDTH; x++)
        {
            /* <---  */
            curColor = getPixelColorXY(x, y - 1);
            if (x > CENTER_X_MAJOR + DEPTH / 2)
            {
                drawPixelXY(x - 1, y - 1, curColor);
            }
            else
            {
                if (curColor != 0U)
                    drawPixelXY(x - 1, y - 1, curColor);
            }
        }

        /* scroll center up ---- */
        for (uint8_t x = CENTER_X_MAJOR - DEPTH; x < CENTER_X_MAJOR + DEPTH; x++)
        {
            if(y == LED_HEIGHT)
                break;
            CRGB newcolor=getPixelColorXY(x, y - 1);
            newcolor.fadeToBlackBy(128 / y);
            drawPixelXY(x, y, newcolor);
            //drawPixelXY(x, y, makeDarker(getPixelColorXY(x, y - 1), 128 / y));
            if (y == 1)
            {
                drawPixelXY(x, 0, CRGB::Black);
            }
        }
        /* --------------------- */
    }

    if (scale > 50)
    {
        count++;
        if (count % 256 == 0U)
            hue += 16U;
    }
    else
    {
        count = 0;
    }
    step++;
}

// ------------- светлячки --------------
//#define LIGHTERS_AM           (100U)  // для экономии памяти берём trackingOBJECT_MAX_COUNT
///////#define trackingOBJECT_MAX_COUNT
//int32_t lightersPos[2U][LIGHTERS_AM];
///////float trackingObjectPosX[trackingOBJECT_MAX_COUNT];
///////float trackingObjectPosY[trackingOBJECT_MAX_COUNT];
//int8_t lightersSpeed[2U][LIGHTERS_AM];
///////float trackingObjectSpeedX[trackingOBJECT_MAX_COUNT];
///////float trackingObjectSpeedY[trackingOBJECT_MAX_COUNT];
//CHSV lightersColor[LIGHTERS_AM];
///////uint8_t trackingObjectHue[trackingOBJECT_MAX_COUNT];
//uint8_t step; // раньше называлось uint8_t loopCounter;
//int32_t angle[LIGHTERS_AM];она нигде не используется. нафига она тут?!
//int32_t speedV[LIGHTERS_AM]; она нигде не используется. нафига она тут?!
//int8_t angleSpeed[LIGHTERS_AM]; она нигде не используется. нафига она тут?!

void FieryLedLampEffectLighters::setup()
{
    step=0;
    if (scale > trackingOBJECT_MAX_COUNT)
        scale = trackingOBJECT_MAX_COUNT;
    for (uint8_t i = 0U; i < trackingOBJECT_MAX_COUNT; i++)
    {
        trackingObjectPosX[i] = random(0, LED_WIDTH * 10);
        trackingObjectPosY[i] = random(0, LED_HEIGHT * 10);
        trackingObjectSpeedX[i] = random(-10, 10);
        trackingObjectSpeedY[i] = random(-10, 10);
        trackingObjectHue[i] = random8();
    }
};
void FieryLedLampEffectLighters::updateInner()
{
    FastLED.clear();
    if (++step > 20)
        step = 0;
    for (uint8_t i = 0; i < scale; i++)
    {
        if (step == 0) // меняем скорость каждые 255 отрисовок
        {
            trackingObjectSpeedX[i] += random(-3, 4);
            trackingObjectSpeedY[i] += random(-3, 4);
            trackingObjectSpeedX[i] = constrain(trackingObjectSpeedX[i], -20, 20);
            trackingObjectSpeedY[i] = constrain(trackingObjectSpeedY[i], -20, 20);
        }

        trackingObjectPosX[i] += trackingObjectSpeedX[i];
        trackingObjectPosY[i] += trackingObjectSpeedY[i];

        if (trackingObjectPosX[i] < 0)
            trackingObjectPosX[i] = (LED_WIDTH - 1) * 10;
        if (trackingObjectPosX[i] >= (int32_t)(LED_WIDTH * 10))
            trackingObjectPosX[i] = 0;

        if (trackingObjectPosY[i] < 0)
        {
            trackingObjectPosY[i] = 0;
            trackingObjectSpeedY[i] = -trackingObjectSpeedY[i];
        }
        if (trackingObjectPosY[i] >= (int32_t)(LED_HEIGHT - 1) * 10)
        {
            trackingObjectPosY[i] = (LED_HEIGHT - 1U) * 10;
            trackingObjectSpeedY[i] = -trackingObjectSpeedY[i];
        }
        drawPixelXY(trackingObjectPosX[i] / 10, trackingObjectPosY[i] / 10, CHSV(trackingObjectHue[i], 255U, 255U));
    }
}

// --------------------------- светлячки со шлейфом ---------------------
void FieryLedLampEffectLighterTraces::setup()
{
    for (uint8_t j = 0U; j < BALLS_AMOUNT; j++)
    {
        int8_t sign;
        // забиваем случайными данными
        coord[j][0U] = LED_WIDTH / 2 * 10;
        random(0, 2) ? sign = 1 : sign = -1;
        vector[j][0U] = random(4, 15) * sign;
        coord[j][1U] = LED_HEIGHT / 2 * 10;
        random(0, 2) ? sign = 1 : sign = -1;
        vector[j][1U] = random(4, 15) * sign;
        //  цвет зависит от масштаба
        ballColors[j] = CHSV((scale * (j + 1)) % 256U, 255U, 255U);
    }
};
void FieryLedLampEffectLighterTraces::updateInner()
{
    /*if (!BALL_TRACK) // режим без следов шариков
    {
        FastLED.clear();
    }
    else // режим со следами
    {
        // fader(TRACK_STEP);
        dimAll(256U - TRACK_STEP);
    }*/
#if defined(BALL_TRACK)
    dimAll(256U - TRACK_STEP);
#else
    FastLED.clear();
#endif

    // движение шариков
    for (uint8_t j = 0U; j < BALLS_AMOUNT; j++)
    {
        // движение шариков
        for (uint8_t i = 0U; i < 2U; i++)
        {
            coord[j][i] += vector[j][i];
            if (coord[j][i] < 0)
            {
                coord[j][i] = 0;
                vector[j][i] = -vector[j][i];
            }
        }

        if (coord[j][0U] > (int16_t)((LED_WIDTH - 1) * 10))
        {
            coord[j][0U] = (LED_WIDTH - 1) * 10;
            vector[j][0U] = -vector[j][0U];
        }
        if (coord[j][1U] > (int16_t)((LED_HEIGHT - 1) * 10))
        {
            coord[j][1U] = (LED_HEIGHT - 1) * 10;
            vector[j][1U] = -vector[j][1U];
        }
        drawPixelXYF(coord[j][0U] / 10., coord[j][1U] / 10., ballColors[j]);
    }
}
