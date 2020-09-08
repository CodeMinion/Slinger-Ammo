
/****
 * Pattern file Generated from a Gimp Image file using the Gimp LEDs plug-in.
 * Gimp LEDs Plug-in Download: https://bit.ly/GimpLeds
 * Gimp Download: https://www.gimp.org
 ****/ 
 
#ifndef AMMOPATTERN_BRIGHTMOSS_H
#define AMMOPATTERN_BRIGHTMOSS_H
#include <avr/pgmspace.h>
#include <Adafruit_NeoPixel.h>
#include "GimpLedPattern.h"

#define AMMOPATTERN_BRIGHTMOSS_DELAY 300

#define AMMOPATTERN_BRIGHTMOSS_TOTAL_LEDS 49

namespace NS_AMMOPATTERN_BRIGHTMOSS {

	const uint32_t TLF_BIRGHTMOSS_FRAME_1[] PROGMEM = { 
	0xff00b5, 0xff00b5, 0xff00b5, 0xff00b5, 0xff00b5, 0xff00b5, 0xff00b5, 0xff00b5, 0xff00b5, 0xff00b5, 
	0xff00b5, 0xff00b5
	};

	const uint32_t TLF_BIRGHTMOSS_FRAME_2_COPY[] PROGMEM = { 
	0xbf0087, 0xbf0087, 0xbf0087, 0xbf0087, 0xbf0087, 0xbf0087, 0xbf0087, 0xbf0087, 0xbf0087, 0xbf0087, 
	0xbf0087, 0xbf0087
	};

	const uint32_t TLF_BIRGHTMOSS_FRAME_3_COPY[] PROGMEM = { 
	0x7f005a, 0x7f005a, 0x7f005a, 0x7f005a, 0x7f005a, 0x7f005a, 0x7f005a, 0x7f005a, 0x7f005a, 0x7f005a, 
	0x7f005a, 0x7f005a
	};

	const uint32_t TLF_BIRGHTMOSS_FRAME_2[] PROGMEM = { 
	0x7f005a, 0x7f005a, 0x7f005a, 0x7f005a, 0x7f005a, 0x7f005a, 0x7f005a, 0x7f005a, 0x7f005a, 0x7f005a, 
	0x7f005a, 0x7f005a
	};

	const uint32_t TLF_BIRGHTMOSS_FRAME_1_COPY[] PROGMEM = { 
	0xbf0087, 0xbf0087, 0xbf0087, 0xbf0087, 0xbf0087, 0xbf0087, 0xbf0087, 0xbf0087, 0xbf0087, 0xbf0087, 
	0xbf0087, 0xbf0087
	};

	const uint32_t *const AMMOPATTERN_BRIGHTMOSS[] PROGMEM = { 
	TLF_BIRGHTMOSS_FRAME_1,
	TLF_BIRGHTMOSS_FRAME_2_COPY,
	TLF_BIRGHTMOSS_FRAME_3_COPY,
	TLF_BIRGHTMOSS_FRAME_2,
	TLF_BIRGHTMOSS_FRAME_1_COPY,
	};

	const uint32_t AMMOPATTERN_BRIGHTMOSS_SIZES[] PROGMEM = { 
	12,
	12,
	12,
	12,
	12,
	};

}

using namespace NS_AMMOPATTERN_BRIGHTMOSS;

		
class Pattern_AMMOPATTERN_BRIGHTMOSS : public GimpLedPattern 
{

  public:
    Pattern_AMMOPATTERN_BRIGHTMOSS(Adafruit_NeoPixel& strip): GimpLedPattern(strip){}

    ~Pattern_AMMOPATTERN_BRIGHTMOSS(){}

    void playPattern() 
    {
      int totalFrames = sizeof(AMMOPATTERN_BRIGHTMOSS) / sizeof(uint32_t*);
      for (int framePos = 0; framePos < totalFrames; framePos ++)
      {
        int frameTotalLeds = pgm_read_dword(&(AMMOPATTERN_BRIGHTMOSS_SIZES[framePos]));
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
		  uint32_t * frameAddress = (uint32_t *)pgm_read_dword(&(AMMOPATTERN_BRIGHTMOSS[framePos]));
		  uint32_t ledColor = pgm_read_dword(&(frameAddress[ledPos]));
		  
          int blue = ledColor & 0x00FF;
          int green = (ledColor >> 8) & 0x00FF;
          int red = (ledColor >>  16) & 0x00FF;
          mStrip.setPixelColor(ledPos + ledOffset, red, green, blue);

        }
        mStrip.show();
        delay(AMMOPATTERN_BRIGHTMOSS_DELAY);
      }
    }

    
    void stopPattern() 
    {
      mInterrupt = true;
    }
};
		
#endif //AMMOPATTERN_BRIGHTMOSS_H

