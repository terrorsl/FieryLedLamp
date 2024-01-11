#include"effect.h"

CRGB leds[NUM_LEDS];

// стандартные функции библиотеки LEDraw от @Palpalych (для адаптаций его эффектов)
void blurScreen(fract8 blur_amount, CRGB *LEDarray = leds)
{
	blur2d(LEDarray, WIDTH, HEIGHT, blur_amount);
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
		if(millis() - effectTimer < 256-speed)
			return;
		break;
	}
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

void FieryLedLampEffectWhiteColorStripeRoutine::setup()
{
	/*FastLED.clear();

	uint8_t thisSize = HEIGHT;
	uint8_t halfScale = modes[currentMode].Scale;
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
			45U,                                                                              // ���������� ���
			map(modes[currentMode].Speed, 0U, 255U, 0U, 170U),                                // ���������� ������������
			i > (center - fullFill - 1)                                                       // ���������� �������
			? 255U                                                                            // ��� ����������� �������������� �����
			: iPol * (i > center - fullFill - 2));  // ��� ��������� �������������� ����� ������� ����� ���� 255, ���� 0 � ����������� �� ��������

		if (modes[currentMode].Scale <= 50U)
			for (uint8_t x = 0U; x < WIDTH; x++)
			{
				drawPixelXY(x, i, color);                         // ��� ������ ������ ������� ����������� ������ ���������� 2 ����������� �������������� ������
				drawPixelXY(x, HEIGHT + offset - i - 2U, color);  // ��� �������� - ����, �� ������
			}
		else
			for (uint8_t y = 0U; y < HEIGHT; y++)
			{
				drawPixelXY((i + modes[currentMode].Speed - 1U) % WIDTH, y, color);                    // ��� ������ ������ ������� ����������� ������ ���������� 2 ����������� ������������ ������
				drawPixelXY((WIDTH + offset - i + modes[currentMode].Speed - 3U) % WIDTH, y, color);   // ��� �������� - ����, �� ������
			}
	}*/
};
void FieryLedLampEffectWhiteColorStripeRoutine::updateInner()
{
	#if 0
	if (loadingFlag)
	{
#if defined(USE_RANDOM_SETS_IN_APP) || defined(RANDOM_SETTINGS_IN_CYCLE_MODE)
		if (selectedSettings) {
			setModeSettings(11U + random8(83U), 1U + random8(255U / WIDTH + 1U) * WIDTH);
		}
#endif //#if defined(USE_RANDOM_SETS_IN_APP) || defined(RANDOM_SETTINGS_IN_CYCLE_MODE)

		loadingFlag = false;
		FastLED.clear();

		uint8_t thisSize = HEIGHT;
		uint8_t halfScale = modes[currentMode].Scale;
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
				45U,                                                                              // ���������� ���
				map(modes[currentMode].Speed, 0U, 255U, 0U, 170U),                                // ���������� ������������
				i > (center - fullFill - 1)                                                       // ���������� �������
				? 255U                                                                            // ��� ����������� �������������� �����
				: iPol * (i > center - fullFill - 2));  // ��� ��������� �������������� ����� ������� ����� ���� 255, ���� 0 � ����������� �� ��������

			if (modes[currentMode].Scale <= 50U)
				for (uint8_t x = 0U; x < WIDTH; x++)
				{
					drawPixelXY(x, i, color);                         // ��� ������ ������ ������� ����������� ������ ���������� 2 ����������� �������������� ������
					drawPixelXY(x, HEIGHT + offset - i - 2U, color);  // ��� �������� - ����, �� ������
				}
			else
				for (uint8_t y = 0U; y < HEIGHT; y++)
				{
					drawPixelXY((i + modes[currentMode].Speed - 1U) % WIDTH, y, color);                    // ��� ������ ������ ������� ����������� ������ ���������� 2 ����������� ������������ ������
					drawPixelXY((WIDTH + offset - i + modes[currentMode].Speed - 3U) % WIDTH, y, color);   // ��� �������� - ����, �� ������
				}
		}
	}
	#endif
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