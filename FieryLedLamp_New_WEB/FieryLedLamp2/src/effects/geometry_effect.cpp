#include"effect.h"
#include"effects/geometry_effect.h"

// =====================================
//          Блуждающий кубик
// =====================================
void FieryLedLampEffectBall::setup()
{
	for (uint8_t i = 0; i < 2; i++)
	{
    	coordB[i] = LED_WIDTH / 2 * 10;
      	vectorB[i] = random(8, 20);
    }
    // ballSize;
    deltaValue = map(scale, 0, 255, 2, max((uint8_t)min(LED_WIDTH, LED_HEIGHT) / 3, 4));
    ballColor = CHSV(random(0, 9) * 28, 255, 255);
    _pulse_color = CHSV(random(0, 9) * 28, 255, 255);
}

void FieryLedLampEffectBall::updateInner()
{
	if (scale & 0x01)
	{
		for (uint8_t i = 0; i < deltaValue; i++)
		{
      		for (uint8_t j = 0; j < deltaValue; j++)
			{
				unsigned char led_index = XY(coordB[0] / 10 + i, coordB[1] / 10 + j);
        		leds[led_index] = _pulse_color;
      		}
    	}
	}

	for (uint8_t i = 0; i < 2; i++)
	{
		coordB[i] += vectorB[i];
    	if (coordB[i] < 0)
		{
			coordB[i] = 0;
      		vectorB[i] = -vectorB[i];
      		ballColor = CHSV(random(0, 9) * 28, 255, 255);
		}
	}
	int16_t maxCoord = (LED_WIDTH - deltaValue) * 10;
	if (coordB[0] > maxCoord)
	{
		coordB[0] = maxCoord;
    	vectorB[0] = -vectorB[0];
    	ballColor = CHSV(random(0, 9) * 28, 255, 255);
	}
	maxCoord = (LED_HEIGHT - deltaValue) * 10;
	if (coordB[1] > maxCoord)
	{
		coordB[1] = maxCoord;
    	vectorB[1] = -vectorB[1];
    	ballColor = CHSV(random(0, 9) * 28, 255, 255);
	}
	FastLED.clear();

    for (uint8_t i = 0; i < deltaValue; i++)
    {
        for (uint8_t j = 0; j < deltaValue; j++)
        {
            leds[XY(coordB[0] / 10 + i, coordB[1] / 10 + j)] = ballColor;
        }
    }
}