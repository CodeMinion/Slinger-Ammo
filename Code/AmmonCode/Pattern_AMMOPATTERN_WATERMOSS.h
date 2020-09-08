
/****
 * Pattern file Generated from a Gimp Image file using the Gimp LEDs plug-in.
 * Gimp LEDs Plug-in Download: https://bit.ly/GimpLeds
 * Gimp Download: https://www.gimp.org
 ****/ 
 
#ifndef AMMOPATTERN_WATERMOSS_H
#define AMMOPATTERN_WATERMOSS_H
#include <avr/pgmspace.h>
#include <Adafruit_NeoPixel.h>
#include "GimpLedPattern.h"

#define AMMOPATTERN_WATERMOSS_DELAY 300

#define AMMOPATTERN_WATERMOSS_TOTAL_LEDS 49

namespace NS_AMMOPATTERN_WATERMOSS {

	const uint32_t TLF_BIRGHTMOSS_FRAME_1_COPY[] PROGMEM = { 
	0x00ff21, 0x00ff21, 0x00ff21, 0x00ff21, 0x00ff21, 0x00ff21, 0x00ff21, 0x00ff21, 0x00ff21, 0x00ff21, 
	0x00ff21, 0x00ff21
	};

	const uint32_t TLF_BIRGHTMOSS_FRAME_1[] PROGMEM = { 
	0x00bf18, 0x00bf18, 0x00bf18, 0x00bf18, 0x00bf18, 0x00bf18, 0x00bf18, 0x00bf18, 0x00bf18, 0x00bf18, 
	0x00bf18, 0x00bf18
	};

	const uint32_t TLF_BIRGHTMOSS_FRAME_1_COPY_1[] PROGMEM = { 
	0x007f10, 0x007f10, 0x007f10, 0x007f10, 0x007f10, 0x007f10, 0x007f10, 0x007f10, 0x007f10, 0x007f10, 
	0x007f10, 0x007f10
	};

	const uint32_t TLF_BIRGHTMOSS_FRAME_1_COPY_2[] PROGMEM = { 
	0x003f08, 0x003f08, 0x003f08, 0x003f08, 0x003f08, 0x003f08, 0x003f08, 0x003f08, 0x003f08, 0x003f08, 
	0x003f08, 0x003f08
	};

	const uint32_t TLF_BIRGHTMOSS_FRAME_1_COPY_3[] PROGMEM = { 
	0x007f10, 0x007f10, 0x007f10, 0x007f10, 0x007f10, 0x007f10, 0x007f10, 0x007f10, 0x007f10, 0x007f10, 
	0x007f10, 0x007f10
	};

	const uint32_t TLF_BIRGHTMOSS_FRAME_1_COPY_4[] PROGMEM = { 
	0x00bf18, 0x00bf18, 0x00bf18, 0x00bf18, 0x00bf18, 0x00bf18, 0x00bf18, 0x00bf18, 0x00bf18, 0x00bf18, 
	0x00bf18, 0x00bf18
	};

	const uint32_t *const AMMOPATTERN_WATERMOSS[] PROGMEM = { 
	TLF_BIRGHTMOSS_FRAME_1_COPY,
	TLF_BIRGHTMOSS_FRAME_1,
	TLF_BIRGHTMOSS_FRAME_1_COPY_1,
	TLF_BIRGHTMOSS_FRAME_1_COPY_2,
	TLF_BIRGHTMOSS_FRAME_1_COPY_3,
	TLF_BIRGHTMOSS_FRAME_1_COPY_4,
	};

	const uint32_t AMMOPATTERN_WATERMOSS_SIZES[] PROGMEM = { 
	12,
	12,
	12,
	12,
	12,
	12,
	};

}

using namespace NS_AMMOPATTERN_WATERMOSS;

		
class Pattern_AMMOPATTERN_WATERMOSS : public GimpLedPattern 
{

  public:
    Pattern_AMMOPATTERN_WATERMOSS(Adafruit_NeoPixel& strip): GimpLedPattern(strip){}

    ~Pattern_AMMOPATTERN_WATERMOSS(){}

    void playPattern() 
    {
      int totalFrames = sizeof(AMMOPATTERN_WATERMOSS) / sizeof(uint32_t*);
      for (int framePos = 0; framePos < totalFrames; framePos ++)
      {
        int frameTotalLeds = pgm_read_dword(&(AMMOPATTERN_WATERMOSS_SIZES[framePos]));
		int ledOffset = 0;
        for (int ledPos = 0; ledPos < frameTotalLeds; ledPos++)
        {
          if(mInterrupt)
          {
            // If we are interrupted stop the pattern. "Clean" LED pattern.
            mStrip.clear();
            mStrip.show();
            mInterrupt = false;
            return;
          }
		  uint32_t * frameAddress = (uint32_t *)pgm_read_dword(&(AMMOPATTERN_WATERMOSS[framePos]));
		  uint32_t ledColor = pgm_read_dword(&(frameAddress[ledPos]));
		  
          //uint32_t ledColor = pgm_read_dword(&(AMMOPATTERN_WATERMOSS[framePos][ledPos]));
          int blue = ledColor & 0x00FF;
          int green = (ledColor >> 8) & 0x00FF;
          int red = (ledColor >>  16) & 0x00FF;
          mStrip.setPixelColor(ledPos + ledOffset, red, green, blue);

        }
        mStrip.show();
        delay(AMMOPATTERN_WATERMOSS_DELAY);
      }
    }

    
    void stopPattern() 
    {
      mInterrupt = true;
    }
};
		
#endif //AMMOPATTERN_WATERMOSS_H

