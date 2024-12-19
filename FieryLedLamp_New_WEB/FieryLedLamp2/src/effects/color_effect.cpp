#include"effect.h"
#include"effects/color_effect.h"

#define BORDERLAND   2 // две дополнительные единицы бегунка Масштаб на границе вертикального и горизонтального варианта эффекта (с каждой стороны границы) будут для света всеми светодиодами в полную силу
void FieryLedLampEffectWhiteColorStripeRoutine::setup()
{
	uint8_t thisSize = LED_HEIGHT;
	uint8_t halfScale = scale;
	if (halfScale > 50U)
	{
		thisSize = LED_WIDTH;
		halfScale = 101U - halfScale;
	}
	halfScale = constrain(halfScale, 0U, 50U - BORDERLAND);

	uint8_t center = thisSize / 2 - 1;
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

		if (scale <= 50)
		{
			for (uint8_t x = 0; x < LED_WIDTH; x++)
			{
				drawPixelXY(x, i, color);
				drawPixelXY(x, LED_HEIGHT + offset - i - 2, color);
			}
		}
		else
			for (uint8_t y = 0; y < LED_HEIGHT; y++)
			{
				drawPixelXY((i + speed - 1) % LED_WIDTH, y, color);
				drawPixelXY((LED_WIDTH + offset - i + speed - 3) % LED_WIDTH, y, color);
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
void FieryLedLampEffectWaterColor::SmearPaint()
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

	if (direction == 1) {  // direction >>>
    	x++;
    	if (x >= end_x) {
      		direction = 0;     // swap direction
      		y--;                     // new line
      		if (step % 2 == 0) {
        		x++;
      		} else {
        		x--;
      		}
      		width--;
    	}
  	} else {                          // direction <<<
    	x--;
    	if (x <= (end_x - width)) {
      		direction = 1;     // swap direction
      		y--;                     // new line
      		if (width >= 1) {
        		temp = width - 1;
        		if (temp < 0) {
          			temp = 0;
        		}
        		width = temp;
        		x++;
      		}
    	}
  	}

  	if (y == 255) {
    	lastColor = true;
  	}

  	divider = floor((scale - 1) / 16.7);
  	if( (x >= LED_WIDTH) || (y == end_y) ) {
    	// deltaHue value == 255 activate -------
    	// set new parameter for new smear ------
    	lastColor = true;
  	}
	drawPixelXY(x, y, colors[divider][hue]);
}

void FieryLedLampEffectWaterColor::setup()
{
	deltaValue = map(speed, 0, 255, 1, 255);
    step = deltaValue; // чтообы при старте эффекта сразу покрасить лампу
    hue = 0;
	lastColor = true; // last color
	x=LED_WIDTH/4;
	y=LED_HEIGHT/4;
}
void FieryLedLampEffectWaterColor::updateInner()
{
	if (step >= deltaValue) {
    	step = 0U;
  	}

	if (lastColor)
	{
    	width = 4 + random8(LED_WIDTH/4);                // width
    	x = random8(LED_WIDTH - width);           // x
    	uint8_t temp =  x + width;
    	if (temp >= (LED_WIDTH - 1)) {
      		temp = LED_WIDTH - 1;
      		if (x > 1) {
        		x--;
      		} else {
        		x++;
      		}
    	}
		end_x = temp;                                            // x end
		y = 3 + random8(LED_HEIGHT - 4);                         // y
		temp = y - random8(3) - 3;
    	if (temp <= 0) {
      		temp = 0;
    	}
    	end_y = temp;                                            // y end
    	direction = 1;
    	hue = random8(8);
    	lastColor=false;
  	}
  	SmearPaint();
  	if (step % 2 == 0) {
    	blurScreen(beatsin8(1U, 1U, 6U));
  	}
	step++;
}

// ------------- пейнтбол -------------
#define BORDERTHICKNESS (1U) // глубина бордюра для размытия яркой частицы: 0U - без границы (резкие края); 1U - 1 пиксель (среднее размытие) ; 2U - 2 пикселя (глубокое размытие)

void FieryLedLampEffectPaintball::setup()
{
	scale = map(scale, 0, 255, 1, 255);
};
void FieryLedLampEffectPaintball::updateInner()
{
	const uint8_t paintWidth = LED_WIDTH - BORDERTHICKNESS * 2;
	const uint8_t paintHeight = LED_HEIGHT - BORDERTHICKNESS * 2;
	// Apply some blurring to whatever's already on the matrix
	// Note that we never actually clear the matrix, we just constantly
	// blur it repeatedly. Since the blurring is 'lossy', there's
	// an automatic trend toward black -- by design.
	// uint8_t blurAmount = dim8_raw(beatsin8(3, 64, 100));
	// blur2d(leds, WIDTH, HEIGHT, blurAmount);
	blurScreen(dim8_raw(beatsin8(3, 64, 100)));

	// Use two out-of-sync sine waves
	uint16_t i = beatsin16(79, 0, 255); // 91
	uint16_t j = beatsin16(67, 0, 255); // 109
	uint16_t k = beatsin16(53, 0, 255); // 73
	uint16_t m = beatsin16(97, 0, 255); // 123

	// The color of each point shifts over time, each at a different speed.
	uint32_t ms = millis() / (scale / 4 + 1);
	leds[XY(highByte(i * paintWidth) + BORDERTHICKNESS, highByte(j * paintHeight) + BORDERTHICKNESS)] += CHSV(ms / 29, 200U, 255U);
	leds[XY(highByte(j * paintWidth) + BORDERTHICKNESS, highByte(k * paintHeight) + BORDERTHICKNESS)] += CHSV(ms / 41, 200U, 255U);
	leds[XY(highByte(k * paintWidth) + BORDERTHICKNESS, highByte(m * paintHeight) + BORDERTHICKNESS)] += CHSV(ms / 37, 200U, 255U);
	leds[XY(highByte(m * paintWidth) + BORDERTHICKNESS, highByte(i * paintHeight) + BORDERTHICKNESS)] += CHSV(ms / 53, 200U, 255U);
}

// ============ Night City =============
//             © SlingMaster
//              Ночной Город
// =====================================
void FieryLedLampEffectNightCity::setup()
{
	const byte PADDING = LED_HEIGHT * 0.13;
	hue = 64;
	step=0;
	for (uint16_t i = 0; i < LED_WIDTH; i++)
	{
		noise[i][0] = PADDING + 2;
		noise[i][1] = PADDING + 3;
	}
};
void FieryLedLampEffectNightCity::updateInner()
{
	const byte PADDING = LED_HEIGHT * 0.13;
	// ---------------------

	byte xx = random8(LED_WIDTH);
	byte yy = random8(LED_HEIGHT);
	byte fade = 80; // 60 - abs(128 - step) / 3;
	fadeToBlackBy(leds, NUM_LEDS, fade);

	// -----------------
	for (uint16_t y = 0; y < LED_HEIGHT; y++)
	{
		for (uint16_t x = 0; x < LED_WIDTH; x++)
		{
			if (y > PADDING)
			{
				if (x % 6U == 0U)
				{
					/* draw Elevator */
					leds[XY(x, noise[x][1])] = CHSV(160, 255U, 255U);
				}
				else
				{
					/* draw light ------- */
					// if ((x % 2U == 0U) & (y % 2U == 0U)) {
					bool flag = (scale > 50U) ? true : x % 2 == 0;
					if (flag & (y % 2 == 0))
					{
						if ((x == xx) & (y == yy))
						{
							/* change light */
							if (noise[x][y] == 0)
							{
								noise[x][y] = random8(1, 5);
								if (speed > 80)
								{
									noise[random8(LED_WIDTH)][random8(PADDING + 1, LED_HEIGHT - 1)] = 6;
								}
								if (speed > 160)
								{
									noise[random8(LED_WIDTH)][random8(PADDING + 1, LED_HEIGHT - 1)] = 6;
								}
							}
							else
							{
								noise[x][y] = 0;
							}
						}
						if (speed > 250)
						{
							noise[x][y] = 2;
						}
						/* draw light ----- */
						if (noise[x][y] > 0)
						{
							if (noise[x][y] == 1U)
							{
								leds[XY(x, y)] = CHSV(32U, 200U, 255U);
							}
							else
							{
								leds[XY(x, y)] = CHSV(128U, 32U, 255U);
							}
						}
					}
				}
			}
			else
			{
				/* draw the lower floors */
				if (y == PADDING)
				{
					leds[XY(x, y)] = CHSV(hue, 255U, 255U);
				}
				else
				{
					leds[XY(x, y)] = CHSV(96U, 128U, 80U + y * 32);
				}
			}
		}
	}

	/* change elevators position */
	if (step % 4U == 0U)
	{
		for (uint16_t i = 0; i < LED_WIDTH; i++)
		{
			if (i % 6U == 0U)
			{
				/* 1 current floor */
				if (noise[i][0] > noise[i][1])
					noise[i][1]++;
				if (noise[i][0] < noise[i][1])
					noise[i][1]--;
			}
		}
	}

	/* 0 set target floor ----- */
	if (step % 128U == 0U)
	{
		for (uint16_t i = 0; i < LED_WIDTH; i++)
		{
			if (i % 6U == 0U)
			{
				/* 0 target floor ----- */
				byte target_floor = random8(PADDING + 1, LED_HEIGHT - 1);
				if (target_floor % 2U)
					target_floor++;
				noise[i][0] = target_floor;
			}
		}
	}

	hue++;
	step++;
}

// ============ ЭФФЕКТЫ ПИКАССО =============
// взято откуда-то by @obliterator или им написано
// https://github.com/DmytroKorniienko/FireLamp_JeeUI/blob/templ/src/effects.cpp

//вместо класса Particle будем повторно использовать переменные из эффекта мячики и мотыльки
//        float position_x = 0;
//float trackingObjectPosX[enlargedOBJECT_MAX_COUNT];
//        float position_y = 0;
//float trackingObjectPosY[enlargedOBJECT_MAX_COUNT];
//        float speed_x = 0;
////float trackingObjectSpeedY[enlargedOBJECT_MAX_COUNT];                   // As time goes on the impact velocity will change, so make an array to store those values
//        float speed_y = 0;
////float trackingObjectShift[enlargedOBJECT_MAX_COUNT];                       // Coefficient of Restitution (bounce damping)
//        CHSV color;
////uint8_t trackingObjectHue[enlargedOBJECT_MAX_COUNT];
//        uint8_t hue_next = 0;
//uint8_t trackingObjectState[enlargedOBJECT_MAX_COUNT] ;                       // прикручено при адаптации для распределения мячиков по радиусу лампы
//        int8_t hue_step = 0;
//float   trackingObjectSpeedX[trackingOBJECT_MAX_COUNT];                       // The integer position of the dot on the strip (LED index)

void FieryLedLampEffectPicasso::PicassoGenerate(bool reset)
{
	for (uint8_t i = 0; i < enlargedObjectNUM; i++)
	{
		if (reset)
		{
			trackingObjectState[i] = random8();
			trackingObjectSpeedX[i] = (trackingObjectState[i] - trackingObjectHue[i]) / 25;
		}
		if (trackingObjectState[i] != trackingObjectHue[i] && trackingObjectSpeedX[i])
		{
			trackingObjectHue[i] += trackingObjectSpeedX[i];
		}
	}
}

void FieryLedLampEffectPicasso::PicassoPosition()
{
	for (uint8_t i = 0; i < enlargedObjectNUM; i++)
	{
		if ((trackingObjectPosX[i] + trackingObjectSpeedY[i]) > (LED_WIDTH-1) || trackingObjectPosX[i] + trackingObjectSpeedY[i] < 0)
		{
			trackingObjectSpeedY[i] = -trackingObjectSpeedY[i];
		}

		if (trackingObjectPosY[i] + trackingObjectShift[i] >= LED_HEIGHT-1 || trackingObjectPosY[i] + trackingObjectShift[i] < 0)
		{
			trackingObjectShift[i] = -trackingObjectShift[i];
		}

		trackingObjectPosX[i] += trackingObjectSpeedY[i];
		trackingObjectPosY[i] += trackingObjectShift[i];
	};
}

void FieryLedLampEffectPicasso::PicassoRoutine()
{
	PicassoGenerate(false);
	PicassoPosition();

	for (uint8_t i = 0; i < enlargedObjectNUM - 2U; i += 2)
		DrawLine(trackingObjectPosX[i], trackingObjectPosY[i], trackingObjectPosX[i + 1U], trackingObjectPosY[i + 1U], CHSV(trackingObjectHue[i], 255U, 255U));

	EVERY_N_MILLIS(20000)
	{
		PicassoGenerate(true);
	}

	blurScreen(80);
}

void FieryLedLampEffectPicasso::PicassoRoutine2()
{
	PicassoGenerate(false);
	PicassoPosition();
	dimAll(180);

	for (uint8_t i = 0; i < enlargedObjectNUM - 1U; i++)
		DrawLineF(trackingObjectPosX[i], trackingObjectPosY[i], trackingObjectPosX[i + 1U], trackingObjectPosY[i + 1U], CHSV(trackingObjectHue[i], 255U, 255U));

	EVERY_N_MILLIS(20000)
	{
		PicassoGenerate(true);
	}

	blurScreen(80);
}

void FieryLedLampEffectPicasso::PicassoRoutine3()
{
	PicassoGenerate(false);
	PicassoPosition();
	dimAll(180);

	for (uint8_t i = 0; i < enlargedObjectNUM - 2U; i += 2)
		drawCircleF(fabs(trackingObjectPosX[i] - trackingObjectPosX[i + 1U]), fabs(trackingObjectPosY[i] - trackingObjectPosX[i + 1U]), fabs(trackingObjectPosX[i] - trackingObjectPosY[i]), CHSV(trackingObjectHue[i], 255U, 255U));

	EVERY_N_MILLIS(20000)
	{
		PicassoGenerate(true);
	}

	blurScreen(80);
}

void FieryLedLampEffectPicasso::setup()
{
	if (scale < 34U)           // если масштаб до 34
		enlargedObjectNUM = (scale - 1U) / 32.0 * (enlargedOBJECT_MAX_COUNT - 3U) + 3U;
	else
	{
		if (scale >= 68U) // если масштаб больше 67
			enlargedObjectNUM = (scale - 68U) / 32.0 * (enlargedOBJECT_MAX_COUNT - 3U) + 3U;
		else // для масштабов посередине
			enlargedObjectNUM = (scale - 34U) / 33.0 * (enlargedOBJECT_MAX_COUNT - 1U) + 1U;
	}

	if (enlargedObjectNUM > enlargedOBJECT_MAX_COUNT)
		enlargedObjectNUM = enlargedOBJECT_MAX_COUNT;
	if (enlargedObjectNUM < 2U)
		enlargedObjectNUM = 2U;

	double minSpeed = 0.2, maxSpeed = 0.8;

	for (uint8_t i = 0; i < enlargedObjectNUM; i++)
	{
		trackingObjectPosX[i] = random8(LED_WIDTH);
		trackingObjectPosY[i] = random8(LED_HEIGHT);

		// curr->color = CHSV(random(1U, 255U), 255U, 255U);
		trackingObjectHue[i] = random8();

		trackingObjectSpeedY[i] = +((-maxSpeed / 3) + (maxSpeed * (float)random8(1, 100) / 100));
		trackingObjectSpeedY[i] += trackingObjectSpeedY[i] > 0 ? minSpeed : -minSpeed;

		trackingObjectShift[i] = +((-maxSpeed / 2) + (maxSpeed * (float)random8(1, 100) / 100));
		trackingObjectShift[i] += trackingObjectShift[i] > 0 ? minSpeed : -minSpeed;

		trackingObjectState[i] = trackingObjectHue[i];
	}
};
void FieryLedLampEffectPicasso::updateInner()
{
	if (scale < 34U) // если масштаб до 34
		PicassoRoutine();
	else if (scale > 67U) // если масштаб больше 67
		PicassoRoutine3();
	else // для масштабов посередине
		PicassoRoutine2();
}

// ------------- цвет ------------------
void FieryLedLampEffectColor::setup()
{
};
void FieryLedLampEffectColor::updateInner()
{
    /*if (scale < 3 && RuninTextOverEffects)
        FastLED.clear();
    else*/
        fillAll(CHSV(scale, speed, 255U));
}