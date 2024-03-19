#ifndef DISPLAY_FILE
#define DISPLAY_FILE

#include"platform.h"

#include<Wire.h>
#include <Adafruit_SSD1306.h>
#include <U8g2_for_Adafruit_GFX.h>

#define SEC_TO_MS(second) second*1000
#define MIN_TO_MS(minuts) SEC_TO_MS(minutes*60)

#define DISPLAY_BRIGHTNESS_LOW_MS 60000
#define DISPLAY_POWER_OFF_MS MIN_TO_MS(5)

enum DisplayState
{
    INIT_STATE,
    ON_STATE,
    LOW_STATE,
    OFF_STATE
};
struct DisplayBrightness
{
    unsigned long low_time, off_time;
    unsigned char low_level, off_level;
};
class Display
{
public:
    Display(){   
    }
    void init(){
        Wire.begin(DISPLAY_SDA, DISPLAY_SCL);
	    display = new Adafruit_SSD1306(128,64,&Wire, -1);
	    display->begin(SSD1306_SWITCHCAPVCC, DISPLAY_ADDRESS);
	    u8g2_for_adafruit_gfx.begin(*display);
        
        u8g2_for_adafruit_gfx.setFontDirection(0);            // left to right (this is default)
		u8g2_for_adafruit_gfx.setForegroundColor(WHITE);      // apply Adafruit GFX color

        set_brightness_low(1, 128);
        set_brightness_off(5, 0);

        state_before=INIT_STATE;
        set_state(OFF_STATE);
    }
    void set_brightness_low(unsigned char minutes, unsigned char level){
        brightness.low_time=MIN_TO_MS(minutes);
        brightness.low_level=level;
    };
    void set_brightness_off(unsigned char minutes, unsigned char level){
        brightness.off_time=MIN_TO_MS(minutes);
        brightness.off_level=level;
    };

    int width(){return display->width();}

    void set_state(unsigned char state){
        state_current=state;
        switch(state_current)
        {
        case OFF_STATE:
            delay=brightness.off_time;
            level=brightness.off_level;

            u8g2_for_adafruit_gfx.setFont(u8g2_font_inr16_mr);
			u8g2_for_adafruit_gfx.setFontMode(1);                 // use u8g2 transparent mode (this is default)
            break;
        case LOW_STATE:
            delay=brightness.low_time;
            level=brightness.low_level;
            break;
        case ON_STATE:
            delay=0;
            level=255;

            u8g2_for_adafruit_gfx.setFont(u8g2_font_inr24_t_cyrillic);  // select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall
            u8g2_for_adafruit_gfx.setFontMode(0);                 // use u8g2 transparent mode (this is default)
            break;
        }
    }

    void draw(const char *text){
        pos_x=0;
        text_size=u8g2_for_adafruit_gfx.getUTF8Width(text);
        strcat(this->text,text);
    }

    void update(){
        time_t t=time(0);
        unsigned long delta=t-time_before;
        if(delta==0)
            return;
        time_before=t;
        if(state_before!=state_current)
        {
            if(delay>0)
            {
                delay-=delta;
            }
            else
            {
                display->ssd1306_command(SSD1306_SETCONTRAST);
                display->ssd1306_command(level);
                state_before=state_current;
            }
        }
        else
        {
            display->clearDisplay();
            u8g2_for_adafruit_gfx.drawUTF8(pos_x, 43, text);
		    display->display();

		    pos_x=pos_x-4;
		    if(pos_x < -1*text_size)
			    pos_x=display->width();
        }
    };
private:
    DisplayBrightness brightness;
    
    unsigned long delay;
    unsigned char level;
    
    char text[256];
    int16_t text_size;
    int16_t pos_x;

    unsigned char state_before, state_current;

    time_t time_before;

    Adafruit_SSD1306 *display;
	U8G2_FOR_ADAFRUIT_GFX u8g2_for_adafruit_gfx;
};
#endif 