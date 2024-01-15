#include"effect.h"

CRGB leds[NUM_LEDS];

// дополнительные палитры для пламени
// для записи в PROGMEM преобразовывал из 4 цветов в 16 на сайте https://colordesigner.io/gradient-generator, но не уверен, что это эквивалент CRGBPalette16()
// значения цветовых констант тут: https://github.com/FastLED/FastLED/wiki/Pixel-reference
extern const TProgmemRGBPalette16 WoodFireColors_p FL_PROGMEM = {CRGB::Black, 0x330e00, 0x661c00, 0x992900, 0xcc3700, CRGB::OrangeRed, 0xff5800, 0xff6b00, 0xff7f00, 0xff9200, CRGB::Orange, 0xffaf00, 0xffb900, 0xffc300, 0xffcd00, CRGB::Gold};             //* Orange
extern const TProgmemRGBPalette16 NormalFire_p FL_PROGMEM = {CRGB::Black, 0x330000, 0x660000, 0x990000, 0xcc0000, CRGB::Red, 0xff0c00, 0xff1800, 0xff2400, 0xff3000, 0xff3c00, 0xff4800, 0xff5400, 0xff6000, 0xff6c00, 0xff7800};                             // пытаюсь сделать что-то более приличное
extern const TProgmemRGBPalette16 NormalFire2_p FL_PROGMEM = {CRGB::Black, 0x560000, 0x6b0000, 0x820000, 0x9a0011, CRGB::FireBrick, 0xc22520, 0xd12a1c, 0xe12f17, 0xf0350f, 0xff3c00, 0xff6400, 0xff8300, 0xffa000, 0xffba00, 0xffd400};                      // пытаюсь сделать что-то более приличное
extern const TProgmemRGBPalette16 LithiumFireColors_p FL_PROGMEM = {CRGB::Black, 0x240707, 0x470e0e, 0x6b1414, 0x8e1b1b, CRGB::FireBrick, 0xc14244, 0xd16166, 0xe08187, 0xf0a0a9, CRGB::Pink, 0xff9ec0, 0xff7bb5, 0xff59a9, 0xff369e, CRGB::DeepPink};        //* Red
extern const TProgmemRGBPalette16 SodiumFireColors_p FL_PROGMEM = {CRGB::Black, 0x332100, 0x664200, 0x996300, 0xcc8400, CRGB::Orange, 0xffaf00, 0xffb900, 0xffc300, 0xffcd00, CRGB::Gold, 0xf8cd06, 0xf0c30d, 0xe9b913, 0xe1af1a, CRGB::Goldenrod};           //* Yellow
extern const TProgmemRGBPalette16 CopperFireColors_p FL_PROGMEM = {CRGB::Black, 0x001a00, 0x003300, 0x004d00, 0x006600, CRGB::Green, 0x239909, 0x45b313, 0x68cc1c, 0x8ae626, CRGB::GreenYellow, 0x94f530, 0x7ceb30, 0x63e131, 0x4bd731, CRGB::LimeGreen};     //* Green
extern const TProgmemRGBPalette16 AlcoholFireColors_p FL_PROGMEM = {CRGB::Black, 0x000033, 0x000066, 0x000099, 0x0000cc, CRGB::Blue, 0x0026ff, 0x004cff, 0x0073ff, 0x0099ff, CRGB::DeepSkyBlue, 0x1bc2fe, 0x36c5fd, 0x51c8fc, 0x6ccbfb, CRGB::LightSkyBlue};  //* Blue
extern const TProgmemRGBPalette16 RubidiumFireColors_p FL_PROGMEM = {CRGB::Black, 0x0f001a, 0x1e0034, 0x2d004e, 0x3c0068, CRGB::Indigo, CRGB::Indigo, CRGB::Indigo, CRGB::Indigo, CRGB::Indigo, CRGB::Indigo, 0x3c0084, 0x2d0086, 0x1e0087, 0x0f0089, CRGB::DarkBlue};        //* Indigo
extern const TProgmemRGBPalette16 PotassiumFireColors_p FL_PROGMEM = {CRGB::Black, 0x0f001a, 0x1e0034, 0x2d004e, 0x3c0068, CRGB::Indigo, 0x591694, 0x682da6, 0x7643b7, 0x855ac9, CRGB::MediumPurple, 0xa95ecd, 0xbe4bbe, 0xd439b0, 0xe926a1, CRGB::DeepPink}; //* Violet
const TProgmemRGBPalette16 *firePalettes[] = {
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

float fmap(const float x, const float in_min, const float in_max, const float out_min, const float out_max) {
  return (out_max - out_min) * (x - in_min) / (in_max - in_min) + out_min;
}
float mapcurve(const float x, const float in_min, const float in_max, const float out_min, const float out_max, float (*curve)(float, float, float, float)) {
  if (x <= in_min) return out_min;
  if (x >= in_max) return out_max;
  return curve((x - in_min), out_min, (out_max - out_min), (in_max - in_min));
}
float InQuad(float t, float b, float c, float d) {
  t /= d;
  return c * t * t + b;
}
float OutQuart(float t, float b, float c, float d) {
  t = t / d - 1;
  return -c * (t * t * t * t - 1) + b;
}
float InOutQuad(float t, float b, float c, float d) {
  t /= d / 2;
  if (t < 1) return c / 2 * t * t + b;
  --t;
  return -c / 2 * (t * (t - 2) - 1) + b;
}

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
	
	if(!(THIS_Y & 0x01) || MATRIX_TYPE)               // Even rows run forwards
		return (THIS_Y * _WIDTH + THIS_X);
	else                                                  
		return (THIS_Y * _WIDTH + _WIDTH - THIS_X - 1);  // Odd rows run backwards
}
	
// стандартные функции библиотеки LEDraw от @Palpalych (для адаптаций его эффектов)
void blurScreen(fract8 blur_amount, CRGB *LEDarray = leds)
{
	blur2d(LEDarray, WIDTH, HEIGHT, blur_amount);
}
void dimAll(uint8_t value, CRGB *LEDarray = leds)
{
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
uint32_t FieryLedLampEffect::getPixColorXY(uint8_t x, uint8_t y)
{
	return getPixColor(XY(x, y));
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
void FieryLedLampEffect::FillNoise(uint8_t **noise3d, uint32_t noise32_x, uint32_t noise32_y, uint32_t noise32_z, uint32_t scale32_x, uint32_t scale32_y, uint8_t noisesmooth)
{
	for (uint8_t i = 0; i < WIDTH; i++) {
    	int32_t ioffset = scale32_x * (i - CENTER_X_MINOR);
		for (uint8_t j = 0; j < HEIGHT; j++) {
			int32_t joffset = scale32_y * (j - CENTER_Y_MINOR);
			int8_t data = inoise16(noise32_x + ioffset, noise32_y + joffset, noise32_z) >> 8;
			int8_t olddata = noise3d[i][j];
			int8_t newdata = scale8( olddata, noisesmooth ) + scale8( data, 255 - noisesmooth );
			data = newdata;
			noise3d[i][j] = data;
		}
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
  	direct = false;
	
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
	setCurrentPalette();//а вот тут явно накосячено. палитры наложены на угол поворота несинхронно, но исправлять особого смысла нет

    waveRotation = (scale % 11U) % 4U;
    waveCount = speed & 0x01;
}

void FieryLedLampEffectWave::updateInner()
{
	dimAll(254);
  	
	int n = 0;

  	/*switch (waveRotation)
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

uint8_t validMinMax(float val, uint8_t minV, uint32_t maxV)
{
	uint8_t result;
  	if (val <= minV) {
    	result = minV;
  	} else if (val >= maxV) {
    	result = maxV;
  	} else {
    	result = ceil(val);
  	}
  	return result;
}
//--------------------------------------
void FieryLedLampEffectMagicLantern::gradientVertical(uint8_t startX, uint8_t startY, uint8_t endX, uint8_t endY, uint8_t start_color, uint8_t end_color, uint8_t start_br, uint8_t end_br, uint8_t saturate) {
  float step_color = 0;
  float step_br = 0;
  if (startX == endX) {
    endX++;
  }
  if (startY == endY) {
    endY++;
  }
  step_color = (end_color - start_color) / abs(startY - endY);

  if (start_color >  end_color) {
    step_color -= 1.2;
  } else {
    step_color += 1.2;
  }

  step_br = (end_br - start_br) / abs(startY - endY);
  if (start_br >  end_color) {
    step_br -= 1.2;
  } else {
    step_br += 1.2;
  }
  for (uint8_t y = startY; y < endY; y++) {
    CHSV thisColor = CHSV( (uint8_t) validMinMax((start_color + (y - startY) * step_color), 0, 255), saturate,
                           (uint8_t) validMinMax((start_br + (y - startY) * step_br), 0, 255) );
    for (uint8_t x = startX; x < endX; x++) {
      drawPixelXY(x, y, thisColor);
    }
  }
}

void FieryLedLampEffectMagicLantern::updateInner()
{
	dimAll(170);
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
	step++;
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
	setCurrentPalette();

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

uint8_t wrapX(int8_t x) {
  return (x + WIDTH) % WIDTH;
}
uint8_t wrapY(int8_t y) {
  return (y + HEIGHT) % HEIGHT;
}
void FieryLedLampEffect::fadePixel(uint8_t i, uint8_t j, uint8_t step)          // новый фейдер
{
  	int32_t pixelNum = XY(i, j);
  	if (getPixColor(pixelNum) == 0U)
		return;

  	if (leds[pixelNum].r >= 30U ||
	    leds[pixelNum].g >= 30U ||
    	leds[pixelNum].b >= 30U)
  	{
    	leds[pixelNum].fadeToBlackBy(step);
  	}
  	else
  	{
    	leds[pixelNum] = 0U;
  	}
}

// ------------- метель - 2 -------------
//SNOWSTORM / МЕТЕЛЬ # STARFALL / ЗВЕЗДОПАД ***** V1.2
// v1.0 - Updating for GuverLamp v1.7 by PalPalych 12.03.2020
// v1.1 - Fix wrong math & full screen drawing by PalPalych 14.03.2020
// v1.2 - Code optimisation + pseudo 3d by PalPalych 21.04.2020
#define e_sns_DENSE (32U) // плотность снега - меньше = плотнее

void FieryLedLampEffectStarFall::setup()
{
}
void FieryLedLampEffectStarFall::updateInner()
{
	bool isColored = speed & 0x01; // сворачиваем 2 эффекта в 1
  	// заполняем головами комет
  	uint8_t Saturation = 0U;    // цвет хвостов
  	uint8_t e_TAIL_STEP = 127U; // длина хвоста
  	if (isColored)
    	Saturation = scale * 2.55;
  	else
  	{
    	e_TAIL_STEP = 255U - scale * 2.5;
  	}
  	for (uint8_t x = 0U; x < WIDTH - 1U; x++) // fix error i != 0U
  	{
    	if (!random8(e_sns_DENSE) &&
        	!getPixColorXY(wrapX(x), HEIGHT - 1U) &&
        	!getPixColorXY(wrapX(x + 1U), HEIGHT - 1U) &&
        	!getPixColorXY(wrapX(x - 1U), HEIGHT - 1U))
    	{
      		drawPixelXY(x, HEIGHT - 1U, CHSV(random8(), Saturation, random8(64U, 255U)));
    	}
  	}

  	// сдвигаем по диагонали
  	for (uint8_t y = 0U; y < HEIGHT - 1U; y++)
  	{
    	for (uint8_t x = 0; x < WIDTH; x++)
    	{
      		drawPixelXY(wrapX(x + 1U), y, getPixColorXY(x, y + 1U));
    	}
  	}

  	// уменьшаем яркость верхней линии, формируем "хвосты"
  	for (uint8_t i = 0U; i < WIDTH; i++)
  	{
    	fadePixel(i, HEIGHT - 1U, e_TAIL_STEP);
  	}
}

/*void stormyRain()
{
  	// ( Depth of dots, maximum brightness, frequency of new dots, length of tails, color, splashes, clouds, ligthening )
  	//rain(0, 90, map8(intensity,0,150)+60, 10, solidRainColor, true, true, true);
  	rain(60, 160, (scale - 1) * 2.58, 30, solidRainColor, true, true, true);
}*/

// ----------- Эффект "ДНК"
// База https://pastebin.com/jwvC1sNF адаптация и доработки kostyamat
// нормальные копирайты:
// https://pastebin.com/jwvC1sNF
//2 DNA spiral with subpixel
//16x16 rgb led matrix demo
//Yaroslaw Turbin 04.09.2020
//https://vk.com/ldirko
//https://www.reddit.com/user/ldirko/
//https://www.reddit.com/r/FastLED/comments/gogs4n/i_made_7x11_matrix_for_my_ntp_clock_project_then/

//this is update for DNA procedure https://pastebin.com/Qa8A5NvW
//add subpixel render foк nice smooth look

void wu_pixel(uint32_t x, uint32_t y, CRGB * col) {      //awesome wu_pixel procedure by reddit u/sutaburosu
  // extract the fractional parts and derive their inverses
  uint8_t xx = x & 0xff, yy = y & 0xff, ix = 255 - xx, iy = 255 - yy;
  // calculate the intensities for each affected pixel
#define WU_WEIGHT(a,b) ((uint8_t) (((a)*(b)+(a)+(b))>>8))
  uint8_t wu[4] = {WU_WEIGHT(ix, iy), WU_WEIGHT(xx, iy),
                   WU_WEIGHT(ix, yy), WU_WEIGHT(xx, yy)
                  };
  // multiply the intensities by the colour, and saturating-add them to the pixels
  for (uint8_t i = 0; i < 4; i++) {
    uint16_t xy = XY((x >> 8) + (i & 1), (y >> 8) + ((i >> 1) & 1));
    if (xy < NUM_LEDS) {
      leds[xy].r = qadd8(leds[xy].r, col->r * wu[i] >> 8);
      leds[xy].g = qadd8(leds[xy].g, col->g * wu[i] >> 8);
      leds[xy].b = qadd8(leds[xy].b, col->b * wu[i] >> 8);
    }
  }
}

void FieryLedLampEffectDNA::setup()
{
    step = map8(speed, 10U, 60U);
    hue = scale;
    deltaHue = hue > 50U;
    if (deltaHue)
	    hue = 101U - hue;
    hue = 255U - map( 51U - hue, 1U, 50U, 0, 255U);
}

void FieryLedLampEffectDNA::updateInner()
{
	double freq = 3000;
  	float mn = 255.0 / 13.8;

  	fadeToBlackBy(leds, NUM_LEDS, step);
  	uint16_t ms = millis();

  	if (deltaHue)
    	for (uint8_t i = 0; i < WIDTH; i++)
    	{
      		uint32_t x = beatsin16(step, 0, (HEIGHT - 1) * 256, 0, i * freq);
      		uint32_t y = i * 256;
      		uint32_t x1 = beatsin16(step, 0, (HEIGHT - 1) * 256, 0, i * freq + 32768);

      		CRGB col = CHSV(ms / 29 + i * 255 / (WIDTH - 1), 255, qadd8(hue, beatsin8(step, 60, 255U, 0, i * mn)));
      		CRGB col1 = CHSV(ms / 29 + i * 255 / (WIDTH - 1) + 128, 255, qadd8(hue, beatsin8(step, 60, 255U, 0, i * mn + 128)));
      		wu_pixel (y , x, &col);
      		wu_pixel (y , x1, &col1);
    	}
  	else
    	for (uint8_t i = 0; i < HEIGHT; i++)
    	{
      		uint32_t x = beatsin16(step, 0, (WIDTH - 1) * 256, 0, i * freq);
      		uint32_t y = i * 256;
      		uint32_t x1 = beatsin16(step, 0, (WIDTH - 1) * 256, 0, i * freq + 32768);

      		CRGB col = CHSV(ms / 29 + i * 255 / (HEIGHT - 1), 255, qadd8(hue, beatsin8(step, 60, 255U, 0, i * mn)));
      		CRGB col1 = CHSV(ms / 29 + i * 255 / (HEIGHT - 1) + 128, 255, qadd8(hue, beatsin8(step, 60, 255U, 0, i * mn + 128)));
      		wu_pixel (x , y, &col);
      		wu_pixel (x1 , y, &col1);
	    }

  	blurScreen(16);
}

void MoveFractionalNoiseX(uint8_t **noise3d, int8_t amplitude = 1, float shift = 0) {
	for (uint8_t y = 0; y < HEIGHT; y++) {
    	int16_t amount = ((int16_t)noise3d[0][y] - 128) * 2 * amplitude + shift * 256  ;
    	int8_t delta = abs(amount) >> 8 ;
    	int8_t fraction = abs(amount) & 255;
    	for (uint8_t x = 0 ; x < WIDTH; x++) {
			uint8_t zD,zF;
			if (amount < 0) {
				zD = x - delta; zF = zD - 1;
			} else {
				zD = x + delta; zF = zD + 1;
			}
			CRGB PixelA = CRGB::Black  ;
			if ((zD >= 0) && (zD < WIDTH)) PixelA = leds[XY(zD, y)];
			CRGB PixelB = CRGB::Black ;
			if ((zF >= 0) && (zF < WIDTH)) PixelB = leds[XY(zF, y)];
			//ledsbuff[XY(x, y)] = (PixelA.nscale8(ease8InOutApprox(255 - fraction))) + (PixelB.nscale8(ease8InOutApprox(fraction)));   // lerp8by8(PixelA, PixelB, fraction );
			leds[XY(x, y)] = (PixelA.nscale8(ease8InOutApprox(255 - fraction))) + (PixelB.nscale8(ease8InOutApprox(fraction)));   // lerp8by8(PixelA, PixelB, fraction );
    	}
  	}
  	//memcpy(leds, ledsbuff, sizeof(CRGB)* NUM_LEDS);
}

void MoveFractionalNoiseY(uint8_t **noise3d, int8_t amplitude = 1, float shift = 0)
{
	for (uint8_t x = 0; x < WIDTH; x++) {
    	int16_t amount = ((int16_t)noise3d[x][0] - 128) * 2 * amplitude + shift * 256 ;
    	int8_t delta = abs(amount) >> 8 ;
    	int8_t fraction = abs(amount) & 255;
    	for (uint8_t y = 0 ; y < HEIGHT; y++) {
			uint8_t zD,zF;
      		if (amount < 0) {
        		zD = y - delta; zF = zD - 1;
      		} else {
        		zD = y + delta; zF = zD + 1;
      		}
      		CRGB PixelA = CRGB::Black ;
      		if ((zD >= 0) && (zD < HEIGHT)) PixelA = leds[XY(x, zD)];
      		CRGB PixelB = CRGB::Black ;
      		if ((zF >= 0) && (zF < HEIGHT)) PixelB = leds[XY(x, zF)];
      		//ledsbuff[XY(x, y)] = (PixelA.nscale8(ease8InOutApprox(255 - fraction))) + (PixelB.nscale8(ease8InOutApprox(fraction)));
			leds[XY(x, y)] = (PixelA.nscale8(ease8InOutApprox(255 - fraction))) + (PixelB.nscale8(ease8InOutApprox(fraction)));
    	}
  	}
  	//memcpy(leds, ledsbuff, sizeof(CRGB)* NUM_LEDS);
}

// ------------------------------ ЭФФЕКТ ДЫМ ----------------------
// (c) SottNick

FieryLedLampEffectSmoke::~FieryLedLampEffectSmoke()
{
	for(int i=0;i<WIDTH;i++)
		delete[] noise3d[i];
	delete[] noise3d;
}
void FieryLedLampEffectSmoke::setup()
{
	hue2 = 0U;
	deltaHue = 0;
	deltaHue2 = 0;
	noise32_x=noise32_y=noise32_z=0;
	noise3d=new uint8_t*[WIDTH];
	for(int i=0;i<WIDTH;i++)
		noise3d[i]=new uint8_t[HEIGHT];
}

void FieryLedLampEffectSmoke::updateInner()
{
  	dimAll(254U);

  	deltaHue++;
  	CRGB color;
  	if (colored)
  	{
    	if (hue2 == scale)
    	{
      		hue2 = 0U;
      		hue = random8();
    	}
    	if (deltaHue & 0x01) // какой-то умножитель охота подключить к задержке смены цвета, но хз какой...
      		hue2++;
		hsv2rgb_spectrum(CHSV(hue, 255U, 127U), color);
  	}
  	else {
    	hsv2rgb_spectrum(CHSV((scale - 1U) * 2.6, (scale > 98U) ? 0U : 255U, 127U), color);
  	}

	if (random8(WIDTH) != 0U) // встречная спираль движется не всегда синхронно основной
    	deltaHue2--;
	for (uint8_t y = 0; y < HEIGHT; y++) {
    	leds[XY((deltaHue  + y + 1U) % WIDTH, HEIGHT - 1U - y)] += color;
    	leds[XY((deltaHue  + y     ) % WIDTH, HEIGHT - 1U - y)] += color; //color2
    	leds[XY((deltaHue2 + y     ) % WIDTH,               y)] += color;
    	leds[XY((deltaHue2 + y + 1U) % WIDTH,               y)] += color; //color2
  	}
	// скорость движения по массиву noise
  	noise32_x += 1500;//1000;
  	noise32_y += 1500;//1000;
  	noise32_z += 1500;//1000;

  	// хрен знает что
  	uint32_t scale32_x = 4000;
  	uint32_t scale32_y = 4000;
  	
	FillNoise(noise3d, noise32_x, noise32_y, noise32_z, scale32_x, scale32_y, 0);
  
  	// допустимый отлёт зажжённого пикселя от изначально присвоенного местоположения (от 0 до указанного значения. дробное)
  	MoveFractionalNoiseX(noise3d, 3);//4
  	MoveFractionalNoiseY(noise3d, 3);//4

  	blurScreen(20); // без размытия как-то пиксельно, наверное...
}

//-------- Эффект Дымовые шашки ----------- aka "Детские сны"
// (c) Stepko
// https://editor.soulmatelights.com/gallery/505
// https://github.com/DmytroKorniienko/FireLamp_JeeUI/blob/master/src/effects.cpp

void FieryLedLampEffectSmokeBalls::setup()
{
	setCurrentPalette();

    enlargedObjectNUM = (scale - 1U) % 11U + 1U;
    //speedfactor = fmap(speed, 1., 255., .02, .1); // попробовал разные способы управления скоростью. Этот максимально приемлемый, хотя и сильно тупой.
    for (byte j = 0; j < enlargedObjectNUM; j++) {
    	trackingObjectShift[j] =  random((WIDTH * 10) - ((WIDTH / 3) * 20)); // сумма trackingObjectState + trackingObjectShift не должна выскакивать за макс.Х
      	trackingObjectSpeedX[j] = (float)random(25, 80 * WIDTH) / 5.;
      	trackingObjectState[j] = random((WIDTH / 2) * 10, (WIDTH / 3) * 20);
      	trackingObjectHue[j] = random8();//(9) * 28;
      	trackingObjectPosX[j] = trackingObjectShift[j];
    }
}

void FieryLedLampEffectSmokeBalls::updateInner()
{
  	for (byte x = 0; x < WIDTH; x++) {
    	for (float y = (float)HEIGHT; y > 0.; y -= speedfactor) {
      		drawPixelXY(x, y, getPixColorXY(x, y - 1));
    	}
  	}

	fadeToBlackBy(leds, NUM_LEDS, 128U / HEIGHT);
  	if (speed & 0x01)
    	blurScreen(20);
  	for (byte j = 0; j < enlargedObjectNUM; j++) {
    	trackingObjectPosX[j] = beatsin16((uint8_t)(trackingObjectSpeedX[j] * (speedfactor * 5.)), trackingObjectShift[j], trackingObjectState[j] + trackingObjectShift[j], trackingObjectHue[j] * 256, trackingObjectHue[j] * 8);
    	drawPixelXYF(trackingObjectPosX[j] / 10., 0.05, ColorFromPalette(*curPalette, trackingObjectHue[j]));
  	}

  	EVERY_N_SECONDS(20) {
    	for (byte j = 0; j < enlargedObjectNUM; j++) {
      		trackingObjectShift[j] += random(-20, 20);
      		trackingObjectHue[j] += 28;
    	}
  	}

  	//loadingFlag = random8() > 253U;
}

// генератор палитр для Жидкой лампы (c) SottNick
static const uint8_t MBVioletColors_arr[5][4] PROGMEM = // та же палитра, но в формате CHSV
{
  {0  , 0  , 255, 255}, //  0, 255,   0,   0, // red
  //  {1  , 108, 161, 122}, //  1,  46, 123,  87, // seaBlue
  {1  , 155, 209, 255}, //  1,  46, 124, 255, // сделал поярче цвет воды
  {80 , 170, 255, 140}, // 80,   0,   0, 139, // DarkBlue
  {150, 213, 255, 128}, //150, 128,   0, 128, // purple
  {255, 0  , 255, 255}  //255, 255,   0,   0  // red again
};

void FieryLedLampEffectLiquidLamp::fillPalette(uint8_t hue, bool isInvert)
{
	int8_t lastSlotUsed = -1;
  	uint8_t istart8, iend8;
  	CRGB rgbstart, rgbend;

  	// начинаем с нуля
  	if (isInvert)
    	//с неявным преобразованием оттенков цвета получаются, как в фотошопе, но для данного эффекта не красиво выглядят
    	//rgbstart = CHSV(256 + hue - pgm_read_byte(&MBVioletColors_arr[0][1]), pgm_read_byte(&MBVioletColors_arr[0][2]), pgm_read_byte(&MBVioletColors_arr[0][3])); // начальная строчка палитры с инверсией
    	hsv2rgb_spectrum(CHSV(256 + hue - pgm_read_byte(&MBVioletColors_arr[0][1]), pgm_read_byte(&MBVioletColors_arr[0][2]), pgm_read_byte(&MBVioletColors_arr[0][3])), rgbstart);
  	else
    	//rgbstart = CHSV(hue + pgm_read_byte(&MBVioletColors_arr[0][1]), pgm_read_byte(&MBVioletColors_arr[0][2]), pgm_read_byte(&MBVioletColors_arr[0][3])); // начальная строчка палитры
    	hsv2rgb_spectrum(CHSV(hue + pgm_read_byte(&MBVioletColors_arr[0][1]), pgm_read_byte(&MBVioletColors_arr[0][2]), pgm_read_byte(&MBVioletColors_arr[0][3])), rgbstart);
  	int indexstart = 0; // начальный индекс палитры
  	for (uint8_t i = 1U; i < 5U; i++) { // в палитре @obliterator всего 5 строчек
    	int indexend = pgm_read_byte(&MBVioletColors_arr[i][0]);
    	if (isInvert)
      		//rgbend = CHSV(256 + hue - pgm_read_byte(&MBVioletColors_arr[i][1]), pgm_read_byte(&MBVioletColors_arr[i][2]), pgm_read_byte(&MBVioletColors_arr[i][3])); // следующая строчка палитры с инверсией
      		hsv2rgb_spectrum(CHSV(256 + hue - pgm_read_byte(&MBVioletColors_arr[i][1]), pgm_read_byte(&MBVioletColors_arr[i][2]), pgm_read_byte(&MBVioletColors_arr[i][3])), rgbend);
    	else
      		//rgbend = CHSV(hue + pgm_read_byte(&MBVioletColors_arr[i][1]), pgm_read_byte(&MBVioletColors_arr[i][2]), pgm_read_byte(&MBVioletColors_arr[i][3])); // следующая строчка палитры
      		hsv2rgb_spectrum(CHSV(hue + pgm_read_byte(&MBVioletColors_arr[i][1]), pgm_read_byte(&MBVioletColors_arr[i][2]), pgm_read_byte(&MBVioletColors_arr[i][3])), rgbend);
    	istart8 = indexstart / 16;
    	iend8   = indexend   / 16;
    	if ((istart8 <= lastSlotUsed) && (lastSlotUsed < 15)) {
      		istart8 = lastSlotUsed + 1;
      		if (iend8 < istart8)
        		iend8 = istart8;
    	}
    	lastSlotUsed = iend8;
    	fill_gradient_RGB( palette, istart8, rgbstart, iend8, rgbend);
    	indexstart = indexend;
    	rgbstart = rgbend;
  	}
}

void FieryLedLampEffectLiquidLamp::position()
{
	for (uint8_t i = 0; i < enlargedObjectNUM; i++) {
    	liquidLampHot[i] += mapcurve(trackingObjectPosY[i], 0, HEIGHT - 1, 5, -5, InOutQuad) * speedfactor;

		float heat = (liquidLampHot[i] / trackingObjectState[i]) - 1;
		if (heat > 0 && trackingObjectPosY[i] < HEIGHT - 1) {
			trackingObjectSpeedY[i] += heat * liquidLampSpf[i];
		}
		if (trackingObjectPosY[i] > 0) {
			trackingObjectSpeedY[i] -= 0.07;
		}

		if (trackingObjectSpeedY[i]) trackingObjectSpeedY[i] *= 0.85;
		trackingObjectPosY[i] += trackingObjectSpeedY[i] * speedfactor;

		if (trackingObjectSpeedX[i]) trackingObjectSpeedX[i] *= 0.7;
		trackingObjectPosX[i] += trackingObjectSpeedX[i] * speedfactor;

		if (trackingObjectPosX[i] > WIDTH - 1) trackingObjectPosX[i] -= WIDTH - 1;
		if (trackingObjectPosX[i] < 0) trackingObjectPosX[i] += WIDTH - 1;
		if (trackingObjectPosY[i] > HEIGHT - 1) trackingObjectPosY[i] = HEIGHT - 1;
		if (trackingObjectPosY[i] < 0) trackingObjectPosY[i] = 0;
  	}
}

void FieryLedLampEffectLiquidLamp::physic()
{
	for (uint8_t i = 0; i < enlargedObjectNUM; i++) {
    	// отключаем физику на границах, чтобы не слипались шары
    	if (trackingObjectPosY[i] < 3 || trackingObjectPosY[i] > HEIGHT - 1) continue;
    	for (uint8_t j = 0; j < enlargedObjectNUM; j++) {
      		if (trackingObjectPosY[j] < 3 || trackingObjectPosY[j] > HEIGHT - 1) continue;
      		float radius = 3;//(trackingObjectShift[i] + trackingObjectShift[j]);
      		if (trackingObjectPosX[i] + radius > trackingObjectPosX[j]
          		&& trackingObjectPosX[i] < radius + trackingObjectPosX[j]
          		&& trackingObjectPosY[i] + radius > trackingObjectPosY[j]
          		&& trackingObjectPosY[i] < radius + trackingObjectPosY[j]
         	) {        
        		float dx =  min((float)fabs(trackingObjectPosX[i] - trackingObjectPosX[j]), (float)WIDTH + trackingObjectPosX[i] - trackingObjectPosX[j]); //по идее бесшовный икс
        		float dy =  fabs(trackingObjectPosY[i] - trackingObjectPosY[j]);
        		float dist = sqrtf((dx * dx) + (dy * dy));

        		if (dist <= radius) {
          			float nx = (trackingObjectPosX[j] - trackingObjectPosX[i]) / dist;
          			float ny = (trackingObjectPosY[j] - trackingObjectPosY[i]) / dist;
          			float p = 2 * (trackingObjectSpeedX[i] * nx + trackingObjectSpeedY[i] * ny - trackingObjectSpeedX[j] * nx - trackingObjectSpeedY[j] * ny) / (trackingObjectState[i] + trackingObjectState[j]);
          			float pnx = p * nx, pny = p * ny;
          			trackingObjectSpeedX[i] = trackingObjectSpeedX[i] - pnx * trackingObjectState[i];
          			trackingObjectSpeedY[i] = trackingObjectSpeedY[i] - pny * trackingObjectState[i];
          			trackingObjectSpeedX[j] = trackingObjectSpeedX[j] + pnx * trackingObjectState[j];
          			trackingObjectSpeedY[j] = trackingObjectSpeedY[j] + pny * trackingObjectState[j];
        		}
      		}
    	}
  	}
}

#define MASS_MIN 10
#define MASS_MAX 50

void FieryLedLampEffectLiquidLamp::setup()
{
	speedfactor = speed / 64.0 + 0.1; // 127 БЫЛО

    if (isColored) {
		fillPalette((scale - 1U) * 2.55, !(scale & 0x01));
      	enlargedObjectNUM = enlargedOBJECT_MAX_COUNT / 2U - 2U; //14U;
    }
    else {
    	enlargedObjectNUM = (scale - 1U) / 99.0 * (enlargedOBJECT_MAX_COUNT - 1U) + 1U;
      	hue = random8();
      	deltaHue = random8(2U);
      	fillPalette(hue, deltaHue);
    }
    if (enlargedObjectNUM > enlargedOBJECT_MAX_COUNT)
		enlargedObjectNUM = enlargedOBJECT_MAX_COUNT;
    else
		if (enlargedObjectNUM < 2U)
			enlargedObjectNUM = 2U;

    double minSpeed = 0.2, maxSpeed = 0.8;

    for (uint8_t i = 0 ; i < enlargedObjectNUM ; i++) {
    	trackingObjectPosX[i] = random8(WIDTH);
    	trackingObjectPosY[i] = 0; //random8(HEIGHT);
    	trackingObjectState[i] = random(MASS_MIN, MASS_MAX);
    	liquidLampSpf[i] = fmap(trackingObjectState[i], MASS_MIN, MASS_MAX, 0.0015, 0.0005);
    	trackingObjectShift[i] = fmap(trackingObjectState[i], MASS_MIN, MASS_MAX, 2, 3);
    	liquidLampMX[i] = map(trackingObjectState[i], MASS_MIN, MASS_MAX, 60, 80); // сила возмущения
    	liquidLampSC[i] = map(trackingObjectState[i], MASS_MIN, MASS_MAX, 6, 10); // радиус возмущения
    	liquidLampTR[i] = liquidLampSC[i]  * 2 / 3; // отсечка расчетов (оптимизация скорости)
    }
}

void FieryLedLampEffectLiquidLamp::updateInner()
{
  	position();
	physic();

  	if(!isColored) {
    	hue2++;
    	if (hue2 % 0x10 == 0U) {
      		hue++;
      		fillPalette(hue, deltaHue);
    	}
  	}

  	for (uint8_t x = 0; x < WIDTH; x++) {
    	for (uint8_t y = 0; y < HEIGHT; y++) {
      		float sum = 0;
      		for (uint8_t i = 0; i < enlargedObjectNUM; i++) {
        		if (abs(x - trackingObjectPosX[i]) > liquidLampTR[i] || abs(y - trackingObjectPosY[i]) > liquidLampTR[i]) continue;
        		float dx =  min((float)fabs(trackingObjectPosX[i] - (float)x), (float)WIDTH + trackingObjectPosX[i] - (float)x); //по идее бесшовный икс
        		float dy =  fabs(trackingObjectPosY[i] - (float)y);
        		float d = sqrtf((dx * dx) + (dy * dy));

        		if (d < trackingObjectShift[i]) {
          			sum += mapcurve(d, 0, trackingObjectShift[i], 255, liquidLampMX[i], InQuad);
        		}
        		else if (d < liquidLampSC[i]) {
          			sum += mapcurve(d, trackingObjectShift[i], liquidLampSC[i], liquidLampMX[i], 0, OutQuart);
        		}
        		if (sum >= 255) {
          			sum = 255;
          			break;
        		}
      		}
      		if (sum < 16) sum = 16;// отрезаем смазанный кусок палитры из-за отсутствия параметра NOBLEND
      		CRGB color = ColorFromPalette(palette, sum); // ,255, NOBLEND
      		drawPixelXY(x, y, color);
    	}
  	}
}

// ============== Swirl ================
//    © SlingMaster | by Alex Dovby
//              EFF_SWIRL
//--------------------------------------
void FieryLedLampEffectSwirl::setup()
{
	FastLED.clear();
    deltaValue = 255U - speed + 1U;
    step = deltaValue;                      // чтообы при старте эффекта сразу покрасить лампу
    deltaHue2 = 0U;                         // count для замедления смены цвета
    deltaHue = 0U;                          // direction | 0 hue-- | 1 hue++ |
    hue2 = 0U;                              // x
}

void FieryLedLampEffectSwirl::updateInner()
{
	#if 0
	if (scale > 50)
		Spindle(); // Якщо масштаб/колір більше 50 - тоді єфект "Веретено"
  	else {
  		uint8_t divider;
  		uint8_t lastHue;
  		static const uint32_t colors[5][6] PROGMEM = {
    		{CRGB::Blue, CRGB::DarkRed, CRGB::Aqua, CRGB::Magenta, CRGB::Gold, CRGB::Green },
    		{CRGB::Yellow, CRGB::LemonChiffon, CRGB::LightYellow, CRGB::Gold, CRGB::Chocolate, CRGB::Goldenrod},
    		{CRGB::Green, CRGB::DarkGreen, CRGB::LawnGreen, CRGB::SpringGreen, CRGB::Cyan, CRGB::Black },
    		{CRGB::Blue, CRGB::DarkBlue, CRGB::MidnightBlue, CRGB::MediumSeaGreen, CRGB::MediumBlue, CRGB:: DeepSkyBlue },
    		{CRGB::Magenta, CRGB::Red, CRGB::DarkMagenta, CRGB::IndianRed, CRGB::Gold, CRGB::MediumVioletRed }
  		};
  		uint32_t color;

  		if (step >= deltaValue) {
    		step = 0U;
  		}
  		divider = floor((scale - 1) / 20); // маштаб задает смену палитры
  		// задаем цвет и рисуем завиток --------
  		color = colors[divider][hue];
  		drawPixelXY(hue2, deltaHue2, color);
  		hue2++;                     // x
  		// два варианта custom_eff задается в сетапе лампы ----
  		if (custom_eff == 1) {
    		deltaHue2++;              // y
  		} else {
    		if (hue2 % 2 == 0) {
      			deltaHue2++;            // y
    		}
  		}
  		// -------------------------------------
  		if  (hue2 > WIDTH) {
    		hue2 = 0U;
  		}

  		if (deltaHue2 >= HEIGHT) {
    		deltaHue2 = 0U;
    		// new swirl ------------
    		hue2 = random8(WIDTH - 2);    
    		// select new color -----
    		hue = random8(6);
			if (lastHue == hue) {
      			hue = hue + 1;
      			if (hue >= 6) {
        			hue = 0;
      			}
    		}
    		lastHue = hue;
  		}
  		blurScreen(4U + random8(8));
  		step++;
  	} // else у пачатку функції
	#endif
}

// =====================================
//                Stars
//     © SottNick and  © Stepko
//      Adaptation © SlingMaster
//                Звезды
// =====================================

void FieryLedLampEffect::drawStar(float xlocl, float ylocl, float biggy, float little, int16_t points, float dangle, uint8_t koler) { // random multipoint star
  	float radius2 = 255.0 / points;
  	for (int i = 0; i < points; i++) {
    	DrawLine(xlocl + ((little * (sin8(i * radius2 + radius2 / 2 - dangle) - 128.0)) / 128), ylocl + ((little * (cos8(i * radius2 + radius2 / 2 - dangle) - 128.0)) / 128), xlocl + ((biggy * (sin8(i * radius2 - dangle) - 128.0)) / 128), ylocl + ((biggy * (cos8(i * radius2 - dangle) - 128.0)) / 128), ColorFromPalette(*curPalette, koler));
    	DrawLine(xlocl + ((little * (sin8(i * radius2 - radius2 / 2 - dangle) - 128.0)) / 128), ylocl + ((little * (cos8(i * radius2 - radius2 / 2 - dangle) - 128.0)) / 128), xlocl + ((biggy * (sin8(i * radius2 - dangle) - 128.0)) / 128), ylocl + ((biggy * (cos8(i * radius2 - dangle) - 128.0)) / 128), ColorFromPalette(*curPalette, koler));
  	}
}

// --------------------------------------
void FieryLedLampEffectStars::setup()
{  
    counter = 0.0;
    // стартуем с центра
    driftx = (float)WIDTH / 2.0;
    drifty = (float)HEIGHT / 2.0;

    cangle = (float)(sin8(random8(25, 220)) - 128.0f) / 128.0f; //angle of movement for the center of animation gives a float value between -1 and 1
    sangle = (float)(sin8(random8(25, 220)) - 128.0f) / 128.0f; //angle of movement for the center of animation in the y direction gives a float value between -1 and 1
    stars_count = WIDTH / 2U;

    if (stars_count > STARS_NUM)
		stars_count = STARS_NUM;
    for (uint8_t num = 0; num < stars_count; num++) {
    	points[num] = map(scale, 1, 255, 3U, 7U); //5; // random8(3, 6);                              // количество углов в звезде
    	delay_arr[num] = speed / 5 + (num << 2) + 2U;               // задержка следующего пуска звезды
    	color[num] = random8();
    }
}

void FieryLedLampEffectStars::updateInner()
{
	fadeToBlackBy(leds, NUM_LEDS, 165);
  	float speedFactor = ((float)speed / 380.0 + 0.05);
  	counter += speedFactor;                                                   // определяет то, с какой скоростью будет приближаться звезда

  	if (driftx > (WIDTH - WIDTH / 4)) cangle = 0 - fabs(cangle);      //change directin of drift if you get near the right 1/4 of the screen
  	if (driftx < WIDTH / 4) cangle = fabs(cangle);                    //change directin of drift if you get near the right 1/4 of the screen
  	if ((uint16_t)counter % CENTER_DRIFT_SPEED == 0) driftx = driftx + (cangle * speedFactor); //move the x center every so often
  	if (drifty > ( HEIGHT - HEIGHT / 4)) sangle = 0 - fabs(sangle);    // if y gets too big, reverse
  	if (drifty < HEIGHT / 4) sangle = fabs(sangle);                    // if y gets too small reverse

  	if ((uint16_t)counter % CENTER_DRIFT_SPEED == 0) drifty = drifty + (sangle * speedFactor); //move the y center every so often

  	for (uint8_t num = 0; num < stars_count; num++) {
    	if (counter >= delay_arr[num]) {              //(counter >= ringdelay)
      		if (counter - delay_arr[num] <= WIDTH + 5) {
        		drawStar(driftx, drifty, 2 * (counter - delay_arr[num]), (counter - delay_arr[num]), points[num], STAR_BLENDER + color[num], color[num]);
        		color[num] += speedFactor;                // в зависимости от знака - направление вращения
      		} else {
        		delay_arr[num] = counter + (stars_count << 1) + 1U; // задержка следующего пуска звезды
      		}
    	}
  	}
  	blur2d(leds, WIDTH, HEIGHT, scale / 2);
}

void FieryLedLampEffectZebra::setup()
{
	#if 0
	// 'black out' all 16 palette entries...
    fill_solid(currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    currentPalette[0] = CRGB::White;
    currentPalette[4] = CRGB::White;
    currentPalette[8] = CRGB::White;
    currentPalette[12] = CRGB::White;
    colorLoop = 1;
	#endif
}
void FieryLedLampEffectZebra::updateInner()
{
	#if 0
	fillNoiseLED();
	#endif
}

// --------------------------------------
void FieryLedLampEffectTixyLand::setup()
{
    //deltaHue = 0;
    pcnt = map(speed, 5, 250, 1U, 25U);
    //FPSdelay = 1;
    deltaHue2 = scale * 2.55;
    hue = 255U;
	hue2 = 0U;
}

// ============= Tixy Land ==============
//        © Martin Kleppe @aemkei
//github.com/owenmcateer/tixy.land-display
//      Create Script Change Effects
//             © SlingMaster
// ======================================
//   набор мат. функций и примитивов для
//            обсчета эффектов
//       © Dmytro Korniienko (kDn)
// ======================================

#define M_PI_2  1.57079632679489661923
static const PROGMEM float LUT[102] = {
  0,           0.0099996664, 0.019997334, 0.029991005, 0.039978687,
  0.049958397, 0.059928156,  0.069885999, 0.079829983, 0.089758173,
  0.099668652, 0.10955953,   0.11942893,  0.12927501,  0.13909595,
  0.14888994,  0.15865526,   0.16839015,  0.17809294,  0.18776195,
  0.19739556,  0.20699219,   0.21655031,  0.22606839,  0.23554498,
  0.24497867,  0.25436807,   0.26371184,  0.27300870,  0.28225741,
  0.29145679,  0.30060568,   0.30970293,  0.31874755,  0.32773849,
  0.33667481,  0.34555557,   0.35437992,  0.36314702,  0.37185606,
  0.38050637,  0.38909724,   0.39762798,  0.40609807,  0.41450688,
  0.42285392,  0.43113875,   0.43936089,  0.44751999,  0.45561564,
  0.46364760,  0.47161558,   0.47951928,  0.48735857,  0.49513325,
  0.50284320,  0.51048833,   0.51806855,  0.52558380,  0.53303409,
  0.54041952,  0.54774004,   0.55499572,  0.56218672,  0.56931317,
  0.57637525,  0.58337301,   0.59030676,  0.59717667,  0.60398299,
  0.61072594,  0.61740589,   0.62402308,  0.63057774,  0.63707036,
  0.64350110,  0.64987046,   0.65617871,  0.66242629,  0.66861355,
  0.67474097,  0.68080884,   0.68681765,  0.69276786,  0.69865984,
  0.70449406,  0.71027100,   0.71599114,  0.72165483,  0.72726268,
  0.73281509,  0.73831260,   0.74375558,  0.74914461,  0.75448018,
  0.75976276,  0.76499283,   0.77017093,  0.77529752,  0.78037310,
  0.78539819,  0.79037325
};
// неточный, зато более быстрый квадратный корень
float sqrt3(const float x)
{
  union
  {
    int i;
    float x;
  } u;

  u.x = x;
  u.i = (1<<29) + (u.i >> 1) - (1<<22);
  return u.x;
}
float atan2_fast(float y, float x) {
  //http://pubs.opengroup.org/onlinepubs/009695399/functions/atan2.html
  //Volkan SALMA

  const float ONEQTR_PI = PI / 4.0;
  const float THRQTR_PI = 3.0 * PI / 4.0;
  float r, angle;
  float abs_y = fabs(y) + 1e-10f;      // kludge to prevent 0/0 condition
  if ( x < 0.0f ) {
    r = (x + abs_y) / (abs_y - x);
    angle = THRQTR_PI;
  } else {
    r = (x - abs_y) / (x + abs_y);
    angle = ONEQTR_PI;
  }
  angle += (0.1963f * r * r - 0.9817f) * r;
  if ( y < 0.0f ) {
    return ( -angle );    // negate if in quad III or IV
  } else {
    return ( angle );
  }
}
// --------------------------------------
float atan_fast(float x) {
  /* A fast look-up method with enough accuracy */
  if (x > 0) {
    if (x <= 1) {
      int index = round(x * 100);
      return LUT[index];
    } else {
      float re_x = 1 / x;
      int index = round(re_x * 100);
      return (M_PI_2 - LUT[index]);
    }
  } else {
    if (x >= -1) {
      float abs_x = -x;
      int index = round(abs_x * 100);
      return -(LUT[index]);
    } else {
      float re_x = 1 / (-x);
      int index = round(re_x * 100);
      return (LUT[index] - M_PI_2);
    }
  }
}

// --------------------------------------
float tan2pi_fast(float x) {
  float y = (1 - x * x);
  return x * (((-0.000221184 * y + 0.0024971104) * y - 0.02301937096) * y + 0.3182994604 + 1.2732402998 / y);
}

float FieryLedLampEffectTixyLand::code(double t, double i, double x, double y)
{
	switch (pcnt)
	{
    /** © Motus Art @motus_art */
    case 1: /* Plasma */
    	hue = 96U; hue2 = 224U;
      	return (sin16((x + t) * 8192.0) * 0.5 + sin16((y + t) * 8192.0) * 0.5 + sin16((x + y + t) * 8192.0) * 0.3333333333333333) / 32767.0;
    case 2: /* Up&Down */
      	hue = 255U; hue2 = 160U;
      	return sin16((cos16(x * 8192.0) / 32767.0 * y / (HEIGHT / 2.0) + t) * 8192.0) / 32767.0;
    case 3:
      	hue = 255U; hue2 = 96U;
      	return sin16((atan_fast(y / x) + t) * 8192.0) / 32767.0;
	/** © tixy.land website */
    case 4: /* Emitting rings */
      hue = 255U; hue2 = 0U;
      return sin16((t - sqrt3((x - (WIDTH / 2)) * (x - (WIDTH / 2)) + (y - (HEIGHT / 2)) * (y - (HEIGHT / 2)))) * 8192.0) / 32767.0;
    case 5: /* Rotation  */
      hue = 136U; hue2 = 48U;
      return sin16((PI * 2.5 * atan_fast((y - (HEIGHT / 2)) / (x - (WIDTH / 2))) + 5 * t) * 8192.0) / 32767.0;
    case 6: /* Vertical fade */
      hue = 160U; hue2 = 0U;
      return sin16((y / 8 + t) * 8192.0) / 32767.0;
    case 7: /* Waves */
      //return sin(x / 2) - sin(x - t) - y + 6;
      hue = 48U; hue2 = 160U;
      return (sin16(x * 4096.0) - sin16((x - t) * 8192.0)) / 32767.0 - y + (HEIGHT / 2);
    case 8: /* Drop */
      hue = 136U; hue2 = 160U;
      return fmod(8 * t, 13) - sqrt3((x - (WIDTH / 2)) * (x - (WIDTH / 2)) + (y - (HEIGHT / 2)) * (y - (HEIGHT / 2)));
    case 9: /* Ripples @thespite */
      hue = 96U; hue2 = 224U;
      return sin16((t - sqrt3(x * x + y * y)) * 8192.0) / 32767.0;
    case 10: /* Bloop bloop bloop @v21 */
      hue = 136U; hue2 = 160U;
      return (x - (WIDTH / 2)) * (y - (HEIGHT / 2)) - sin16(t * 4096.0) / 512.0;
    case 11: /* SN0WFAKER */
      // https://www.reddit.com/r/programming/comments/jpqbux/minimal_16x16_dots_coding_environment/gbgk7c0/
      hue = 96U; hue2 = 160U;
      return sin16((atan_fast((y - (HEIGHT / 2)) / (x - (WIDTH / 2))) + t) * 8192.0) / 32767.0;
    case 12: /* detunized */
      // https://www.reddit.com/r/programming/comments/jpqbux/minimal_16x16_dots_coding_environment/gbgk30l/
      hue = 136U; hue2 = 160U;
      return sin16((y / (HEIGHT / 2) + t * 0.5) * 8192.0) / 32767.0 + x / 16 - 0.5;
    /** © @akella | https://twitter.com/akella/status/1323549082552619008 */
    case 13:
      hue = 255U; hue2 = 0U;
      return sin16((6 * atan2_fast(y - (HEIGHT / 2), x) + t) * 8192.0) / 32767.0;
    case 14:
      hue = 32U; hue2 = 160U;
      return sin16((i / 5 + t) * 16384.0) / 32767.0;
    /** © Paul Malin | https://twitter.com/P_Malin/ */

    // sticky blood
    // by @joeytwiddle
    //(t,i,x,y) => y-t*3+9+3*cos(x*3-t)-5*sin(x*7)

    //      if (x < 8) {
    //       // hue = 160U;
    //      } else {
    //       // hue = 96U;
    //      }
    //      if ((y == HEIGHT -1)&(x == 8)) {
    //        hue = hue + 30;
    //        if (hue >= 255U) {
    //          hue = 0;
    //        }
    //      }
    //      hue = t/128+8;

    //    case 19: // !!!! paint
    //      // Matrix Rain https://twitter.com/P_Malin/status/1323583013880553472
    //      //return 1. - fmod((x * x - y + t * (fmod(1 + x * x, 5)) * 6), 16) / 16;
    //      return 1. - fmod((x * x - (HEIGHT - y) + t * (1 + fmod(x * x, 5)) * 3), WIDTH) / HEIGHT;
    //      break;

    case 15: /* Burst */
      // https://twitter.com/P_Malin/status/1323605999274594304
      hue = 136U; hue2 = 160U;
      return -10. / ((x - (WIDTH / 2)) * (x - (WIDTH / 2)) + (y - (HEIGHT / 2)) * (y - (HEIGHT / 2)) - fmod(t * 0.3, 0.7) * 200);
    case 16: /* Rays */
      hue = 255U; hue2 = 0U;
      return sin16((atan2_fast(x, y) * 5 + t * 2) * 8192.0) / 32767.0;
    case 17: /* Starfield */
      // org | https://twitter.com/P_Malin/status/1323702220320313346
      hue = 255U; hue2 = 160U;
      return !((int)(x + t * 50 / (fmod(y * y, 5.9) + 1)) & 15) / (fmod(y * y, 5.9) + 1);
      //      {
      //        uint16_t _y = HEIGHT - y;
      //        float d = (fmod(_y * _y + 4, 4.1) + 0.85) * 0.5; // коэффициенты тут отвечают за яркость (размер), скорость, смещение, подбираются экспериментально :)
      //        return !((int)(x + t * 7.0 / d) & 15) / d; // 7.0 - множитель скорости
      //      }
    case 18:
      hue = 255U; hue2 = 0U;
      return sin16((3.5 * atan2_fast(y - (HEIGHT / 2) + sin16(t * 8192.0) * 0.00006, x - (WIDTH / 2) + sin16(t * 8192.0) * 0.00006) + t * 1.5 + 5) * 8192.0) / 32767.0;
    case 19:
      hue = 255U; hue2 = 224U;
      return (y - 8) / 3 - tan2pi_fast((x / 6 + 1.87) / PI * 2) * sin16(t * 16834.0) / 32767.0;
    case 20:
      hue = 136U; hue2 = 160U;
      return (y - 8) / 3 - (sin16((x / 4 + t * 2) * 8192.0) / 32767.0);
    case 21:
      hue = 72U; hue2 = 96U;
      return cos(sin16(x * t * 819.2) / 32767.0 * PI) + cos16((sin16((y * t / 10 + (sqrt3(abs(cos16(x * t * 8192.0) / 32767.0)))) * 8192.0) / 32767.0 * PI) * 8192.0) / 32767.0;
    case 22: /* bambuk */
      hue = 96U; hue2 = 80U;
      return sin16(x / 3 * sin16(t * 2730.666666666667) / 2.0) / 32767.0 + cos16(y / 4 * sin16(t * 4096.0) / 2.0) / 32767.0;
    case 23:
      hue = 0U; hue2 = 224U;
      {
        float _x = x - fmod(t, WIDTH);
        float _y = y - fmod(t, HEIGHT);
        return -.4 / (sqrt3(_x * _x + _y * _y) - fmod(t, 2) * 9);
      }
      break;

    case 24: /* honey */
      hue = 255U; hue2 = 40U;
      return sin16(y * t * 2048.0) / 32767.0 * cos16(x * t * 2048.0) / 32767.0;
    case 25:
      hue = 96U; hue2 = 160U;
      return atan_fast((x - (WIDTH / 2)) * (y - (HEIGHT / 2))) - 2.5 * sin16(t * 8192.0) / 32767.0;
    default:
      if (pcnt > 25) {
        deltaHue2 += 32;
      }
      pcnt = 1;
      hue = 96U; hue2 = 0U;
      return sin16(t * 8192.0) / 32767.0;
  }
}

void FieryLedLampEffectTixyLand::processFrame(double t, double x, double y)
{
	double i = (y * WIDTH) + x;
  	uint8_t frame = constrain(code(t, i, x, y), -1, 1) * 255;
  	if (frame > 0) {
    	if ( hue == 255U) {
      		drawPixelXY(x, y, CRGB(frame, frame, frame));
    	} else {
      		drawPixelXY(x, y, CHSV(hue, frame, frame));
    	}
  	} else {
    	if (frame < 0) {
      		if (scale < 5)
				deltaHue2 = 0;
      			drawPixelXY(x, y, CHSV(hue2 + deltaHue2, frame * -1, frame * -1));
    	} else {
      		drawPixelXY(x, y, CRGB::Black);
    	}
  	}
}

void FieryLedLampEffectTixyLand::updateInner()
{
	unsigned long milli = millis();
  	double t = milli / 1000.0;

  	/*EVERY_N_SECONDS(FavoritesManager::Interval)
	{
    	if ((speed < 5) || (speed > 250)) {
      		pcnt++;
    	}
  	}*/
  	for ( double x = 0; x < WIDTH; x++) {
    	for ( double y = 0; y < HEIGHT; y++) {
      		processFrame(t, x, y);
    	}
  	}
}

// ------------- Змейки --------------
// (c) SottNick

#define SNAKES_LENGTH (8U) // длина червяка от 2 до 15 (+ 1 пиксель голова/хвостик), ограничена размером переменной для хранения трактории тела червяка

void FieryLedLampEffectSnakes::setup()
{
	speedfactor = (float)speed / 555.0f + 0.001f;

    enlargedObjectNUM = (scale - 1U) / 99.0 * (enlargedOBJECT_MAX_COUNT - 1U) + 1U;
    if (enlargedObjectNUM > enlargedOBJECT_MAX_COUNT)
		enlargedObjectNUM = enlargedOBJECT_MAX_COUNT;
    for (uint8_t i = 0; i < enlargedObjectNUM; i++) {
		enlargedObjectTime[i] = 0;
      	trackingObjectPosX[i] = random8(WIDTH);
      	trackingObjectPosY[i] = random8(HEIGHT);
      	trackingObjectSpeedX[i] = (255. + random8()) / 255.;
      	trackingObjectSpeedY[i] = 0;
      	trackingObjectHue[i] = random8();
      	trackingObjectState[i] = random8(4);//     B00           направление головы змейки
      // B10     B11
      //     B01
    }
}
  
void FieryLedLampEffectSnakes::updateInner()
{
	FastLED.clear();

	int8_t dx, dy;
  	for (uint8_t i = 0; i < enlargedObjectNUM; i++) {
    	trackingObjectSpeedY[i] += trackingObjectSpeedX[i] * speedfactor;
    	if (trackingObjectSpeedY[i] >= 1)
    	{
      		trackingObjectSpeedY[i] = trackingObjectSpeedY[i] - (int)trackingObjectSpeedY[i];
      		if (random8(9U) == 0U) // вероятность поворота
        		if (random8(2U)) { // <- поворот налево
          			enlargedObjectTime[i] = (enlargedObjectTime[i] << 2) | B01; // младший бит = поворот
          			switch (trackingObjectState[i]) {
            		case B10:
              			trackingObjectState[i] = B01;
              			if (trackingObjectPosY[i] == 0U)
                			trackingObjectPosY[i] = HEIGHT - 1U;
              			else
                			trackingObjectPosY[i]--;
              			break;
            		case B11:
              			trackingObjectState[i] = B00;
              			if (trackingObjectPosY[i] >= HEIGHT - 1U)
                			trackingObjectPosY[i] = 0U;
              			else
                			trackingObjectPosY[i]++;
              			break;
            		case B00:
              			trackingObjectState[i] = B10;
              			if (trackingObjectPosX[i] == 0U)
                			trackingObjectPosX[i] = WIDTH - 1U;
              			else
                			trackingObjectPosX[i]--;
              			break;
            		case B01:
              			trackingObjectState[i] = B11;
              			if (trackingObjectPosX[i] >= WIDTH - 1U)
                			trackingObjectPosX[i] = 0U;
              			else
                			trackingObjectPosX[i]++;
              			break;
          			}
        		}
        		else { // -> поворот направо
          			enlargedObjectTime[i] = (enlargedObjectTime[i] << 2) | B11; // младший бит = поворот, старший = направо
          			switch (trackingObjectState[i]) {
            		case B11:
              			trackingObjectState[i] = B01;
              			if (trackingObjectPosY[i] == 0U)
                			trackingObjectPosY[i] = HEIGHT - 1U;
              			else
                			trackingObjectPosY[i]--;
              			break;
            		case B10:
              			trackingObjectState[i] = B00;
              			if (trackingObjectPosY[i] >= HEIGHT - 1U)
                			trackingObjectPosY[i] = 0U;
              			else
                			trackingObjectPosY[i]++;
              			break;
            		case B01:
              			trackingObjectState[i] = B10;
              			if (trackingObjectPosX[i] == 0U)
                			trackingObjectPosX[i] = WIDTH - 1U;
              			else
                			trackingObjectPosX[i]--;
              			break;
            		case B00:
              			trackingObjectState[i] = B11;
              			if (trackingObjectPosX[i] >= WIDTH - 1U)
                			trackingObjectPosX[i] = 0U;
              			else
                			trackingObjectPosX[i]++;
              			break;
          			}
        		}
      		else { // двигаем без поворота
        		enlargedObjectTime[i] = (enlargedObjectTime[i] << 2);
        		switch (trackingObjectState[i]) {
          		case B01:
            		if (trackingObjectPosY[i] == 0U)
              			trackingObjectPosY[i] = HEIGHT - 1U;
            		else
              			trackingObjectPosY[i]--;
            		break;
          		case B00:
            		if (trackingObjectPosY[i] >= HEIGHT - 1U)
              			trackingObjectPosY[i] = 0U;
            		else
              			trackingObjectPosY[i]++;
            		break;
          		case B10:
            		if (trackingObjectPosX[i] == 0U)
              			trackingObjectPosX[i] = WIDTH - 1U;
            		else
              			trackingObjectPosX[i]--;
            		break;
          		case B11:
            		if (trackingObjectPosX[i] >= WIDTH - 1U)
              			trackingObjectPosX[i] = 0U;
            		else
              			trackingObjectPosX[i]++;
            		break;
        		}
      		}
    	}

		switch (trackingObjectState[i]) {
		case B01:
			dy = 1;
			dx = 0;
			break;
		case B00:
			dy = -1;
			dx = 0;
			break;
		case B10:
			dy = 0;
			dx = 1;
			break;
		case B11:
			dy = 0;
			dx = -1;
			break;
		}
		long temp = enlargedObjectTime[i];
		uint8_t x = trackingObjectPosX[i];
		uint8_t y = trackingObjectPosY[i];	
		leds[XY(x, y)] += CHSV(trackingObjectHue[i], 255U, trackingObjectSpeedY[i] * 255); // тут рисуется голова

		for (uint8_t m = 0; m < SNAKES_LENGTH; m++) { // 16 бит распаковываем, 14 ещё остаётся без дела в запасе, 2 на хвостик
			x = (WIDTH + x + dx) % WIDTH;
			y = (HEIGHT + y + dy) % HEIGHT;      
			leds[XY(x, y)] += CHSV(trackingObjectHue[i] + (m + trackingObjectSpeedY[i]) * 4U, 255U, 255U); // тут рисуется тело

			if (temp & B01) { // младший бит = поворот, старший = направо
				temp = temp >> 1;
				if (temp & B01) { // старший бит = направо
					if (dx == 0) {
						dx = 0 - dy;
						dy = 0;
					}
					else {
						dy = dx;
						dx = 0;
					}
				}
				else { // иначе налево
					if (dx == 0) {
						dx = dy;
						dy = 0;
					}
					else {
						dy = 0 - dx;
						dx = 0;
					}
				}
				temp = temp >> 1;
			}
			else { // если без поворота
				temp = temp >> 2;
			}
		}
		x = (WIDTH + x + dx) % WIDTH;
		y = (HEIGHT + y + dy) % HEIGHT;
		leds[XY(x, y)] += CHSV(trackingObjectHue[i] + (SNAKES_LENGTH + trackingObjectSpeedY[i]) * 4U, 255U, (1 - trackingObjectSpeedY[i]) * 255); // хвостик
	}
}

// ============= ЭФФЕКТ ИСТОЧНИК ===============
// (c) SottNick
// выглядит как https://github.com/fuse314/arduino-particle-sys/blob/master/examples/StarfieldFastLED/StarfieldFastLED.ino

void FieryLedLampEffectFontain::particlesUpdate2(uint8_t i)
{
	//age
  	trackingObjectState[i]--; //ttl // ещё и сюда надо speedfactor вкорячить. удачи там!

  	//apply velocity
  	trackingObjectPosX[i] += trackingObjectSpeedX[i];
  	trackingObjectPosY[i] += trackingObjectSpeedY[i];
  	if (trackingObjectState[i] == 0 || trackingObjectPosX[i] <= -1 || trackingObjectPosX[i] >= WIDTH || trackingObjectPosY[i] <= -1 || trackingObjectPosY[i] >= HEIGHT)
    	trackingObjectIsShift[i] = false;
}

void FieryLedLampEffectFontain::starfield2Emit(uint8_t i)
{
	if (hue++ & 0x01)
    	hue2++;
  	trackingObjectPosX[i] = WIDTH * 0.5;//CENTER_X_MINOR;// * RENDERER_RESOLUTION; //  particle->x = source->x;
  	trackingObjectPosY[i] = HEIGHT * 0.5;//CENTER_Y_MINOR;// * RENDERER_RESOLUTION; //  // particle->y = source->y;

  	trackingObjectSpeedX[i] = ((float)random8() - 127.) / 512.; // random(_hVar)-_constVel; // particle->vx
  	trackingObjectSpeedY[i] = sqrt3(0.0626 - trackingObjectSpeedX[i] * trackingObjectSpeedX[i]); // SQRT_VARIANT(pow(_constVel,2)-pow(trackingObjectSpeedX[i],2)); // particle->vy зависит от particle->vx - не ошибка
  	if (random8(2U)) {
    	trackingObjectSpeedY[i] = -trackingObjectSpeedY[i];
  	}
  	trackingObjectState[i] = random8(50, 250); // random8(minLife, maxLife);// particle->ttl
  	if (speed & 0x01)
    	trackingObjectHue[i] = hue2;// (counter/2)%255; // particle->hue
  	else
    	trackingObjectHue[i] = random8();
  	trackingObjectIsShift[i] = true; // particle->isAlive
}

void FieryLedLampEffectFontain::setup()
{
    enlargedObjectNUM = (scale - 1U) / 99.0 * (trackingOBJECT_MAX_COUNT - 1U) + 1U;
    if (enlargedObjectNUM > trackingOBJECT_MAX_COUNT)
		enlargedObjectNUM = trackingOBJECT_MAX_COUNT;
    deltaValue = enlargedObjectNUM / (sqrt3(CENTER_X_MAJOR * CENTER_X_MAJOR + CENTER_Y_MAJOR * CENTER_Y_MAJOR) * 4U) + 1U; // 4 - это потому что за 1 цикл частица пролетает ровно четверть расстояния между 2мя соседними пикселями
    for (int i = 0; i < enlargedObjectNUM; i++)
    	trackingObjectIsShift[i] = false; // particle->isAlive
}

void FieryLedLampEffectFontain::updateInner()
{
	uint8_t step = deltaValue; //счётчик количества частиц в очереди на зарождение в этом цикле 
  	dimAll(127);

  	//go over particles and update matrix cells on the way
  	for (int i = 0; i < enlargedObjectNUM; i++) {
    	if (!trackingObjectIsShift[i] && step) {
			starfield2Emit(i);
      		step--;
    	}
    	if (trackingObjectIsShift[i]) { // particle->isAlive
      		particlesUpdate2(i);
	      	//generate RGB values for particle
      		CRGB baseRGB = CHSV(trackingObjectHue[i], 255, 255); // particles[i].hue
      		baseRGB.nscale8(trackingObjectState[i]);//эквивалент
      		drawPixelXYF(trackingObjectPosX[i], trackingObjectPosY[i], baseRGB);
    	}
  	}
}

// =====================================
//               DropInWater
//                © Stepko
//        Adaptation © SlingMaster
// =====================================
void FieryLedLampEffectDropInWater::setup()
{
#define Sat (255)
#define MaxRad WIDTH + HEIGHT

    hue = scale * 2.55;
    for (int i = 0; i < ((HEIGHT + WIDTH) / 8) - 1; i++)  {
    	posx[i] = random(WIDTH - 1);
    	posy[i] = random(HEIGHT - 1);
    	rad[i] = random(-1, MaxRad);
    }
}
void FieryLedLampEffect::drawCircleF(float x0, float y0, float radius, CRGB color) {
  	float x = 0, y = radius, error = 0;
  	float delta = 1. - 2. * radius;

  	while (y >= 0) {
    	drawPixelXYF(fmod(x0 + x + WIDTH, WIDTH), y0 + y, color); // сделал, чтобы круги были бесшовными по оси х
    	drawPixelXYF(fmod(x0 + x + WIDTH, WIDTH), y0 - y, color);
    	drawPixelXYF(fmod(x0 - x + WIDTH, WIDTH), y0 + y, color);
    	drawPixelXYF(fmod(x0 - x + WIDTH, WIDTH), y0 - y, color);
    	error = 2. * (delta + y) - 1.;
    	if (delta < 0 && error <= 0) {
      		++x;
      		delta += 2. * x + 1.;
      		continue;
    	}
    	error = 2. * (delta - x) - 1.;
    	if (delta > 0 && error > 0) {
      		--y;
      		delta += 1. - 2. * y;
      		continue;
    	}
    	++x;
    	delta += 2. * (x - y);
    	--y;
  	}
}

void FieryLedLampEffectDropInWater::updateInner()
{
	CRGBPalette16 currentPalette(PartyColors_p);

	fill_solid( currentPalette, 16, CHSV(hue, Sat, 230));
  	currentPalette[10] = CHSV(hue, Sat - 60, 255);
  	currentPalette[9] = CHSV(hue, 255 - Sat, 210);
  	currentPalette[8] = CHSV(hue, 255 - Sat, 210);
  	currentPalette[7] = CHSV(hue, Sat - 60, 255);
  	fillAll(ColorFromPalette(currentPalette, 1));

  	for (uint8_t i = ((HEIGHT + WIDTH) / 8 - 1); i > 0 ; i--) {
    	drawCircle(posx[i], posy[i], rad[i], ColorFromPalette(currentPalette, (256 / 16) * 8.5 - rad[i]));
    	drawCircle(posx[i], posy[i], rad[i] - 1, ColorFromPalette(currentPalette, (256 / 16) * 7.5 - rad[i]));
    	if (rad[i] >= MaxRad) {
      		rad[i] = 0; // random(-1, MaxRad);
      		posx[i] = random(WIDTH);
      		posy[i] = random(HEIGHT);
    	} else {
      		rad[i]++;
    	}
  	}
  	if (scale == 100) {
    	hue++;
  	}
  	blur2d(leds, WIDTH, HEIGHT, 64);
}

// ============= ЭФФЕКТ Капли на стекле ===============
// https://github.com/DmytroKorniienko/FireLamp_JeeUI/blob/master/src/effects.cpp
void FieryLedLampEffectDrops::setup()
{
	setCurrentPalette();

    enlargedObjectNUM = map(speed, 1, 255, 1, trackingOBJECT_MAX_COUNT);

    for (uint8_t i = 0U; i < enlargedObjectNUM; i++)
    {
      	trackingObjectPosX[i] = random8(WIDTH);
      	trackingObjectPosY[i] = random8(HEIGHT);
      	trackingObjectSpeedY[i] = random8(150, 250) / 100.;
    	trackingObjectState[i] = random8(127U, 255U);
    }
    hue = scale * 2.55;
}

void FieryLedLampEffectDrops::updateInner()
{
	float speedfactor = 0.136f; // фиксируем хорошую скорость

	dimAll(246); // для фиксированной скорости
	CHSV color;

  	for (uint8_t i = 0U; i < enlargedObjectNUM; i++)
  	{
    	trackingObjectPosY[i] -= trackingObjectSpeedY[i] * speedfactor;

    	if (scale == 100U) {
      		color = rgb2hsv_approximate(CRGB::Gray);
      		color.val = trackingObjectState[i];
    	} else if (scale == 1U) {
      		color = CHSV(++hue, 255, trackingObjectState[i]);
    	} else {
      		color = CHSV(hue, 255, trackingObjectState[i]);
    	}
    	drawPixelXYF(trackingObjectPosX[i], trackingObjectPosY[i], color);

#define GLUK 20 // вероятность горизонтального сдвига капли
    	if (random8() < GLUK) {
      		trackingObjectPosX[i] = (uint8_t)(trackingObjectPosX[i] + WIDTH - 1U + random8(3U)) % WIDTH ;
      		trackingObjectState[i] = random8(196, 255);
    	}

    	if (trackingObjectPosY[i] < -1) {
      		trackingObjectPosX[i] = random8(WIDTH);
      		trackingObjectPosY[i] = random8(HEIGHT - HEIGHT / 2, HEIGHT);
      		trackingObjectSpeedY[i] = random8(150, 250) / 100.;
      		trackingObjectState[i] = random8(127U, 255U);
    	}
  	}
}

// ============= Эффект Кипение ===============
// (c) SottNick
//по мотивам LDIRKO Ленд - эффект номер 10
//...ldir... Yaroslaw Turbin, 18.11.2020
//https://vk.com/ldirko
//https://www.reddit.com/user/ldirko/

void FieryLedLampEffectLLand::setup()
{
	setCurrentPalette();
    deltaValue = 10U * ((scale - 1U) % 11U + 1U);// значения от 1 до 11
	ff_y=0;
	ff_z=0;
}

void FieryLedLampEffectLLand::updateInner()
{
	hue2 += 32U;
  	if (hue2 < 32U)
    	hue++;
  	ff_y += 16U;
	for (uint8_t y = 0; y < HEIGHT; y++)
    	for (uint16_t x = 0; x < WIDTH; x++)
			drawPixelXY(x, y, ColorFromPalette (*curPalette, map(inoise8(x * deltaValue, y * deltaValue - ff_y, ff_z) - y * 255 / (HEIGHT - 1), 0, 255, 205, 255) + hue, 255));
  	ff_z++;
}

// ------------------------------ ЭФФЕКТ КОЛЬЦА / КОДОВЫЙ ЗАМОК ----------------------
// (c) SottNick
// из-за повторного использоваия переменных от других эффектов теперь в этом коде невозможно что-то понять.
// поэтому для понимания придётся сперва заменить названия переменных на человеческие. но всё равно это песец, конечно.
//uint8_t deltaHue2; // максимальне количество пикселей в кольце (толщина кольца) от 1 до HEIGHT / 2 + 1
//uint8_t deltaHue; // количество колец от 2 до HEIGHT
//uint8_t noise3d[1][1][HEIGHT]; // начальный оттенок каждого кольца (оттенка из палитры) 0-255
//uint8_t shiftValue[HEIGHT]; // местоположение начального оттенка кольца 0-WIDTH-1
//uint8_t shiftHue[HEIGHT]; // 4 бита на ringHueShift, 4 на ringHueShift2
////ringHueShift[ringsCount]; // шаг градиета оттенка внутри кольца -8 - +8 случайное число
////ringHueShift2[ringsCount]; // обычная скорость переливания оттенка всего кольца -8 - +8 случайное число
//uint8_t deltaValue; // кольцо, которое в настоящий момент нужно провернуть
//uint8_t step; // оставшееся количество шагов, на которое нужно провернуть активное кольцо - случайное от WIDTH/5 до WIDTH-3
//uint8_t hue, hue2; // количество пикселей в нижнем (hue) и верхнем (hue2) кольцах

void FieryLedLampEffectRings::setup()
{
    setCurrentPalette();

    deltaHue2 = (scale - 1U) % 11U + 1U; // толщина кольца от 1 до 11 для каждой из палитр
    deltaHue = HEIGHT / deltaHue2 + ((HEIGHT % deltaHue2 == 0U) ? 0U : 1U); // количество колец
    hue2 = deltaHue2 - (deltaHue2 * deltaHue - HEIGHT) / 2U; // толщина верхнего кольца. может быть меньше нижнего
    hue = HEIGHT - hue2 - (deltaHue - 2U) * deltaHue2; // толщина нижнего кольца = всё оставшееся
    for (uint8_t i = 0; i < deltaHue; i++)
    {
      	noise3d[i] = random8(257U - WIDTH / 2U); // начальный оттенок кольца из палитры 0-255 за минусом длины кольца, делённой пополам
      	shiftHue[i] = random8();
      	shiftValue[i] = 0U; //random8(WIDTH); само прокрутится постепенно
      	step = 0U;
      	deltaValue = random8(deltaHue);
    }
}

void FieryLedLampEffectRings::updateInner()
{
	uint8_t h, x, y;
  	for (uint8_t i = 0; i < deltaHue; i++)
  	{
    	if (i != deltaValue) // если это не активное кольцо
    	{
      		h = shiftHue[i] & 0x0F; // сдвигаем оттенок внутри кольца
      		if (h > 8U)
        		noise3d[i]--;
      		else
        		noise3d[i]++;
    	}
    	else
    	{
      		if (step == 0) // если сдвиг активного кольца завершён, выбираем следующее
      		{
        		deltaValue = random8(deltaHue);
        		do {
          			step = WIDTH - 3U - random8((WIDTH - 3U) * 2U); // проворот кольца от хз до хз
        		} while (step < WIDTH / 5U || step > 255U - WIDTH / 5U);
      		}
      		else
      		{
        		if (step > 127U)
        		{
          			step++;
          			shiftValue[i] = (shiftValue[i] + 1U) % WIDTH;
        		}
        		else
        		{
          			step--;
          			shiftValue[i] = (shiftValue[i] - 1U + WIDTH) % WIDTH;
        		}
      		}
    	}
    	// отрисовываем кольца
    	h = (shiftHue[i] >> 4) & 0x0F; // берём шаг для градиента вутри кольца
    	if (h > 8U)
      		h = 7U - h;
    	for (uint8_t j = 0U; j < ((i == 0U) ? hue : ((i == deltaHue - 1U) ? hue2 : deltaHue2)); j++) // от 0 до (толщина кольца - 1)
    	{
      		y = i * deltaHue2 + j - ((i == 0U) ? 0U : deltaHue2 - hue);
      		// mod для чётных скоростей by @kostyamat - получается какая-то другая фигня. не стоит того
      		for (uint8_t k = 0; k < WIDTH / 2U; k++) // полукольцо
      		{
        		x = (shiftValue[i] + k) % WIDTH; // первая половина кольца
        		leds[XY(x, y)] = ColorFromPalette(*curPalette, noise3d[i] + k * h);
        		x = (WIDTH - 1 + shiftValue[i] - k) % WIDTH; // вторая половина кольца (зеркальная первой)
        		leds[XY(x, y)] = ColorFromPalette(*curPalette, noise3d[i] + k * h);
      		}
      		if (WIDTH & 0x01) //(WIDTH % 2U > 0U) // если число пикселей по ширине матрицы нечётное, тогда не забываем и про среднее значение
      		{
        		x = (shiftValue[i] + WIDTH / 2U) % WIDTH;
        		leds[XY(x, y)] = ColorFromPalette(*curPalette, noise3d[i] + WIDTH / 2U * h);
      		}
    	}
  	}
}

FieryLedLampEffectComet::~FieryLedLampEffectComet()
{
	for(int i=0;i<WIDTH;i++)
		delete[] noise3d[i];
	delete[] noise3d;
}
// Кометы обычные
void FieryLedLampEffectComet::setup()
{
	noise32_x=noise32_y=noise32_z=0;
	noise3d=new uint8_t*[WIDTH];
	for(int i=0;i<WIDTH;i++)
		noise3d[i]=new uint8_t[HEIGHT];
}
void FieryLedLampEffectComet::updateInner()
{
	dimAll(254U); // < -- затухание эффекта для последующего кадра
  	CRGB _eNs_color = CHSV(millis() / scale * 2, 255, 255);
  	leds[XY(CENTER_X_MINOR, CENTER_Y_MINOR)] += _eNs_color;
  	leds[XY(CENTER_X_MINOR + 1, CENTER_Y_MINOR)] += _eNs_color;
  	leds[XY(CENTER_X_MINOR, CENTER_Y_MINOR + 1)] += _eNs_color;
  	leds[XY(CENTER_X_MINOR + 1, CENTER_Y_MINOR + 1)] += _eNs_color;

  	// Noise
  	noise32_x += 1500;
  	noise32_y += 1500;
  	noise32_z += 1500;
  	uint32_t scale32_x = 8000;
  	uint32_t scale32_y = 8000;
  	
	FillNoise(noise3d, noise32_x, noise32_y, noise32_z, scale32_x, scale32_y, 0);

  	MoveFractionalNoiseX(noise3d, WIDTH / 2U - 1U);
  	MoveFractionalNoiseY(noise3d, HEIGHT / 2U - 1U);
}

// Кометы белые и одноцветные
FieryLedLampEffectCometColor::~FieryLedLampEffectCometColor()
{
	for(int i=0;i<WIDTH;i++)
		delete[] noise3d[i];
	delete[] noise3d;
}
void FieryLedLampEffectCometColor::setup()
{
	noise32_x=noise32_y=noise32_z=0;
	noise3d=new uint8_t*[WIDTH];
	for(int i=0;i<WIDTH;i++)
		noise3d[i]=new uint8_t[HEIGHT];
}
void FieryLedLampEffectCometColor::updateInner()
{
	dimAll(254U); // < -- затухание эффекта для последующего кадра
  	CRGB _eNs_color = CRGB::White;

  	if (scale < 100)
		_eNs_color = CHSV((scale) * 2.57, 255, 255); // 2.57 вместо 2.55, потому что при 100 будет белый цвет
  	leds[XY(CENTER_X_MINOR, CENTER_Y_MINOR)] += _eNs_color;
  	leds[XY(CENTER_X_MINOR + 1, CENTER_Y_MINOR)] += _eNs_color;
  	leds[XY(CENTER_X_MINOR, CENTER_Y_MINOR + 1)] += _eNs_color;
  	leds[XY(CENTER_X_MINOR + 1, CENTER_Y_MINOR + 1)] += _eNs_color;

  	// Noise
  	noise32_x += 1500;
  	noise32_y += 1500;
  	noise32_z += 1500;
  	uint16_t scale32_x = 8000;
  	uint16_t scale32_y = 8000;
  	FillNoise(noise3d, noise32_x, noise32_y, noise32_z, scale32_x, scale32_y, 0);
  	MoveFractionalNoiseX(noise3d, WIDTH / 2U - 1U);
  	MoveFractionalNoiseY(noise3d, HEIGHT / 2U - 1U);
}

// NoiseSmearing(by StefanPetrick) Effect mod for GyverLamp by PalPalych
void FieryLedLampEffectCometCount::setup()
{
	// 2 comets
    hue = 0U; // 0xFF0000
    hue2 = 43U; // 0xFFFF00

	deltaHue = 171U; //0x0000FF;

    trackingObjectState[0] = WIDTH / 8;
    trackingObjectState[1] = HEIGHT / 8;
    trackingObjectShift[0] = 255. / (WIDTH - 1. - trackingObjectState[0] - trackingObjectState[0]);
    trackingObjectShift[1] = 255. / (HEIGHT - 1. - trackingObjectState[1] - trackingObjectState[1]);
    trackingObjectState[2] = WIDTH / 4;
    trackingObjectState[3] = HEIGHT / 4;
    trackingObjectShift[2] = 255. / (WIDTH - 1. - trackingObjectState[2] - trackingObjectState[2]); // ((WIDTH>10)?9.:5.));
    trackingObjectShift[3] = 255. / (HEIGHT - 1. - trackingObjectState[3] - trackingObjectState[3]); //- ((HEIGHT>10)?9.:5.));
}

void FieryLedLampEffectCometCount::updateInner()
{
	dimAll(255U - scale * 2);

	// gelb im Kreis
	byte xx, yy;
	if(count == 2)
  	{
		xx = trackingObjectState[0] + sin8( millis() / 10) / trackingObjectShift[0];// / 22;
  		yy = trackingObjectState[1] + cos8( millis() / 10) / trackingObjectShift[1];// / 22;
	}
	else
	{
		xx = trackingObjectState[0] + sin8( millis() / 10) / trackingObjectShift[0];// / 22;
  		yy = trackingObjectState[1] + cos8( millis() / 9) / trackingObjectShift[1];// / 22;
	}

  	if (xx < WIDTH && yy < HEIGHT)
	{
		if(count == 2)
    		leds[XY( xx, yy)] = CHSV(hue2 , 255, 255);//0xFFFF00;
		else
			leds[XY( xx, yy)] += CHSV(deltaHue , 255, 255);//0x0000FF;
	}
	// rot in einer Acht
	if(count)
	{
		xx = trackingObjectState[2] + sin8( millis() / 46) / trackingObjectShift[2];// / 32;
		yy = trackingObjectState[3] + cos8( millis() / 15) / trackingObjectShift[3];// / 32;
	}
	else
	{
		xx = trackingObjectState[2] + sin8( millis() / 10) / trackingObjectShift[2];// / 32;
  		yy = trackingObjectState[3] + cos8( millis() / 7) / trackingObjectShift[3];// / 32;
	}
	if (xx < WIDTH && yy < HEIGHT)
		leds[XY( xx, yy)] = CHSV(hue , 255, 255);//0xFF0000;

	if(count==3)
		leds[XY( CENTER_X_MINOR, CENTER_Y_MINOR)] += CHSV(hue2 , 255, 255);//0xFFFF00;
	// Noise
	/*noise32_x[0] += 3000;
	noise32_y[0] += 3000;
	noise32_z[0] += 3000;
	scale32_x[0] = 8000;
	scale32_y[0] = 8000;
	FillNoise(0);
	if(count==2)
	{
		MoveFractionalNoiseX(3, 0.33);
		MoveFractionalNoiseY(3);
	}
	else
	{
		MoveFractionalNoiseX(2);
  		MoveFractionalNoiseY(2, 0.33);
	}*/
}

// ================Contacts==============
//             © Yaroslaw Turbin
//        Adaptation © SlingMaster
//          modifed © alvikskor
//              Контакты
// =====================================

void FieryLedLampEffectContacts::setup()
{
	//FPSdelay = 80U;
    FastLED.clear();
}

static const uint8_t exp_gamma[256] PROGMEM = {
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,
    1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
    1,   2,   2,   2,   2,   2,   2,   2,   2,   2,   3,   3,   3,   3,   3,
    4,   4,   4,   4,   4,   5,   5,   5,   5,   5,   6,   6,   6,   7,   7,
    7,   7,   8,   8,   8,   9,   9,   9,   10,  10,  10,  11,  11,  12,  12,
    12,  13,  13,  14,  14,  14,  15,  15,  16,  16,  17,  17,  18,  18,  19,
    19,  20,  20,  21,  21,  22,  23,  23,  24,  24,  25,  26,  26,  27,  28,
    28,  29,  30,  30,  31,  32,  32,  33,  34,  35,  35,  36,  37,  38,  39,
    39,  40,  41,  42,  43,  44,  44,  45,  46,  47,  48,  49,  50,  51,  52,
    53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  67,
    68,  70,  71,  72,  73,  74,  75,  77,  78,  79,  80,  82,  83,  84,  85,
    87,  89,  91,  92,  93,  95,  96,  98,  99,  100, 101, 102, 105, 106, 108,
    109, 111, 112, 114, 115, 117, 118, 120, 121, 123, 125, 126, 128, 130, 131,
    133, 135, 136, 138, 140, 142, 143, 145, 147, 149, 151, 152, 154, 156, 158,
    160, 162, 164, 165, 167, 169, 171, 173, 175, 177, 179, 181, 183, 185, 187,
    190, 192, 194, 196, 198, 200, 202, 204, 207, 209, 211, 213, 216, 218, 220,
    222, 225, 227, 229, 232, 234, 236, 239, 241, 244, 246, 249, 251, 253, 254,
    255
};

void FieryLedLampEffectContacts::updateInner()
{
	int a = millis() / floor((255 - speed) / 10);
  	hue = floor(scale / 14);
  	for (int x = 0; x < WIDTH; x++) {
    	for (int y = 0; y < HEIGHT; y++) {
      		int index = XY(x, y);
      		uint8_t color1 = pgm_read_byte(&exp_gamma[sin8(cos8((x * 7 +a/5)) - cos8((y * 10) +a/3)/4+a )]);
      		uint8_t color2 = pgm_read_byte(&exp_gamma[(sin8(x * 16 + a / 3) + cos8(y * 8 + a / 2)) / 2]);
      		uint8_t color3 = pgm_read_byte(&exp_gamma[sin8(cos8(x * 8 + a / 3) + sin8(y * 8 + a / 4) + a)]);
      		switch(hue)
			{
			case 0:
        		leds[index].b = color3 >> 2;
        		leds[index].g = color2;
        		leds[index].r = 0;
				break;
      		case 1:
				leds[index].b = color1;
				leds[index].g = 0;
				leds[index].r = color3 >> 2;
				break;
      		case 2:
				leds[index].b = 0;
				leds[index].g = color1 >> 2;
				leds[index].r = color3;
				break;
      		case 3:
				leds[index].b = color1;
				leds[index].g = color2;
				leds[index].r = color3;
				break;
      		case 4:
				leds[index].b = color3;
				leds[index].g = color1;
				leds[index].r = color2;
				break;
      		case 5:
				leds[index].b = color2;
				leds[index].g = color3;
				leds[index].r = color1;
				break;
      		case 6:
				leds[index].b = color3;
				leds[index].g = color1;
				leds[index].r = color2;
				break;
			}
      	}
    }
}

// ------------- конфетти --------------
#define FADE_OUT_SPEED        (70U)                         // скорость затухания
void FieryLedLampEffectSparkles::setup()
{
	for (uint16_t i = 0; i < NUM_LEDS; i++)
    	if (random8(3U))
        	leds[i].nscale8(random8());
      	else
        	leds[i] = 0U;
}

void FieryLedLampEffectSparkles::updateInner()
{
	for (uint8_t i = 0; i < scale; i++)
  	{
    	uint8_t x = random8(WIDTH);
    	uint8_t y = random8(HEIGHT);
    	if (getPixColorXY(x, y) == 0U)
    	{
      		leds[XY(x, y)] = CHSV(random8(), 255U, 255U);
    	}
  	}
  	dimAll(256U - FADE_OUT_SPEED);
}

// ------------------------------ ЭФФЕКТ КУБИК РУБИКА 2D ----------------------
// (c) SottNick

#define PAUSE_MAX 7 // пропустить 7 кадров после завершения анимации сдвига ячеек

//uint8_t noise3d[1][WIDTH][HEIGHT]; // тут используем только нулевую колонку и нулевую строку. просто для экономии памяти взяли существующий трёхмерный массив
//uint8_t hue2; // осталось шагов паузы
//uint8_t step; // текущий шаг сдвига (от 0 до deltaValue-1)
//uint8_t deltaValue; // всего шагов сдвига (до razmer? до (razmer?+1)*shtuk?)
//uint8_t deltaHue, deltaHue2; // глобальный X и глобальный Y нашего "кубика"

void FieryLedLampEffectCube2D::setup()
{
  	uint8_t x, y;
  	CRGB color;

    setCurrentPalette();
    FastLED.clear();

    razmerX = (scale - 1U) % 11U + 1U; // размер ячейки от 1 до 11 пикселей для каждой из 9 палитр
    razmerY = razmerX;
    if (speed & 0x01) // по идее, ячейки не обязательно должны быть квадратными, поэтому можно тут поизвращаться
    	razmerY = (razmerY << 1U) + 1U;

    shtukY = HEIGHT / (razmerY + 1U);
    if (shtukY < 2U)
    	shtukY = 2U;
    y = HEIGHT / shtukY - 1U;
    if (razmerY > y)
    	razmerY = y;
    poleY = (razmerY + 1U) * shtukY;
    shtukX = WIDTH / (razmerX + 1U);
    if (shtukX < 2U)
      shtukX = 2U;
    x = WIDTH / shtukX - 1U;
    if (razmerX > x)
      razmerX = x;
    poleX = (razmerX + 1U) * shtukX;
    seamlessX = (poleX == WIDTH);
    deltaHue = 0U;
    deltaHue2 = 0U;
    globalShiftX = 0;
    globalShiftY = 0;

    for (uint8_t j = 0U; j < shtukY; j++)
    {
    	y = j * (razmerY + 1U); // + deltaHue2 т.к. оно =0U
      	for (uint8_t i = 0U; i < shtukX; i++)
      	{
        	x = i * (razmerX + 1U); // + deltaHue т.к. оно =0U
        	if (scale == 100U)
          		color = CHSV(45U, 0U, 128U + random8(128U));
        	else
          		color = ColorFromPalette(*curPalette, random8());
        	for (uint8_t k = 0U; k < razmerY; k++)
          		for (uint8_t m = 0U; m < razmerX; m++)
            		leds[XY(x + m, y + k)] = color;
      	}
    }
    step = 4U; // текущий шаг сдвига первоначально с перебором (от 0 до deltaValue-1)
    deltaValue = 4U; // всего шагов сдвига (от razmer? до (razmer?+1) * shtuk?)
    hue2 = 0U; // осталось шагов паузы
	krutimVertikalno=false;
}

void FieryLedLampEffectCube2D::updateInner()
{
	uint8_t x, y;
	int8_t shift, kudaVse;
	uint8_t anim0; // будем считать тут начальный пиксель для анимации сдвига строки/колонки
	CRGB color, color2;
  	//двигаем, что получилось...
  	if (hue2 == 0 && step < deltaValue) // если пауза закончилась, а цикл вращения ещё не завершён
  	{
    	step++;
    	if (krutimVertikalno)
    	{
      		for (uint8_t i = 0U; i < shtukX; i++)
      		{
        		x = (deltaHue + i * (razmerX + 1U)) % WIDTH;
        		if (noise3d[i][0] > 0) // в нулевой ячейке храним оставшееся количество ходов прокрутки
        		{
          			noise3d[i][0]--;
          			shift = noise3d[i][1] - 1; // в первой ячейке храним направление прокрутки

          			if (globalShiftY == 0)
            			anim0 = (deltaHue2 == 0U) ? 0U : deltaHue2 - 1U;
          			else if (globalShiftY > 0)
            			anim0 = deltaHue2;
          			else
            			anim0 = deltaHue2 - 1U;

          			if (shift < 0) // если крутим столбец вниз
          			{
            			color = leds[XY(x, anim0)];                                   // берём цвет от нижней строчки
            			for (uint8_t k = anim0; k < anim0 + poleY - 1; k++)
            			{
              				color2 = leds[XY(x, k + 1)];                                // берём цвет от строчки над нашей
              				for (uint8_t m = x; m < x + razmerX; m++)
                				leds[XY(m % WIDTH, k)] = color2;                          // копируем его на всю нашу строку
            			}
            			for   (uint8_t m = x; m < x + razmerX; m++)
              				leds[XY(m % WIDTH, anim0 + poleY - 1)] = color;             // цвет нижней строчки копируем на всю верхнюю
          			}
          			else if (shift > 0) // если крутим столбец вверх
          			{
            			color = leds[XY(x, anim0 + poleY - 1)];                       // берём цвет от верхней строчки
            			for (uint8_t k = anim0 + poleY - 1; k > anim0 ; k--)
            			{
              				color2 = leds[XY(x, k - 1)];                                // берём цвет от строчки под нашей
              				for (uint8_t m = x; m < x + razmerX; m++)
                			leds[XY(m % WIDTH, k)] = color2;                          // копируем его на всю нашу строку
            			}
            			for   (uint8_t m = x; m < x + razmerX; m++)
              				leds[XY(m % WIDTH, anim0)] = color;                         // цвет верхней строчки копируем на всю нижнюю
          			}
        		}
      		}
    	}
    	else
    	{
      		for (uint8_t j = 0U; j < shtukY; j++)
      		{
        		y = deltaHue2 + j * (razmerY + 1U);
        		if (noise3d[0][j] > 0) // в нулевой ячейке храним оставшееся количество ходов прокрутки
        		{
          			noise3d[0][j]--;
          			shift = noise3d[1][j] - 1; // в первой ячейке храним направление прокрутки

          			if (seamlessX)
            			anim0 = 0U;
          			else if (globalShiftX == 0)
            			anim0 = (deltaHue == 0U) ? 0U : deltaHue - 1U;
          			else if (globalShiftX > 0)
            			anim0 = deltaHue;
          			else
            			anim0 = deltaHue - 1U;

          			if (shift < 0) // если крутим строку влево
          			{
            			color = leds[XY(anim0, y)];                            // берём цвет от левой колонки (левого пикселя)
            			for (uint8_t k = anim0; k < anim0 + poleX - 1; k++)
            			{
              				color2 = leds[XY(k + 1, y)];                         // берём цвет от колонки (пикселя) правее
              				for (uint8_t m = y; m < y + razmerY; m++)
                				leds[XY(k, m)] = color2;                           // копируем его на всю нашу колонку
            			}
            			for   (uint8_t m = y; m < y + razmerY; m++)
              				leds[XY(anim0 + poleX - 1, m)] = color;              // цвет левой колонки копируем на всю правую
          			}
          			else if (shift > 0) // если крутим столбец вверх
          			{
            			color = leds[XY(anim0 + poleX - 1, y)];                // берём цвет от правой колонки
            			for (uint8_t k = anim0 + poleX - 1; k > anim0 ; k--)
            			{
              				color2 = leds[XY(k - 1, y)];                         // берём цвет от колонки левее
              				for (uint8_t m = y; m < y + razmerY; m++)
                				leds[XY(k, m)] = color2;                           // копируем его на всю нашу колонку
            			}
            			for   (uint8_t m = y; m < y + razmerY; m++)
              				leds[XY(anim0, m)] = color;                          // цвет правой колонки копируем на всю левую
          			}
        		}
      		}
    	}
  	}
  	else if (hue2 != 0U) // пропускаем кадры после прокрутки кубика (делаем паузу)
    	hue2--;

  	if (step >= deltaValue) // если цикл вращения завершён, меняем местами соответствующие ячейки (цвет в них) и точку первой ячейки
  	{
    	step = 0U;
    	hue2 = PAUSE_MAX;
    	//если часть ячеек двигалась на 1 пиксель, пододвигаем глобальные координаты начала
    	deltaHue2 = deltaHue2 + globalShiftY; //+= globalShiftY;
    	globalShiftY = 0;
    	//deltaHue += globalShiftX; для бесшовной не годится
    	deltaHue = (WIDTH + deltaHue + globalShiftX) % WIDTH;
    	globalShiftX = 0;

    	//пришла пора выбрать следующие параметры вращения
    	kudaVse = 0;
    	krutimVertikalno = random8(2U);
    	if (krutimVertikalno) // идём по горизонтали, крутим по вертикали (столбцы двигаются)
    	{
      		for (uint8_t i = 0U; i < shtukX; i++)
      		{
        		noise3d[i][1] = random8(3);
        		shift = noise3d[i][1] - 1; // в первой ячейке храним направление прокрутки
        		if (kudaVse == 0)
          			kudaVse = shift;
        		else if (shift != 0 && kudaVse != shift)
          			kudaVse = 50;
      		}
      		deltaValue = razmerY + ((deltaHue2 - kudaVse >= 0 && deltaHue2 - kudaVse + poleY < (int)HEIGHT) ? random8(2U) : 1U);
      		if (deltaValue == razmerY) // значит полюбому kudaVse было = (-1, 0, +1) - и для нуля в том числе мы двигаем весь куб на 1 пиксель
      		{
        		globalShiftY = 1 - kudaVse; //временно на единичку больше, чем надо
        		for (uint8_t i = 0U; i < shtukX; i++)
          			if (noise3d[i][1] == 1U) // если ячейка никуда не планировала двигаться
          			{
            			noise3d[i][1] = globalShiftY;
            			noise3d[i][0] = 1U; // в нулевой ячейке храним количество ходов сдвига
          			}
          			else
            			noise3d[i][0] = deltaValue; // в нулевой ячейке храним количество ходов сдвига
        		globalShiftY--;
      		}
      		else
      		{
        		x = 0;
        		for (uint8_t i = 0U; i < shtukX; i++)
          		if (noise3d[i][1] != 1U)
          		{
            		y = random8(shtukY);
            		if (y > x)
              			x = y;
            		noise3d[i][0] = deltaValue * (x + 1U); // в нулевой ячейке храним количество ходов сдвига
          		}
        		deltaValue = deltaValue * (x + 1U);
      		}
    	}
    	else // идём по вертикали, крутим по горизонтали (строки двигаются)
    	{
      		for (uint8_t j = 0U; j < shtukY; j++)
      		{
        		noise3d[1][j] = random8(3);
        		shift = noise3d[1][j] - 1; // в первой ячейке храним направление прокрутки
        		if (kudaVse == 0)
          			kudaVse = shift;
        		else if (shift != 0 && kudaVse != shift)
          			kudaVse = 50;
      		}
      		if (seamlessX)
        		deltaValue = razmerX + ((kudaVse < 50) ? random8(2U) : 1U);
      		else
        		deltaValue = razmerX + ((deltaHue - kudaVse >= 0 && deltaHue - kudaVse + poleX < (int)WIDTH) ? random8(2U) : 1U);
      		if (deltaValue == razmerX) // значит полюбому kudaVse было = (-1, 0, +1) - и для нуля в том числе мы двигаем весь куб на 1 пиксель
      		{
        		globalShiftX = 1 - kudaVse; //временно на единичку больше, чем надо
        		for (uint8_t j = 0U; j < shtukY; j++)
          		if (noise3d[1][j] == 1U) // если ячейка никуда не планировала двигаться
          		{
            		noise3d[1][j] = globalShiftX;
            		noise3d[0][j] = 1U; // в нулевой ячейке храним количество ходов сдвига
          		}
          		else
            		noise3d[0][j] = deltaValue; // в нулевой ячейке храним количество ходов сдвига
        		globalShiftX--;
      		}
      		else
      		{
        		y = 0;
        		for (uint8_t j = 0U; j < shtukY; j++)
          			if (noise3d[1][j] != 1U)
          			{
            			x = random8(shtukX);
            			if (x > y)
              				y = x;
            			noise3d[0][j] = deltaValue * (x + 1U); // в нулевой ячейке храним количество ходов сдвига
          			}
        		deltaValue = deltaValue * (y + 1U);
      		}
    	}
  	}
}

void FieryLedLampEffect::fillNoiseLED(const TProgmemRGBPalette16 *currentPalette)
{
	uint8_t dataSmoothing = 0;
  	if (speed < 50)
  	{
    	dataSmoothing = 200 - (speed * 4);
  	}
  	for (uint8_t i = 0; i < MAX_DIMENSION; i++)
  	{
    	int32_t ioffset = scale * i;
    	for (uint8_t j = 0; j < MAX_DIMENSION; j++)
    	{
      		int32_t joffset = scale * j;
      		uint8_t data = inoise8(x + ioffset, y + joffset, z);

      		data = qsub8(data, 16);
      		data = qadd8(data, scale8(data, 39));

      		if (dataSmoothing)
      		{
        		uint8_t olddata = noise[i][j];
        		uint8_t newdata = scale8( olddata, dataSmoothing) + scale8( data, 256 - dataSmoothing);
        		data = newdata;
      		}
      		noise[i][j] = data;
    	}
  	}
  	z += speed;
	// apply slow drift to X and Y, just for visual variation.
  	x += speed / 8;
  	y -= speed / 16;

  	for (uint8_t i = 0; i < WIDTH; i++)
  	{
    	for (uint8_t j = 0; j < HEIGHT; j++)
    	{
      		uint8_t index = noise[j][i];
      		uint8_t bri =   noise[i][j];
      		// if this palette is a 'loop', add a slowly-changing base value
      		if ( colorLoop)
      		{
        		index += ihue;
      		}
      		// brighten up, as the color palette itself often contains the
      		// light/dark dynamic range desired
      		if ( bri > 127 )
      		{
        		bri = 255;
      		}
      		else
      		{
        		bri = dim8_raw( bri * 2);
      		}
      		CRGB color = ColorFromPalette( *currentPalette, index, bri);      
      		drawPixelXY(i, j, color);                             //leds[XY(i, j)] = color;
    	}
  	}
  	ihue += 1;
}

void FieryLedLampEffectLava::setup()
{
    colorLoop = 0;
}
void FieryLedLampEffectLava::updateInner()
{
	fillNoiseLED(&LavaColors_p);
}

// ------------- Светлячки 2 - Светлячки в банке - Мотыльки - Лампа с мотыльками --------------
// (c) SottNick

//#define trackingOBJECT_MAX_COUNT           (100U) // максимальное количество мотыльков
#define BUTTERFLY_FIX_COUNT               (20U) // количество мотыльков для режима, когда бегунок Масштаб регулирует цвет
//float trackingObjectPosX[trackingOBJECT_MAX_COUNT];
//float trackingObjectPosY[trackingOBJECT_MAX_COUNT];
//float trackingObjectSpeedX[trackingOBJECT_MAX_COUNT];
//float trackingObjectSpeedY[trackingOBJECT_MAX_COUNT];
//float trackingObjectShift[trackingOBJECT_MAX_COUNT];
//uint8_t trackingObjectHue[trackingOBJECT_MAX_COUNT];
//uint8_t trackingObjectState[trackingOBJECT_MAX_COUNT];

void FieryLedLampEffectButterflyLamp::setup()
{
	deltaHue=0;
    speedfactor = (float)speed / 2048.0f + 0.001f;
    if (isColored) // для режима смены цвета фона фиксируем количество мотыльков
      	deltaValue = (scale > trackingOBJECT_MAX_COUNT) ? trackingOBJECT_MAX_COUNT : scale;
    else
      	deltaValue = BUTTERFLY_FIX_COUNT;
    for (uint8_t i = 0U; i < trackingOBJECT_MAX_COUNT; i++)
    {
      	trackingObjectPosX[i] = random8(WIDTH);
      	trackingObjectPosY[i] = random8(HEIGHT);
      	trackingObjectSpeedX[i] = 0;
      	trackingObjectSpeedY[i] = 0;
      	trackingObjectShift[i] = 0;
      	trackingObjectHue[i] = (isColored) ? random8() : 255U;
      	trackingObjectState[i] = 255U;
    }
    //для инверсии, чтобы сто раз не пересчитывать
    if (scale != 1U)
    	hue = (float)(scale - 1U) * 2.6;
    else
    	hue = random8();
    if(scale == 100U) { // вместо белого будет желтоватая лампа
    	hue2 = 170U;
    	hue = 31U;
    }
    else
    	hue2 = 255U;
}
void FieryLedLampEffectButterflyLamp::updateInner()
{
	bool isWings = speed & 0x01;
  	
	if (isWings && isColored)
    	dimAll(35U); // для крылышков
  	else
    	FastLED.clear();

  	float maxspeed;
  	uint8_t tmp;
  	if (++step >= deltaValue)
    	step = 0U;
  	for (uint8_t i = 0U; i < deltaValue; i++)
  	{
    	trackingObjectPosX[i] += trackingObjectSpeedX[i] * speedfactor;
    	trackingObjectPosY[i] += trackingObjectSpeedY[i] * speedfactor;

    	if (trackingObjectPosX[i] < 0)
      		trackingObjectPosX[i] = (float)(WIDTH - 1) + trackingObjectPosX[i];
    	if (trackingObjectPosX[i] > WIDTH - 1)
      		trackingObjectPosX[i] = trackingObjectPosX[i] + 1 - WIDTH;

    	if (trackingObjectPosY[i] < 0)
    	{
      		trackingObjectPosY[i] = -trackingObjectPosY[i];
      		trackingObjectSpeedY[i] = -trackingObjectSpeedY[i];
    	}
    	if (trackingObjectPosY[i] > HEIGHT - 1U)
    	{
      		trackingObjectPosY[i] = (HEIGHT << 1U) - 2U - trackingObjectPosY[i];
      		trackingObjectSpeedY[i] = -trackingObjectSpeedY[i];
    	}

    	//проворот траектории
    	maxspeed = fabs(trackingObjectSpeedX[i]) + fabs(trackingObjectSpeedY[i]); // максимальная суммарная скорость
    	if (maxspeed == fabs(trackingObjectSpeedX[i] + trackingObjectSpeedY[i]))
    	{
      		if (trackingObjectSpeedX[i] > 0) // правый верхний сектор вектора
      		{
        		trackingObjectSpeedX[i] += trackingObjectShift[i];
        		if (trackingObjectSpeedX[i] > maxspeed) // если вектор переехал вниз
        		{
          			trackingObjectSpeedX[i] = maxspeed + maxspeed - trackingObjectSpeedX[i];
          			trackingObjectSpeedY[i] = trackingObjectSpeedX[i] - maxspeed;
        		}
        		else
          			trackingObjectSpeedY[i] = maxspeed - fabs(trackingObjectSpeedX[i]);
      		}
      		else                           // левый нижний сектор
      		{
        		trackingObjectSpeedX[i] -= trackingObjectShift[i];
        		if (trackingObjectSpeedX[i] + maxspeed < 0) // если вектор переехал вверх
        		{
          			trackingObjectSpeedX[i] = 0 - trackingObjectSpeedX[i] - maxspeed - maxspeed;
          			trackingObjectSpeedY[i] = maxspeed - fabs(trackingObjectSpeedX[i]);
        		}
        		else
          			trackingObjectSpeedY[i] = fabs(trackingObjectSpeedX[i]) - maxspeed;
      		}
    	}
    	else //левый верхний и правый нижний секторы вектора
    	{
      		if (trackingObjectSpeedX[i] > 0) // правый нижний сектор
      		{
        		trackingObjectSpeedX[i] -= trackingObjectShift[i];
        		if (trackingObjectSpeedX[i] > maxspeed) // если вектор переехал наверх
        		{
          			trackingObjectSpeedX[i] = maxspeed + maxspeed - trackingObjectSpeedX[i];
          			trackingObjectSpeedY[i] = maxspeed - trackingObjectSpeedX[i];
        		}
        		else
          			trackingObjectSpeedY[i] = fabs(trackingObjectSpeedX[i]) - maxspeed;
      		}
      		else                           // левый верхний сектор
      		{
        		trackingObjectSpeedX[i] += trackingObjectShift[i];
        		if (trackingObjectSpeedX[i] + maxspeed < 0) // если вектор переехал вниз
        		{
          			trackingObjectSpeedX[i] = 0 - trackingObjectSpeedX[i] - maxspeed - maxspeed;
          			trackingObjectSpeedY[i] = 0 - trackingObjectSpeedX[i] - maxspeed;
        		}
        		else
          			trackingObjectSpeedY[i] = maxspeed - fabs(trackingObjectSpeedX[i]);
      		}
    	}

    	if (trackingObjectState[i] == 255U)
    	{
      		if (step == i && random8(2U) == 0U)//(step == 0U && ((pcnt + i) & 0x01))
      		{
        		trackingObjectState[i] = random8(220U, 244U);
        		trackingObjectSpeedX[i] = (float)random8(101U) / 20.0f + 1.0f;
        		if (random8(2U) == 0U) trackingObjectSpeedX[i] = -trackingObjectSpeedX[i];
        		trackingObjectSpeedY[i] = (float)random8(101U) / 20.0f + 1.0f;
				if (random8(2U) == 0U) trackingObjectSpeedY[i] = -trackingObjectSpeedY[i];
				// проворот траектории
				//trackingObjectShift[i] = (float)random8((fabs(trackingObjectSpeedX[i])+fabs(trackingObjectSpeedY[i]))*2.0+2.0) / 40.0f;
				trackingObjectShift[i] = (float)random8((fabs(trackingObjectSpeedX[i]) + fabs(trackingObjectSpeedY[i])) * 20.0f + 2.0f) / 200.0f;
				if (random8(2U) == 0U) trackingObjectShift[i] = -trackingObjectShift[i];
      		}
    	}
    	else
		{
			if (step == i)
				trackingObjectState[i]++;
			tmp = 255U - trackingObjectState[i];
			if (tmp == 0U || ((uint16_t)(trackingObjectPosX[i] * tmp) % tmp == 0U && (uint16_t)(trackingObjectPosY[i] * tmp) % tmp == 0U))
			{
				trackingObjectPosX[i] = round(trackingObjectPosX[i]);
				trackingObjectPosY[i] = round(trackingObjectPosY[i]);
				trackingObjectSpeedX[i] = 0;
				trackingObjectSpeedY[i] = 0;
				trackingObjectShift[i] = 0;
				trackingObjectState[i] = 255U;
			}
		}

    	if (isWings)
      		drawPixelXYF(trackingObjectPosX[i], trackingObjectPosY[i], CHSV(trackingObjectHue[i], 255U, (trackingObjectState[i] == 255U) ? 255U : 128U + random8(2U) * 111U)); // это процедура рисования с нецелочисленными координатами. ищите её в прошивке
    	else
      		drawPixelXYF(trackingObjectPosX[i], trackingObjectPosY[i], CHSV(trackingObjectHue[i], 255U, trackingObjectState[i])); // это процедура рисования с нецелочисленными координатами. ищите её в прошивке
  	}

  	// постобработка кадра
  	if (isColored) {
    	for (uint8_t i = 0U; i < deltaValue; i++) // ещё раз рисуем всех Мотыльков, которые "сидят на стекле"
      	if (trackingObjectState[i] == 255U)
        	drawPixelXY(trackingObjectPosX[i], trackingObjectPosY[i], CHSV(trackingObjectHue[i], 255U, trackingObjectState[i]));
  	}
  	else {
    	//теперь инверсия всей матрицы
    	if (scale == 1U)
      		if (++deltaHue == 0U) hue++;
    	for (uint16_t i = 0U; i < NUM_LEDS; i++)
      		leds[i] = CHSV(hue, hue2, 255U - leds[i].r);
	}
}

void FieryLedLampEffectForest::setup()
{
    colorLoop = 0;
}
void FieryLedLampEffectForest::updateInner()
{
	fillNoiseLED(&ForestColors_p);
}

// =============== Эффект Lumenjer ================
// (c) SottNick

#define DIMSPEED (254U - 500U / WIDTH / HEIGHT)

void FieryLedLampEffectLumenjer::setup()
{
	if (scale > 100) scale = 100; // чтобы не было проблем при прошивке без очистки памяти
    if (scale > 50)
    	curPalette = firePalettes[(uint8_t)((scale - 50) / 50.0F * ((sizeof(firePalettes) / sizeof(TProgmemRGBPalette16 *)) - 0.01F))];
    else
      	curPalette = palette_arr[(uint8_t)(scale / 50.0F * ((sizeof(palette_arr) / sizeof(TProgmemRGBPalette16 *)) - 0.01F))];

    deltaHue = -1;
    deltaHue2 = -1;
    dimAll(245U);
	step=0;
}
void FieryLedLampEffectLumenjer::updateInner()
{
	dimAll(DIMSPEED);

  	deltaHue = random8(3) ? deltaHue : -deltaHue;
  	deltaHue2 = random8(3) ? deltaHue2 : -deltaHue2;
#if (WIDTH % 2 == 0 && HEIGHT % 2 == 0)
  	hue = (WIDTH + hue + (int8_t)deltaHue * (bool)random8(64)) % WIDTH;
#else
  	hue = (WIDTH + hue + (int8_t)deltaHue) % WIDTH;
#endif
  	hue2 = (HEIGHT + hue2 + (int8_t)deltaHue2) % HEIGHT;

  	if (scale == 100U)
    	leds[XY(hue, hue2)] += CHSV(random8(), 255U, 255U);
  	else
    	leds[XY(hue, hue2)] += ColorFromPalette(*curPalette, step++);
}

// ------------------------------ ЭФФЕКТ ПРЫГУНЫ ----------------------
// взято откуда-то by @obliterator
// https://github.com/DmytroKorniienko/FireLamp_JeeUI/blob/templ/src/effects.cpp

//Leaper leapers[20];
//вместо класса Leaper будем повторно использовать переменные из эффекта мячики и мотыльки
//float x, y; будет:
//float trackingObjectPosX[enlargedOBJECT_MAX_COUNT];
//float trackingObjectPosY[enlargedOBJECT_MAX_COUNT];
//float xd, yd; будет:
////float trackingObjectSpeedX[enlargedOBJECT_MAX_COUNT];                   // As time goes on the impact velocity will change, so make an array to store those values
////float trackingObjectSpeedY[enlargedOBJECT_MAX_COUNT];                       // Coefficient of Restitution (bounce damping)
//CHSV color; будет:
////uint8_t trackingObjectHue[enlargedOBJECT_MAX_COUNT];

void FieryLedLampEffectLeapers::restart(uint8_t l)
{
  	// leap up and to the side with some random component
  	trackingObjectSpeedX[l] = (1 * (float)random8(1, 100) / 100);
  	trackingObjectSpeedY[l] = (2 * (float)random8(1, 100) / 100);

  	// for variety, sometimes go 50% faster
  	if (random8() < 12) {
    	trackingObjectSpeedX[l] += trackingObjectSpeedX[l] * 0.5;
    	trackingObjectSpeedY[l] += trackingObjectSpeedY[l] * 0.5;
  	}

  	// leap towards the centre of the screen
  	if (trackingObjectPosX[l] > (WIDTH / 2)) {
    	trackingObjectSpeedX[l] = -trackingObjectSpeedX[l];
  	}
}

void FieryLedLampEffectLeapers::move(uint8_t l)
{
#define GRAVITY            0.06
#define SETTLED_THRESHOLD  0.1
#define WALL_FRICTION      0.95
#define WIND               0.95    // wind resistance

	trackingObjectPosX[l] += trackingObjectSpeedX[l];
	trackingObjectPosY[l] += trackingObjectSpeedY[l];

  	// bounce off the floor and ceiling?
  	if (trackingObjectPosY[l] < 0 || trackingObjectPosY[l] > HEIGHT - 1) {
		trackingObjectSpeedY[l] = (-trackingObjectSpeedY[l] * WALL_FRICTION);
		trackingObjectSpeedX[l] = ( trackingObjectSpeedX[l] * WALL_FRICTION);
		trackingObjectPosY[l] += trackingObjectSpeedY[l];
		if (trackingObjectPosY[l] < 0)
			trackingObjectPosY[l] = 0; // settled on the floor?
		if (trackingObjectPosY[l] <= SETTLED_THRESHOLD && fabs(trackingObjectSpeedY[l]) <= SETTLED_THRESHOLD) {
			restart(l);
		}
  	}

  	// bounce off the sides of the screen?
  	if (trackingObjectPosX[l] <= 0 || trackingObjectPosX[l] >= WIDTH - 1) {
    	trackingObjectSpeedX[l] = (-trackingObjectSpeedX[l] * WALL_FRICTION);
    	if (trackingObjectPosX[l] <= 0) {
      		trackingObjectPosX[l] = -trackingObjectPosX[l];
    	} else {
      		trackingObjectPosX[l] = WIDTH + WIDTH - 2 - trackingObjectPosX[l];
    	}
  	}

  	trackingObjectSpeedY[l] -= GRAVITY;
  	trackingObjectSpeedX[l] *= WIND;
  	trackingObjectSpeedY[l] *= WIND;
}

void FieryLedLampEffectLeapers::setup()
{
    setCurrentPalette();
    enlargedObjectNUM = (scale - 1U) % 11U / 10.0 * (enlargedOBJECT_MAX_COUNT - 1U) + 1U;
    if (enlargedObjectNUM > enlargedOBJECT_MAX_COUNT) enlargedObjectNUM = enlargedOBJECT_MAX_COUNT;

    for (uint8_t i = 0 ; i < enlargedObjectNUM ; i++) {
    	trackingObjectPosX[i] = random8(WIDTH);
      	trackingObjectPosY[i] = random8(HEIGHT);

      	trackingObjectHue[i] = random8();
    }
}

void FieryLedLampEffectLeapers::updateInner()
{
	FastLED.clear();
	for (uint8_t i = 0; i < enlargedObjectNUM; i++) {
    	move(i);
		drawPixelXYF(trackingObjectPosX[i], trackingObjectPosY[i], ColorFromPalette(*curPalette, trackingObjectHue[i]));
  	};

  	blurScreen(20);
}

// ============= Эффект Магма ===============
// (c) SottNick
// берём эффекты Огонь 2020 и Прыгуны:
// хуяк-хуяк - и в продакшен!

void FieryLedLampEffectMagma::setup()
{
	deltaValue = scale * 0.0899;// /100.0F * ((sizeof(palette_arr) /sizeof(TProgmemRGBPalette16 *))-0.01F));
    if (deltaValue == 3U || deltaValue == 4U)
      	curPalette =  palette_arr[deltaValue]; // (uint8_t)(modes[currentMode].Scale/100.0F * ((sizeof(palette_arr) /sizeof(TProgmemRGBPalette16 *))-0.01F))];
    else
      	curPalette = firePalettes[deltaValue]; // (uint8_t)(modes[currentMode].Scale/100.0F * ((sizeof(firePalettes)/sizeof(TProgmemRGBPalette16 *))-0.01F))];
    deltaValue = 12U;
    deltaHue = 10U;// map(deltaValue, 8U, 168U, 8U, 84U); // высота языков пламени должна уменьшаться не так быстро, как ширина
    //step = map(255U-deltaValue, 87U, 247U, 4U, 32U); // вероятность смещения искорки по оси ИКС
    for (uint8_t j = 0; j < HEIGHT; j++) {
      	shiftHue[j] = (HEIGHT - 1 - j) * 255 / (HEIGHT - 1); // init colorfade table
    }

    enlargedObjectNUM = (scale - 1U) % 11U / 10.0 * (enlargedOBJECT_MAX_COUNT - 1U) + 1U;
    if (enlargedObjectNUM > enlargedOBJECT_MAX_COUNT)
		enlargedObjectNUM = enlargedOBJECT_MAX_COUNT;

    for (uint8_t i = 0 ; i < enlargedObjectNUM ; i++) {
      	trackingObjectPosX[i] = random8(WIDTH);
      	trackingObjectPosY[i] = random8(HEIGHT);

	    trackingObjectHue[i] = 50U; random8();
    }
	ff_y=0;
	ff_z=0;
}

void FieryLedLampEffectMagma::updateInner()
{
	dimAll(181);

  	for (uint8_t i = 0; i < WIDTH; i++) {
    	for (uint8_t j = 0; j < HEIGHT; j++) {
			drawPixelXYF(i, HEIGHT - 1U - j, ColorFromPalette(*curPalette, qsub8(inoise8(i * deltaValue, (j + ff_y + random8(2)) * deltaHue, ff_z), shiftHue[j]), 255U));
    	}
  	}

  	for (uint8_t i = 0; i < enlargedObjectNUM; i++) {
    	move(i);
        if (trackingObjectPosY[i] >= HEIGHT / 4U)
      		drawPixelXYF(trackingObjectPosX[i], trackingObjectPosY[i], ColorFromPalette(*curPalette, trackingObjectHue[i]));
  	};

	ff_y++;
  	if (ff_y & 0x01)
    	ff_z++;
}

// ============ Oil Paints ==============
//      © SlingMaster | by Alex Dovby
//              EFF_PAINT
//           Масляные Краски
//---------------------------------------
void FieryLedLampEffectOilPaints::setup()
{
    FastLED.clear();
    deltaValue = 255U - speed + 1U;
    step = deltaValue;                    // чтообы при старте эффекта сразу покрасить лампу
    hue = floor(21.25 * (random8(11) + 1)); // next color
    deltaHue = hue - 22;                  // last color
    deltaHue2 = 80;                       // min bright
    max_val = pow(2, WIDTH);
}

void FieryLedLampEffectOilPaints::updateInner()
{
	uint8_t divider;
	uint8_t entry_point;

	if (step >= deltaValue) {
    	step = 0U;
  	}

  	// Create Oil Paints --------------
  	// выбираем краски  ---------------
  	if (step % CENTER_Y_MINOR == 0) {
    	divider = floor((scale - 1) / 10);             // маштаб задает диапазон изменения цвета
		deltaHue = hue;                                                   // set last color
		hue += 6 * divider;                                               // new color
		hue2 = 255;                                                       // restore brightness
		deltaHue2 = 80 - floor(log(brightness) * 6);   // min bright
		entry_point = random8(WIDTH);                                     // start X position
		trackingObjectHue[entry_point] = hue;                             // set start position
		drawPixelXY(entry_point,  HEIGHT - 2, CHSV(hue, 255U, 255U));
		// !!! ********
		#if 0
		if (custom_eff == 1) {
			drawPixelXY(entry_point + 1,  HEIGHT - 3, CHSV(hue + 30, 255U, 255U));
		}
		#endif
  	}

  	// формируем форму краски, плавно расширяя струю ----
  	if (random8(3) == 1) {
    	for (uint8_t x = 1U; x < WIDTH; x++) {
      		if (trackingObjectHue[x] == hue) {
        		trackingObjectHue[x - 1] = hue;
        		break;
      		}
    	}
  	} else {
    	for (uint8_t x = WIDTH - 1; x > 0U ; x--) {
      		if (trackingObjectHue[x] == hue) {
        		trackingObjectHue[x + 1] = hue;
        		break;
      		}
    	}
  	}
  	// выводим сформированную строку --------------------- максимально яркую в момент смены цвета
  	for (uint8_t x = 0U; x < WIDTH; x++) {
    //                                                                                set color  next |    last  |
    	drawPixelXY(x,  HEIGHT - 1, CHSV(trackingObjectHue[x], 255U, (trackingObjectHue[x] == hue) ? hue2 : deltaHue2));
  	}
  	// уменьшаем яркость для следующих строк
  	if ( hue2 > (deltaHue2 + 16)) {
    	hue2 -= 16U;
  	}
  	// сдвигаем неравномерно поток вниз ---
  	uint16_t value = random16(max_val);
  	for (uint8_t x = 0U; x < WIDTH; x++) {
    	if ( bitRead(value, x ) == 0) {
      		for (uint8_t y = 0U; y < HEIGHT - 1; y++) {
        		drawPixelXY(x, y, getPixColorXY(x, y + 1U));
      		}
    	}
  	}
	step++;
}

// ------------- матрица ---------------
void FieryLedLampEffectMatrix::setup()
{

}
void FieryLedLampEffectMatrix::updateInner()
{
	for (uint8_t x = 0U; x < WIDTH; x++)
  	{
    	// обрабатываем нашу матрицу снизу вверх до второй сверху строчки
    	for (uint8_t y = 0U; y < HEIGHT - 1U; y++)
    	{
      		uint32_t thisColor = getPixColorXY(x, y);                                              // берём цвет нашего пикселя
      		uint32_t upperColor = getPixColorXY(x, y + 1U);                                        // берём цвет пикселя над нашим
      		if (upperColor >= 0x900000 && random(7 * HEIGHT) != 0U)                  // если выше нас максимальная яркость, игнорим этот факт с некой вероятностью или опускаем цепочку ниже
        		drawPixelXY(x, y, upperColor);
      		else if (thisColor == 0U && random((100 - scale) * HEIGHT) == 0U)  // если наш пиксель ещё не горит, иногда зажигаем новые цепочки
        	//else if (thisColor == 0U && random((100 - modes[currentMode].Scale) * HEIGHT*3) == 0U)  // для длинных хвостов
        		drawPixelXY(x, y, 0x9bf800);
      		else if (thisColor <= 0x050800)                                                        // если наш пиксель почти погас, стараемся сделать затухание медленней
      		{
        		if (thisColor >= 0x030000)
          			drawPixelXY(x, y, 0x020300);
        		else if (thisColor != 0U)
          			drawPixelXY(x, y, 0U);
      		}
      		else if (thisColor >= 0x900000)                                                        // если наш пиксель максимальной яркости, резко снижаем яркость
        		drawPixelXY(x, y, 0x558800);
      		else
        		drawPixelXY(x, y, thisColor - 0x0a1000);                                             // в остальных случаях снижаем яркость на 1 уровень
      			//drawPixelXY(x, y, thisColor - 0x050800);                                             // для длинных хвостов
    	}
    	// аналогично обрабатываем верхний ряд пикселей матрицы
    	uint32_t thisColor = getPixColorXY(x, HEIGHT - 1U);
    	if (thisColor == 0U)                                                                     // если наш верхний пиксель не горит, заполняем его с вероятностью .Scale
    	{
      		if (random(100 - scale) == 0U)
        		drawPixelXY(x, HEIGHT - 1U, 0x9bf800);
    	}
    	else if (thisColor <= 0x050800)                                                          // если наш верхний пиксель почти погас, стараемся сделать затухание медленней
    	{
      		if (thisColor >= 0x030000)
        		drawPixelXY(x, HEIGHT - 1U, 0x020300);
      		else
        		drawPixelXY(x, HEIGHT - 1U, 0U);
    	}
    	else if (thisColor >= 0x900000)                                                          // если наш верхний пиксель максимальной яркости, резко снижаем яркость
      		drawPixelXY(x, HEIGHT - 1U, 0x558800);
    	else
      		drawPixelXY(x, HEIGHT - 1U, thisColor - 0x0a1000);                                     // в остальных случаях снижаем яркость на 1 уровень
    		//drawPixelXY(x, HEIGHT - 1U, thisColor - 0x050800);                                     // для длинных хвостов
  	}
}

// ------------------------------ ЭФФЕКТ МЕРЦАНИЕ ----------------------
// (c) SottNick

#define TWINKLES_SPEEDS 4     // всего 4 варианта скоростей мерцания
#define TWINKLES_MULTIPLIER 6 // слишком медленно, если на самой медленной просто по единичке к яркости добавлять

void FieryLedLampEffectTwinkles::setup()
{
	setCurrentPalette();
    hue = 0U;
    deltaValue = (scale - 1U) % 11U + 1U;  // вероятность пикселя загореться от 1/1 до 1/11
    for (uint32_t idx = 0; idx < NUM_LEDS; idx++) {
      	if (random8(deltaValue) == 0) {
        	leds[idx].r = random8();                          // оттенок пикселя
        	leds[idx].g = random8(1, TWINKLES_SPEEDS * 2 + 1); // скорость и направление (нарастает 1-4 или угасает 5-8)
        	leds[idx].b = random8();                          // яркость
      	}
      	else
        	leds[idx] = 0;                                    // всё выкл
    }
}

void FieryLedLampEffectTwinkles::updateInner()
{
  	for (uint32_t idx = 0; idx < NUM_LEDS; idx++) {
    	if (leds[idx].b == 0) {
      		if (random8(deltaValue) == 0 && hue > 0) { // если пиксель ещё не горит, зажигаем каждый ХЗй
        		leds[idx].r = random8();                          // оттенок пикселя
        		leds[idx].g = random8(1, TWINKLES_SPEEDS + 1);    // скорость и направление (нарастает 1-4, но не угасает 5-8)
        		leds[idx].b = leds[idx].g;                    // яркость
        		hue--; // уменьшаем количество погасших пикселей
      		}
    	}
    	else if (leds[idx].g <= TWINKLES_SPEEDS) {            // если нарастание яркости
      		if (leds[idx].b > 255U - leds[idx].g - TWINKLES_MULTIPLIER) {           // если досигнут максимум
        		leds[idx].b = 255U;
        		leds[idx].g = leds[idx].g + TWINKLES_SPEEDS;
      		}
      		else
        		leds[idx].b = leds[idx].b + leds[idx].g + TWINKLES_MULTIPLIER;
    	}
    	else {                                                    // если угасание яркости
      		if (leds[idx].b <= leds[idx].g - TWINKLES_SPEEDS + TWINKLES_MULTIPLIER) { // если досигнут минимум
        		leds[idx].b = 0;                                  // всё выкл
        		hue++; // считаем количество погасших пикселей
      		}
      		else
        		leds[idx].b = leds[idx].b - leds[idx].g + TWINKLES_SPEEDS - TWINKLES_MULTIPLIER;
    	}
    	if (leds[idx].b == 0)
      		leds[idx] = 0U;
    	else
      		leds[idx] = ColorFromPalette(*curPalette, leds[idx].r, leds[idx].b);
  	}
}