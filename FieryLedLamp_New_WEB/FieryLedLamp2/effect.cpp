#include"effect.h"

void FieryLedLampEffect::update()
{
	updateInner();

#ifdef MP3_TX_PIN
	mp3_folder = effects_folders[currentMode];
#endif  //MP3_TX_PIN
#ifdef USE_MULTIPLE_LAMPS_CONTROL
	if (repeat_multiple_lamp_control) {
		for (uint8_t n = 0; n< MODE_AMOUNT; n++)
		{
			if (eff_num_correct[n] == currentMode) {
				jsonWrite(configSetup, "eff_sel", n);
				break;
			}
		}
#ifdef MP3_TX_PIN
		if (mp3_player_connect == 4) {
			mp3_loop();
			jsonWrite(configSetup, "fold_sel", CurrentFolder);
		}
#endif  //MP3_TX_PIN
		jsonWrite(configSetup, "br", modes[currentMode].Brightness);
		jsonWrite(configSetup, "sp", modes[currentMode].Speed);
		jsonWrite(configSetup, "sc", modes[currentMode].Scale);
		multiple_lamp_control();
		repeat_multiple_lamp_control = false;
	}
#endif  //USE_MULTIPLE_LAMPS_CONTROL
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

	//FastLED.show();
};

void FieryLedLampEffectWhiteColorStripeRoutine::updateInner()
{
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
				45U,                                                                              // определяем тон
				map(modes[currentMode].Speed, 0U, 255U, 0U, 170U),                                // определяем насыщенность
				i > (center - fullFill - 1)                                                       // определяем яркость
				? 255U                                                                            // для центральных горизонтальных полос
				: iPol * (i > center - fullFill - 2));  // для остальных горизонтальных полос яркость равна либо 255, либо 0 в зависимости от масштаба

			if (modes[currentMode].Scale <= 50U)
				for (uint8_t x = 0U; x < WIDTH; x++)
				{
					drawPixelXY(x, i, color);                         // при чётной высоте матрицы максимально яркими отрисуются 2 центральных горизонтальных полосы
					drawPixelXY(x, HEIGHT + offset - i - 2U, color);  // при нечётной - одна, но дважды
				}
			else
				for (uint8_t y = 0U; y < HEIGHT; y++)
				{
					drawPixelXY((i + modes[currentMode].Speed - 1U) % WIDTH, y, color);                    // при чётной ширине матрицы максимально яркими отрисуются 2 центральных вертикальных полосы
					drawPixelXY((WIDTH + offset - i + modes[currentMode].Speed - 3U) % WIDTH, y, color);   // при нечётной - одна, но дважды
				}
		}
	}
};