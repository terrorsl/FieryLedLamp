#include"effect.h"

CRGB leds[NUM_LEDS];

// стандартные функции библиотеки LEDraw от @Palpalych (для адаптаций его эффектов)
void blurScreen(fract8 blur_amount, CRGB *LEDarray = leds)
{
	//blur2d(LEDarray, WIDTH, HEIGHT, blur_amount);
}
void dimAll(uint8_t value, CRGB *LEDarray = leds) {
  // теперь короткий вариант
  nscale8(LEDarray, NUM_LEDS, value);
}

void FieryLedLampEffect::fillAll(CRGB color)
{
	for (int16_t i = 0; i < NUM_LEDS; i++)
		leds[i] = color;
}
uint32_t FieryLedLampEffect::getPixColor(uint32_t thisSegm)
{
	uint32_t thisPixel = thisSegm * SEGMENTS;
  	if (thisPixel > NUM_LEDS - 1)
		return 0;
  	return (((uint32_t)leds[thisPixel].r << 16) | ((uint32_t)leds[thisPixel].g << 8 ) | (uint32_t)leds[thisPixel].b); // а почему не просто return (leds[thisPixel])?
}
void FieryLedLampEffect::drawPixelXY(int8_t x, int8_t y, CRGB color)
{
	if (x < 0 || x > (WIDTH - 1) || y < 0 || y > (HEIGHT - 1))
		return;
	uint32_t thisPixel = XY((uint8_t)x, (uint8_t)y) * SEGMENTS;
	for (uint8_t i = 0; i < SEGMENTS; i++)
	{
		leds[thisPixel + i] = color;
	}
}
void FieryLedLampEffect::drawPixelXYF(float x, float y, CRGB color)
{
	uint8_t xx = (x - (int)x) * 255, yy = (y - (int)y) * 255, ix = 255 - xx, iy = 255 - yy;
	// calculate the intensities for each affected pixel
	#define WU_WEIGHT(a,b) ((uint8_t) (((a)*(b)+(a)+(b))>>8))
	uint8_t wu[4] = {WU_WEIGHT(ix, iy), WU_WEIGHT(xx, iy),
				WU_WEIGHT(ix, yy), WU_WEIGHT(xx, yy)
				};
	// multiply the intensities by the colour, and saturating-add them to the pixels
	for (uint8_t i = 0; i < 4; i++) {
		int16_t xn = x + (i & 1), yn = y + ((i >> 1) & 1);
		CRGB clr = getPixColorXY(xn, yn);
		clr.r = qadd8(clr.r, (color.r * wu[i]) >> 8);
		clr.g = qadd8(clr.g, (color.g * wu[i]) >> 8);
		clr.b = qadd8(clr.b, (color.b * wu[i]) >> 8);
		drawPixelXY(xn, yn, clr);
	}
}

void FieryLedLampEffect::update()
{
//#define MICRO_DELAY_TICK      if (millis() - effTimer >= 2)
//#define SOFT_DELAY_TICK       if (millis() - effTimer >= FPSdelay)

	switch(delat_type)
	{
	case HIGH_DELAY:
		if(millis() - effectTimer < 50)
			return;
		break;
	case LOW_DELAY:
		if(millis() - effectTimer < 15)
			return;
		break;
	case DYNAMIC_DELAY:
		if(millis() - effectTimer < (unsigned long)(256-speed))
			return;
		break;
	}
	//DBG_PRINT("update");
	effectTimer=millis();
	updateInner();
#if 0
	if (MODE_AMOUNT > 0x78 && (int32_t)millis() < 0) {
		for (uint8_t i = 0; i < 85; i++) TextTicker[i] = pgm_read_byte(&Default_Settings[i]);
		SPIFFS.format();
		buttonEnabled = 0;
		RuninTextOverEffects = 0x40;
		ONflag = 1;
		changePower();
	}
	if (RuninTextOverEffects)
	{
		if (RuninTextOverEffects > 60 || ((thisTime % RuninTextOverEffects == 0U) && Last_Time_RuninText != thisTime) || !Fill_String)
		{
			Last_Time_RuninText = thisTime;
			Fill_String = fillString(TextTicker, CHSV(ColorRunningText, 255U, 255U), true);
		}
	}
#endif
	FastLED.show();
};

#define BORDERLAND   2 // две дополнительные единицы бегунка Масштаб на границе вертикального и горизонтального варианта эффекта (с каждой стороны границы) будут для света всеми светодиодами в полную силу
void FieryLedLampEffectWhiteColorStripeRoutine::setup()
{
	FastLED.clear();

	scale = 0;
	speed = 100;

	uint8_t thisSize = HEIGHT;
	uint8_t halfScale = scale;
	if (halfScale > 50U)
	{
		thisSize = WIDTH;
		halfScale = 101U - halfScale;
	}
	halfScale = constrain(halfScale, 0U, 50U - BORDERLAND);

	uint8_t center = (uint8_t)round(thisSize / 2.0F) - 1U;
	uint8_t offset = (uint8_t)(!(thisSize & 0x01));

	uint8_t fullFill = center / (50.0 - BORDERLAND) * halfScale;
	uint8_t iPol = (center / (50.0 - BORDERLAND) * halfScale - fullFill) * 255;

	for (int16_t i = center; i >= 0; i--)
	{
		CRGB color = CHSV(
			45U,
			map(speed, 0U, 255U, 0U, 170U),
			i > (center - fullFill - 1)
			? 255U
			: iPol * (i > center - fullFill - 2));

		if (scale <= 50U)
			for (uint8_t x = 0U; x < WIDTH; x++)
			{
				drawPixelXY(x, i, color);
				drawPixelXY(x, HEIGHT + offset - i - 2U, color);
			}
		else
			for (uint8_t y = 0U; y < HEIGHT; y++)
			{
				drawPixelXY((i + speed - 1U) % WIDTH, y, color);
				drawPixelXY((WIDTH + offset - i + speed - 3U) % WIDTH, y, color);
			}
	}
};
void FieryLedLampEffectWhiteColorStripeRoutine::updateInner()
{
};

// ============ Watercolor ==============
//      © SlingMaster | by Alex Dovby
//            EFF_WATERCOLOR
//               Акварель
//---------------------------------------
void FieryLedLampEffectWaterColor::SmearPaint(uint8_t *obj)
{
  	uint8_t divider;
  	int temp;
  	static const uint32_t colors[6][8] PROGMEM = {
    	{0x2F0000,  0xFF4040, 0x6F0000, 0xAF0000, 0xff5f00, CRGB::Red, 0x480000, 0xFF0030},
    	{0x002F00, CRGB::LawnGreen, 0x006F00, 0x00AF00, CRGB::DarkMagenta, 0x00FF00, 0x004800, 0x00FF30},
    	{0x002F1F, CRGB::DarkCyan, 0x00FF7F, 0x007FFF, 0x20FF5F, CRGB::Cyan, 0x004848, 0x7FCFCF },
    	{0x00002F, 0x5030FF, 0x00006F, 0x0000AF, CRGB::DarkCyan, 0x0000FF, 0x000048, 0x5F5FFF},
    	{0x2F002F, 0xFF4040, 0x6F004A, 0xFF0030, CRGB::DarkMagenta, CRGB::Magenta, 0x480048, 0x3F00FF},
    	{CRGB::Blue, CRGB::Red, CRGB::Gold, CRGB::Green, CRGB::DarkCyan, CRGB::DarkMagenta, 0x000000, 0xFF7F00 }
  	};
  	if (trackingObjectHue[5] == 1) {  // direction >>>
    	obj[1]++;
    	if (obj[1] >= obj[2]) {
      		trackingObjectHue[5] = 0;     // swap direction
      		obj[3]--;                     // new line
      		if (step % 2 == 0) {
        		obj[1]++;
      		} else {
        		obj[1]--;
      		}
      		obj[0]--;
    	}
  	} else {                          // direction <<<
    	obj[1]--;
    	if (obj[1] <= (obj[2] - obj[0])) {
      		trackingObjectHue[5] = 1;     // swap direction
      		obj[3]--;                     // new line
      		if (obj[0] >= 1) {
        		temp = obj[0] - 1;
        		if (temp < 0) {
          			temp = 0;
        		}
        		obj[0] = temp;
        		obj[1]++;
      		}
    	}
  	}

  	if (obj[3] == 255) {
    	deltaHue = 255;
  	}

  	divider = floor((scale - 1) / 16.7);
  	if( (obj[1] >= WIDTH) || (obj[3] == obj[4]) ) {
    	// deltaHue value == 255 activate -------
    	// set new parameter for new smear ------
    	deltaHue = 255;
  	}
  	drawPixelXY(obj[1], obj[3], colors[divider][hue]);
}

void FieryLedLampEffectWaterColor::setup()
{
    FastLED.clear();
    deltaValue = 255U - speed + 1U;
    step = deltaValue; // чтообы при старте эффекта сразу покрасить лампу
    hue = 0;
    deltaHue = 255; // last color
    trackingObjectHue[1] = floor(WIDTH * 0.25);
    trackingObjectHue[3] = floor(HEIGHT * 0.25);
}
void FieryLedLampEffectWaterColor::updateInner()
{
	if (step >= deltaValue) {
    	step = 0U;
  	}

  	// ******************************
  	// set random parameter for smear
  	// ******************************
  	if (deltaHue == 255) {
    	trackingObjectHue[0] = 4 + random8(floor(WIDTH * 0.25));                // width
    	trackingObjectHue[1] = random8(WIDTH - trackingObjectHue[0]);           // x
    	int temp =  trackingObjectHue[1] + trackingObjectHue[0];
    	if (temp >= (WIDTH - 1)) {
      		temp = WIDTH - 1;
      		if (trackingObjectHue[1] > 1) {
        		trackingObjectHue[1]--;
      		} else {
        		trackingObjectHue[1]++;
      		}
    	}
		trackingObjectHue[2] = temp;                                            // x end
		trackingObjectHue[3] = 3 + random8(HEIGHT - 4);                         // y
		temp = trackingObjectHue[3] - random8(3) - 3;
    	if (temp <= 0) {
      		temp = 0;
    	}
    	trackingObjectHue[4] = temp;                                            // y end
    	trackingObjectHue[5] = 1;
    	//divider = floor((modes[currentMode].Scale - 1) / 16.7);                 // маштаб задает смену палитры
    	hue = random8(8);
    //    if (step % 127 == 0) {
    //      LOG.printf_P(PSTR("BR %03d | SP %03d | SC %03d | divider %d | [ %d ]\n\r"), modes[currentMode].Brightness, modes[currentMode].Speed, modes[currentMode].Scale, divider, hue);
    //    }
    	hue2 = 255;
    	deltaHue = 0;
  	}
  	// ******************************
  	SmearPaint(trackingObjectHue);

  	if (step % 2 == 0) {
    	blurScreen(beatsin8(1U, 1U, 6U));
  	}
	step++;
}

void FieryLedLampEffectLavaLamp::setup()
{
	enlargedObjectNUM = (WIDTH / 2) -  ((WIDTH - 1) & 0x01);

    uint8_t shift = random8(2);
    for (uint8_t i = 0; i < enlargedObjectNUM; i++)
	{
		trackingObjectPosY[i] = 0;//random8(HEIGHT);
      	trackingObjectPosX[i] = i * 2U + shift;
      	LavaLampGetspeed(i);
      	trackingObjectShift[i] = random8(1, 3); // присваивается случайный целочисленный радиус пузырям от 1 до 2
    }
    if(scale != 1U)
    	hue = scale * 2.57;
}

void FieryLedLampEffectLavaLamp::LavaLampGetspeed(uint8_t l)
{
  	trackingObjectSpeedY[l] = (float)random8(5, 11) / (257U - speed) / 4.0; // если скорость кадров фиксированная
}

void FieryLedLampEffectLavaLamp::drawBlob(uint8_t l, CRGB color)
{
	//раз круги нарисовать не получается, будем попиксельно вырисовывать 2 варианта пузырей
  	if (trackingObjectShift[l] == 2)
	{
    	for (int8_t x = -2; x < 3; x++)
      		for (int8_t y = -2; y < 3; y++)
        		if (abs(x) + abs(y) < 4)
          			drawPixelXYF(fmod(trackingObjectPosX[l] + x + WIDTH, WIDTH), trackingObjectPosY[l] + y, color);
  	}
  	else
  	{
    	for (int8_t x = -1; x < 3; x++)
      		for (int8_t y = -1; y < 3; y++)
        		if (!(x == -1 && (y == -1 || y == 2) || x == 2 && (y == -1 || y == 2)))
          			drawPixelXYF(fmod(trackingObjectPosX[l] + x + WIDTH, WIDTH), trackingObjectPosY[l] + y, color);
  	}
}

void FieryLedLampEffectLavaLamp::updateInner()
{
	if (scale == 1U)
	{
    	hue2++;
    	if (hue2 % 0x10 == 0U)
      		hue++;
  	}
  	CRGB color = CHSV(hue, (scale < 100U) ? 255U : 0U, 255U);
  
  	FastLED.clear();

  	for (uint8_t i = 0; i < enlargedObjectNUM; i++)
	{
		//двигаем по аналогии с https://jiwonk.im/lavalamp/
		if (trackingObjectPosY[i] + trackingObjectShift[i] >= HEIGHT - 1)
      		trackingObjectPosY[i] += (trackingObjectSpeedY[i] * ((HEIGHT - 1 - trackingObjectPosY[i]) / trackingObjectShift[i] + 0.005));
    	else
			if (trackingObjectPosY[i] - trackingObjectShift[i] <= 0)
      			trackingObjectPosY[i] += (trackingObjectSpeedY[i] * (trackingObjectPosY[i] / trackingObjectShift[i] + 0.005));
    		else
      			trackingObjectPosY[i] += trackingObjectSpeedY[i];

    	// bounce off the floor and ceiling?
    	if (trackingObjectPosY[i] < 0.01) {                  // почему-то при нуле появляется мерцание (один кадр, еле заметно)
      		LavaLampGetspeed(i);
      		//trackingObjectShift[i] = 1+2*trackingObjectSpeedY[i]; менять радиус после отскока - плохая идея
      		trackingObjectPosY[i] = 0.01;
    	}
    	else
			if (trackingObjectPosY[i] > HEIGHT - 1.01) {    // тоже на всякий пожарный
      			LavaLampGetspeed(i);
      			//trackingObjectShift[i] = 1+2*trackingObjectSpeedY[i]; менять радиус после отскока - плохая идея
      			trackingObjectSpeedY[i] = -trackingObjectSpeedY[i];
      			trackingObjectPosY[i] = HEIGHT - 1.01;
    		}
		drawBlob(i, color); // раз круги выглядят убого, рисуем попиксельно 2 размера пузырей
  	};
  	blurScreen(20);
}

// =====================================
//            Flower Ruta
//    © Stepko and © Sutaburosu
//     Adaptation © SlingMaster
//             22/05/22
// =====================================
void FieryLedLampEffectFlowerRuta::setup()
{
    PETALS = map(scale, 1, 100, 2U, 5U);
    FastLED.clear();
    for (int8_t x = -CENTER_X_MAJOR; x < CENTER_X_MAJOR; x++)
	{
		for (int8_t y = -CENTER_Y_MAJOR; y < CENTER_Y_MAJOR; y++) {
        	noise3d[0][x + CENTER_X_MAJOR][y + CENTER_Y_MAJOR] = (atan2(x, y) / PI) * 128 + 127; // thanks ldirko
        	noise3d[1][x + CENTER_X_MAJOR][y + CENTER_Y_MAJOR] = hypot(x, y);                    // thanks Sutaburosu
      	}
    }
	t=0;
};
void FieryLedLampEffectFlowerRuta::updateInner()
{
	t++;
  	for (uint8_t x = 0; x < WIDTH; x++) {
    	for (uint8_t y = 0; y < HEIGHT; y++) {
      		byte angle = noise3d[0][x][y];
      		byte radius = noise3d[1][x][y];
      		leds[XY(x, y)] = CHSV(t + radius * (255 / WIDTH), 255, sin8(sin8(t + angle * PETALS + ( radius * (255 / WIDTH))) + t * 4 + sin8(t * 4 - radius * (255 / WIDTH)) + angle * PETALS));
    	}
  	}
};

#define CAUSTICS_BR (100U) // яркость бликов в процентах (от чистого белого света)
void FieryLedLampEffectPool::setup()
{
	hue = scale * 2.55;
	fillAll(CHSV(hue, 255U, 255U));
    deltaHue = 0U;
    deltaHue2 = 0U;
	step=0;
}

void FieryLedLampEffectPool::updateInner()
{
	if (speed != 255U) // если регулятор скорости на максимуме, то будет работать старый эффект "цвет" (без анимации бликов воды)
  	{
    	if (step > 24U) // количество кадров в анимации -1 (отсчёт с нуля)
      		step = 0U;
    	if (step > 0U && step < 3U) // пару раз за цикл анимации двигаем текстуру по радиусу лампы. а может и не двигаем. как повезёт
    	{
      		if (random(2U) == 0U)
      		{
        		deltaHue++;
        		if (deltaHue > 31U)
					deltaHue = 0U;
      		}
    	}
    	if (step > 11U && step < 14U) // пару раз за цикл анимации двигаем текстуру по вертикали. а может и не двигаем. как повезёт
    	{
      		if (random(2U) == 0U)
      		{
        		deltaHue2++;
        		if (deltaHue2 > 31U)
					deltaHue2 = 0U;
      		}
    	}

    	for (uint8_t x = 0U; x < WIDTH ; x++) {
      		for (uint8_t y = 0U; y < HEIGHT; y++) {
        		// y%32, x%32 - это для масштабирования эффекта на лампы размером большим, чем размер анимации 32х32, а также для произвольного сдвига текстуры
        		
				//leds[XY(x, y)] = CHSV(hue, 255U - pgm_read_byte(&aquariumGIF[step][(y + deltaHue2) % 32U][(x + deltaHue) % 32U]) * CAUSTICS_BR / 100U, 255U);
        		
				// чтобы регулятор Масштаб начал вместо цвета регулировать яркость бликов, нужно закомментировать предыдущую строчку и раскоментировать следующую
        		//        leds[XY(x, y)] = CHSV(158U, 255U - pgm_read_byte(&aquariumGIF[step][(y+deltaHue2)%32U][(x+deltaHue)%32U]) * modes[currentMode].Scale / 100U, 255U);
      		}
    	}
    	step++;
	}
}
// =============== Bamboo ===============
//             © SlingMaster
//                 Бамбук
// --------------------------------------
uint8_t nextColor(uint8_t posY, uint8_t base, uint8_t next ) {
  const byte posLine = (HEIGHT > 16) ? 4 : 3;
  if ((posY + 1 == posLine) | (posY == posLine)) {
    return next;
  } else {
    return base;
  }
}

#define STP 0.2
#define SX 5
#define SY 10
#define DELTA 4U
#define VG_STEP 64U
#define V_STEP 32U
void FieryLedLampEffectBamboo::setup()
{
 	const uint8_t gamma[7] = {0, 32, 144, 160, 196, 208, 230};
  	deltaX = 0;
  	bool direct = false;
	
    index = STP;
    uint8_t idx = map(scale, 5, 95, 0U, 6U);;
    colLine = gamma[idx];
    step = 0U;
}
void FieryLedLampEffectBamboo::updateInner()
{
	const byte posLine = (HEIGHT > 16) ? 4 : 3;
	for (int y = 0; y < HEIGHT + SY; y++) {
    	if (scale < 50U) {
      		if (step % 128 == 0U) {
        		deltaX += STP * ((direct) ? -1 : 1);
        		if ((deltaX > 1) | (deltaX < -1)) direct = !direct;
      		}
    	} else {
      		deltaX = 0;
    	}
    	posY = y;
    	for (int x = 0; x < WIDTH + SX; x++) {
      		if (y == posLine) {
        		drawPixelXYF(x , y - 1, CHSV(colLine, 255U, 128U));
        		drawPixelXYF(x, y, CHSV(colLine, 255U, 96U));
        		if (HEIGHT > 16) {
          			drawPixelXYF(x, y - 2, CHSV(colLine, 10U, 64U));
        		}
      		}
      		if ((x % SX == 0U) & (y % SY == 0U)) {
        		for (int i = 1; i < (SY - 3); i++) {
          			if (i < 3) {
            			posY = y - i + 1 - DELTA + index;
            			drawPixelXYF(x - 3 + deltaX, posY, CHSV(nextColor(posY, 96, colLine), 255U, 255 - V_STEP * i));
            			posY = y - i + index;
            			drawPixelXYF(x + deltaX, posY, CHSV(nextColor(posY, 96, colLine), 255U, 255 - VG_STEP * i));
          			}
          			posY = y - i - DELTA + index;
          			drawPixelXYF(x - 4 + deltaX, posY , CHSV(nextColor(posY, 96, colLine), 180U, 255 - V_STEP * i));
          			posY = y - i + 1 + index;
          			drawPixelXYF(x - 1 + deltaX, posY , CHSV(nextColor(posY, ((i == 1) ? 96 : 80), colLine), 255U, 255 - V_STEP * i));
        		}
      		}
    	}
    	step++;
  	}
  	if (index >= SY)  {
    	index = 0;
  	}
  	fadeToBlackBy(leds, NUM_LEDS, 60);
  	index += STP;
}

// ============= ЭФФЕКТ ВОЛНЫ ===============
// https://github.com/pixelmatix/aurora/blob/master/PatternWave.h
// Адаптация от (c) SottNick

byte waveThetaUpdate = 0;
byte waveThetaUpdateFrequency = 0;
byte waveTheta = 0;

byte hueUpdate = 0;
byte hueUpdateFrequency = 0;
//    byte hue = 0; будем использовать сдвиг от эффектов Радуга

uint8_t waveScale = 256 / WIDTH;

void FieryLedLampEffectWave::setup()
{
	//setCurrentPalette();//а вот тут явно накосячено. палитры наложены на угол поворота несинхронно, но исправлять особого смысла нет

    waveRotation = (scale % 11U) % 4U;
    waveCount = speed & 0x01;
}

void FieryLedLampEffectWave::updateInner()
{
	/*dimAll(254);
  	
	int n = 0;

  	switch (waveRotation)
	{
    case 0:
    	for (uint8_t x = 0; x < WIDTH; x++) {
        	n = quadwave8(x * 2 + waveTheta) / waveScale;
        	drawPixelXY(x, n, ColorFromPalette(*curPalette, hue + x));
        	if (waveCount != 1)
          		drawPixelXY(x, HEIGHT - 1 - n, ColorFromPalette(*curPalette, hue + x));
      	}
      	break;
    case 1:
      	for (uint8_t y = 0; y < HEIGHT; y++) {
        	n = quadwave8(y * 2 + waveTheta) / waveScale;
        	drawPixelXY(n, y, ColorFromPalette(*curPalette, hue + y));
        	if (waveCount != 1)
          		drawPixelXY(WIDTH - 1 - n, y, ColorFromPalette(*curPalette, hue + y));
      	}
      	break;
    case 2:
      	for (uint8_t x = 0; x < WIDTH; x++) {
        	n = quadwave8(x * 2 - waveTheta) / waveScale;
        	drawPixelXY(x, n, ColorFromPalette(*curPalette, hue + x));
        	if (waveCount != 1)
          		drawPixelXY(x, HEIGHT - 1 - n, ColorFromPalette(*curPalette, hue + x));
      	}
      	break;
    case 3:
      	for (uint8_t y = 0; y < HEIGHT; y++) {
        	n = quadwave8(y * 2 - waveTheta) / waveScale;
        	drawPixelXY(n, y, ColorFromPalette(*curPalette, hue + y));
        	if (waveCount != 1)
          		drawPixelXY(WIDTH - 1 - n, y, ColorFromPalette(*curPalette, hue + y));
      	}
      	break;
  	}
	
	if (waveThetaUpdate >= waveThetaUpdateFrequency) {
    	waveThetaUpdate = 0;
    	waveTheta++;
  	}
  	else {
    	waveThetaUpdate++;
  	}

	if (hueUpdate >= hueUpdateFrequency) {
    	hueUpdate = 0;
    	hue++;
  	}
  	else {
    	hueUpdate++;
  	}

	blurScreen(20); // @Palpalych советует делать размытие. вот в этом эффекте его явно не хватает...
	*/
}

// =====================================
//          Блуждающий кубик
// =====================================
void FieryLedLampEffectBall::setup()
{
	for (uint8_t i = 0U; i < 2U; i++) {
      coordB[i] = WIDTH / 2 * 10;
      vectorB[i] = random(8, 20);
    }
    // ballSize;
    deltaValue = map(scale * 2.55, 0U, 255U, 2U, max((uint8_t)min(WIDTH, HEIGHT) / 3, 4));
    ballColor = CHSV(random(0, 9) * 28, 255U, 255U);
    _pulse_color = CHSV(random(0, 9) * 28, 255U, 255U);
}

void FieryLedLampEffectBall::updateInner()
{
#define RANDOM_COLOR          (1U)                          // случайный цвет при отскоке
	if (scale & 0x01) {
    	for (uint8_t i = 0U; i < deltaValue; i++) {
      		for (uint8_t j = 0U; j < deltaValue; j++) {
        		leds[XY(coordB[0U] / 10 + i, coordB[1U] / 10 + j)] = _pulse_color;
      		}
    	}
  	}
  	
	for (uint8_t i = 0U; i < 2U; i++) {
    	coordB[i] += vectorB[i];
    	if (coordB[i] < 0) {
      		coordB[i] = 0;
      		vectorB[i] = -vectorB[i];
      		if (RANDOM_COLOR) ballColor = CHSV(random(0, 9) * 28, 255U, 255U); // if (RANDOM_COLOR && (modes[currentMode].Scale & 0x01))
      		//vectorB[i] += random(0, 6) - 3;
    	}
  	}
  	if (coordB[0U] > (int16_t)((WIDTH - deltaValue) * 10)) {
    	coordB[0U] = (WIDTH - deltaValue) * 10;
    	vectorB[0U] = -vectorB[0U];
    	if (RANDOM_COLOR) ballColor = CHSV(random(0, 9) * 28, 255U, 255U);
    	//vectorB[0] += random(0, 6) - 3;
  	}
  	if (coordB[1U] > (int16_t)((HEIGHT - deltaValue) * 10)) {
    	coordB[1U] = (HEIGHT - deltaValue) * 10;
    	vectorB[1U] = -vectorB[1U];
    	if (RANDOM_COLOR) ballColor = CHSV(random(0, 9) * 28, 255U, 255U);
    	//vectorB[1] += random(0, 6) - 3;
  	}
	FastLED.clear();

  	for (uint8_t i = 0U; i < deltaValue; i++) {
    	for (uint8_t j = 0U; j < deltaValue; j++) {
      		leds[XY(coordB[0U] / 10 + i, coordB[1U] / 10 + j)] = ballColor;
    	}
  	}
}

// =============- новый огонь / водопад -===============
// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 55, suggested range 20-100
#define COOLINGNEW 32
// 8  практически сплошной поток красивой подсвеченной воды ровным потоком сверху донизу. будто бы на столе стоит маленький "родничок"
// 20 ровный водопад с верщиной на свету, где потоки летящей воды наверху разбиваются ветром в белую пену
// 32 уже не ровный водопад, у которого струи воды долетают до земли неравномерно
// чем больше параметр, тем больше тени снизу
// 55 такое, как на видео

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKINGNEW 80 // 30 // 120 // 90 // 60
// 80 почти все белые струи сверху будут долетать до низа - хорошо при выбранном ползунке Масштаб = 100 (белая вода без подкрашивания)
// 50 чуть больше половины будет долетать. для цветных вариантов жидкости так более эффектно

void FieryLedLampEffectWaterfall::setup()
{
}
void FieryLedLampEffectWaterfall::updateInner()
{
	//    bool fire_water = modes[currentMode].Scale <= 50;
  	//    uint8_t COOLINGNEW = fire_water ? modes[currentMode].Scale * 2  + 20 : (100 - modes[currentMode].Scale ) *  2 + 20 ;
  	//    uint8_t COOLINGNEW = modes[currentMode].Scale * 2  + 20 ;
  	// Array of temperature readings at each simulation cell
  	//static byte heat[WIDTH][HEIGHT]; будет noise3d[0][WIDTH][HEIGHT]

  	for (uint8_t x = 0; x < WIDTH; x++) {
    	// Step 1.  Cool down every cell a little
    	for (uint8_t i = 0; i < HEIGHT; i++) {
      		noise3d[0][x][i] = qsub8(noise3d[0][x][i], random8(0, ((COOLINGNEW * 10) / HEIGHT) + 2));
    	}

    	// Step 2.  Heat from each cell drifts 'up' and diffuses a little
		for (uint8_t k = HEIGHT - 1; k >= 2; k--) {
			noise3d[0][x][k] = (noise3d[0][x][k - 1] + noise3d[0][x][k - 2] + noise3d[0][x][k - 2]) / 3;
		}

		// Step 3.  Randomly ignite new 'sparks' of heat near the bottom
		if (random8() < SPARKINGNEW) {
			uint8_t y = random8(2);
			noise3d[0][x][y] = qadd8(noise3d[0][x][y], random8(160, 255));
		}

		// Step 4.  Map from heat cells to LED colors
		for (uint8_t j = 0; j < HEIGHT; j++) {
			// Scale the heat value from 0-255 down to 0-240
			// for best results with color palettes.
			byte colorindex = scale8(noise3d[0][x][j], 240);
			//if (scale == 100)
			//	leds[XY(x, (HEIGHT - 1) - j)] = ColorFromPalette(WaterfallColors_p, colorindex);
			//else
				leds[XY(x, (HEIGHT - 1) - j)] = ColorFromPalette(CRGBPalette16( CRGB::Black, CHSV(scale * 2.57, 255U, 255U) , CHSV(scale * 2.57, 128U, 255U) , CRGB::White), colorindex);// 2.57 вместо 2.55, потому что 100 для белого цвета
		}
	}
}

// ============ Magic Lantern ===========
//             © SlingMaster
//            Волшебный Фонарик
// --------------------------------------
#define PADDING (HEIGHT * 0.25)
#define WARM_LIGHT 55U
#define STEP 4U
void FieryLedLampEffectMagicLantern::setup()
{
  uint8_t delta;
	deltaValue = 0;
    step = deltaValue;
    if (speed > 52) {
    	// brightness = 50 + modes[currentMode].Speed;
      	brightness = map(speed, 1, 255, 50U, 250U);
      	low_br = 50U;
    } else {
      	brightness = 0U;
    	low_br = 0U;
    }
    saturation = (scale > 50U) ? 64U : 0U;
    if (abs (70 - scale) <= 5) saturation = 170U;
    FastLED.clear();
}

void FieryLedLampEffectMagicLantern::updateInner()
{
	/*dimAll(170);
  	hue = (scale > 95) ? floor(step / 32) * 32U : scale * 2.55;

	for (uint8_t x = 0U; x < WIDTH + 1 ; x++) {
	    // light ---
    	if (low_br > 0) {
      		gradientVertical( x - deltaValue, CENTER_Y_MAJOR, x + 1U - deltaValue, HEIGHT - PADDING - 1,  WARM_LIGHT, WARM_LIGHT, brightness, low_br, saturation);
      		gradientVertical( WIDTH - x + deltaValue, CENTER_Y_MAJOR, WIDTH - x + 1U + deltaValue, HEIGHT - PADDING - 1,  WARM_LIGHT, WARM_LIGHT, brightness, low_br, saturation);
      		gradientVertical( x - deltaValue, PADDING + 1, x + 1U - deltaValue, CENTER_Y_MAJOR, WARM_LIGHT, WARM_LIGHT, low_br + 10, brightness, saturation);
      		gradientVertical( WIDTH - x + deltaValue, PADDING + 1, WIDTH - x + 1U + deltaValue, CENTER_Y_MAJOR, WARM_LIGHT, WARM_LIGHT, low_br + 10, brightness, saturation);
    	} else {
      		if (x % (STEP + 1) == 0) {
        		leds[XY(random8(WIDTH), random8(PADDING + 2, HEIGHT - PADDING - 2))] = CHSV(step - 32U, random8(128U, 255U), 255U);
      		}
      		if ((speed < 25) & (low_br == 0)) {
        		deltaValue = 0;
        		if (x % 2 != 0) {
          			gradientVertical( x - deltaValue, HEIGHT - PADDING, x + 1U - deltaValue, HEIGHT,  hue, hue + 2, 64U, 20U, 255U);
          			gradientVertical( (WIDTH - x + deltaValue), 0U,  (WIDTH - x + 1U + deltaValue), PADDING,  hue, hue, 42U, 64U, 255U);
        		}
        		//        deltaValue = 0;
      		}
    	}
    	if (x % STEP == 0) {
      		// body --
      		gradientVertical( x - deltaValue, HEIGHT - PADDING, x + 1U - deltaValue, HEIGHT,  hue, hue + 2, 255U, 20U, 255U);
      		gradientVertical( (WIDTH - x + deltaValue), 0U,  (WIDTH - x + 1U + deltaValue), PADDING,  hue, hue, 42U, 255U, 255U);
    	}
  	}
  	// ------
  	deltaValue++;
  	if (deltaValue >= STEP) {
    	deltaValue = 0;
  	}
	step++;*/
}

// =============== Wine ================
//    © SlingMaster | by Alex Dovby
//               EFF_WINE
//--------------------------------------
void FieryLedLampEffectWine::setup()
{
    fillAll(CHSV(55U, 255U, 65U));
    // minspeed 230 maxspeed 250 ============
    // minscale  40 maxscale  75 ============
    // красное вино hue > 0 & <=10
    // розовое вино hue > 10 & <=20
    // белое вино   hue > 20U & <= 40
    // шампанское   hue > 40U & <= 60

    deltaHue2 = 0U;                         // count для замедления смены цвета
    step = 255U - speed + 1U;                      // чтообы при старте эффекта сразу покрасить лампу
    deltaHue = 1U;                          // direction | 0 hue-- | 1 hue++ |
    hue = 55U;                              // Start Color
    hue2 = 65U;                             // Brightness
    pcnt = 0;
}

void FieryLedLampEffectWine::updateInner()
{
	deltaHue2++;
  	// маштаб задает скорость изменения цвета 5 уровней
  	uint8_t divider = 5 - floor((scale - 1) / 20);

  	// возвращаем яркость для перехода к белому
  	if (hue >= 10 && hue2 < 100U) {
    	hue2++;
  	}
  	// уменьшаем яркость для красного вина
  	if (hue < 10 && hue2 > 40U) {
    	hue2--;
  	}
	// изменение цвета вина -----
  	if (deltaHue == 1U) {
    	if (deltaHue2 % divider == 0) {
      		hue++;
    	}
  	} else {
    	if (deltaHue2 % divider == 0) {
      		hue--;
    	}
  	}
  	// --------
	// сдвигаем всё вверх -----------
  	for (uint8_t x = 0U; x < WIDTH; x++) {
    	for (uint8_t y = HEIGHT; y > 0U; y--) {
      		drawPixelXY(x, y, getPixColorXY(x, y - 1U));
    	}
  	}

  	if (hue > 40U) {
    	// добавляем перляж для шампанского
    	pcnt = random(0, WIDTH);
  	} else {
    	pcnt = 0;
  	}
	// заполняем нижнюю строку с учетом перляжа
  	for (uint8_t x = 0U; x < WIDTH; x++) {
    	if ((x == pcnt) && (pcnt > 0)) {
      		// с перляжем ------
      		drawPixelXY(x, 0U, CHSV(hue, 150U, hue2 + 20U + random(0, 50U)));
    	} else {
      		drawPixelXY(x, 0U, CHSV(hue, 255U, hue2));
    	}
  	}
  	// меняем направление изменения цвета вина от красного к шампанскому и обратно
  	// в диапазоне шкалы HUE |0-60|
  	if  (hue == 0U) {
    	deltaHue = 1U;
  	}
  	if (hue == 60U) {
    	deltaHue = 0U;
  	}
  	step++;
}

void FieryLedLampEffectMadnessNoise::setup()
{
	//scale = modes[currentMode].Scale;
	//speed = modes[currentMode].Speed;
}

#define MAX_DIMENSION (max(WIDTH, HEIGHT))
void FieryLedLampEffectMadnessNoise::fillnoise8()
{
	for (uint8_t i = 0; i < MAX_DIMENSION; i++)
  	{
    	int32_t ioffset = scale * i;
    	for (uint8_t j = 0; j < MAX_DIMENSION; j++)
    	{
      		int32_t joffset = scale * j;
      		noise[i][j] = inoise8(x + ioffset, y + joffset, z);
    	}
  	}
  	z += speed;
}

void FieryLedLampEffectMadnessNoise::updateInner()
{
	fillnoise8();
  	for (uint8_t i = 0; i < WIDTH; i++)
  	{
    	for (uint8_t j = 0; j < HEIGHT; j++)
    	{
      		CRGB thisColor = CHSV(noise[j][i], 255, noise[i][j]);
      		drawPixelXY(i, j, thisColor);                         //leds[XY(i, j)] = CHSV(noise[j][i], 255, noise[i][j]);
    	}
  	}
  	//ihue += 1;
}

// ============= ЭФФЕКТ ВИХРИ ===============
// https://github.com/pixelmatix/aurora/blob/master/PatternFlowField.h
// Адаптация (c) SottNick
// используются переменные эффекта Стая. Без него работать не будет.

static const uint8_t ff_speed = 1; // чем выше этот параметр, тем короче переходы (градиенты) между цветами. 1 - это самое красивое
static const uint8_t ff_scale = 26; // чем больше этот параметр, тем больше "языков пламени" или как-то так. 26 - это норм

void FieryLedLampEffectWhirl::setup()
{
	//setCurrentPalette();

    ff_x = random16();
    ff_y = random16();
    ff_z = random16();

    for (uint8_t i = 0; i < AVAILABLE_BOID_COUNT; i++) {
    	boids[i] = Boid(random8(WIDTH), 0);
    }
}
void FieryLedLampEffectWhirl::updateInner()
{
	dimAll(240);

  	for (uint8_t i = 0; i < AVAILABLE_BOID_COUNT; i++) {
    	Boid * boid = &boids[i];

    	int ioffset = ff_scale * boid->location.x;
    	int joffset = ff_scale * boid->location.y;

    	byte angle = inoise8(ff_x + ioffset, ff_y + joffset, ff_z);

    	boid->velocity.x = (float) sin8(angle) * 0.0078125 - 1.0;
    	boid->velocity.y = -((float)cos8(angle) * 0.0078125 - 1.0);
    	boid->update();

    	if (oneColor)	
      		drawPixelXYF(boid->location.x, boid->location.y, CHSV(scale * 2.55, (scale == 100) ? 0U : 255U, 255U)); // цвет белый для .Scale=100
    	//else
      	//	drawPixelXYF(boid->location.x, boid->location.y, ColorFromPalette(*curPalette, angle + hue)); // + hue постепенно сдвигает палитру по кругу

    	if (boid->location.x < 0 || boid->location.x >= WIDTH || boid->location.y < 0 || boid->location.y >= HEIGHT) {
      		boid->location.x = random(WIDTH);
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