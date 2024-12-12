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