#include"effect.h"
#include"effects/color_effect.h"

#define BORDERLAND   2 // две дополнительные единицы бегунка Масштаб на границе вертикального и горизонтального варианта эффекта (с каждой стороны границы) будут для света всеми светодиодами в полную силу
void FieryLedLampEffectWhiteColorStripeRoutine::setup()
{
	FastLED.clear();

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
#if 0	
  	if (trackingObject[5] == 1) {  // direction >>>
    	obj[1]++;
    	if (obj[1] >= obj[2]) {
      		trackingObject[5] = 0;     // swap direction
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
      		trackingObject[5] = 1;     // swap direction
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
  	if( (obj[1] >= LED_WIDTH) || (obj[3] == obj[4]) ) {
    	// deltaHue value == 255 activate -------
    	// set new parameter for new smear ------
    	deltaHue = 255;
  	}
	drawPixelXY(obj[1], obj[3], colors[divider][hue]);
#else
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
#endif
}

void FieryLedLampEffectWaterColor::setup()
{
    FastLED.clear();
    //deltaValue = 255U - speed + 1;
	deltaValue = map(speed, 0, 255, 1, 255);
    step = deltaValue; // чтообы при старте эффекта сразу покрасить лампу
    hue = 0;
    deltaHue = 255; // last color
    trackingObject[1] = floor(LED_WIDTH * 0.25);
    trackingObject[3] = floor(LED_HEIGHT * 0.25);

	lastColor = true; // last color
	x=LED_WIDTH/4;
	y=LED_HEIGHT/4;
}
void FieryLedLampEffectWaterColor::updateInner()
{
	if (step >= deltaValue) {
    	step = 0U;
  	}

  	// ******************************
  	// set random parameter for smear
  	// ******************************
#if 0
  	if (deltaHue == 255) {
    	trackingObject[0] = 4 + random8(floor(LED_WIDTH * 0.25));                // width
    	trackingObject[1] = random8(LED_WIDTH - trackingObject[0]);           // x
    	int temp =  trackingObject[1] + trackingObject[0];
    	if (temp >= (LED_WIDTH - 1)) {
      		temp = LED_WIDTH - 1;
      		if (trackingObject[1] > 1) {
        		trackingObject[1]--;
      		} else {
        		trackingObject[1]++;
      		}
    	}
		trackingObject[2] = temp;                                            // x end
		trackingObject[3] = 3 + random8(LED_HEIGHT - 4);                         // y
		temp = trackingObject[3] - random8(3) - 3;
    	if (temp <= 0) {
      		temp = 0;
    	}
    	trackingObject[4] = temp;                                            // y end
    	trackingObject[5] = 1;
    	//divider = floor((modes[currentMode].Scale - 1) / 16.7);                 // маштаб задает смену палитры
    	hue = random8(8);
    	hue2 = 255;
    	deltaHue = 0;
  	}
#else
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
    	//divider = floor((modes[currentMode].Scale - 1) / 16.7);                 // маштаб задает смену палитры
    	hue = random8(8);
    	lastColor=false;
  	}
#endif
  	// ******************************
  	SmearPaint(trackingObject);

  	if (step % 2 == 0) {
    	blurScreen(beatsin8(1U, 1U, 6U));
  	}
	step++;
}