#include <Adafruit_NeoPixel.h>
#include "BlePropHelper.h"
#include "BlePropService.h"

// 1 - Include at the top of Arduino sketch under your other #include statements.
#include "Pattern_AMMOPATTERN_WATERMOSS.h"
#include "Pattern_AMMOPATTERN_BRIGHTMOSS.h"

#define LED_PIN    7
#define LED_COUNT 49


Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
// 2 - Paste on top of setup() and under Adafruit NeoPixel declaration.
// Note: This assumes you named your pixel strip 'strip' as in the Adafruit sample
// from: https://learn.adafruit.com/adafruit-neopixel-uberguide?view=all#arduino-library-installation
// If you named it differently used that name here instead of 'strip'
GimpLedPattern * pattern_ammopattern_watermoss = new Pattern_AMMOPATTERN_WATERMOSS(strip);
GimpLedPattern * pattern_ammopattern_brightmoss = new Pattern_AMMOPATTERN_BRIGHTMOSS(strip);
GimpLedPattern * activePattern = pattern_ammopattern_brightmoss;

#define STATUS_LED (19)

const int LOW_BATTERY_THRESHOLD = 40;
int lastBatteryReading = 100; 

const char* DEVICENAME = "Slinger";
const char* DEVICE_MODEL = "Slinger Ammo";
const char* DEVICE_MANUFACTURER = "Rounin Labs";

const int UUID16_SVC_SLINGER_AMMO = 0x52FF;
const int UUID16_CHR_SLINGER_AMMO_PATTERN = 0x5A38;
char * SLINGER_AMMO_PATTERN_DESCRIPTION = "Ammo LED Pattern [0-2]";

void connect_callback(uint16_t conn_handle);
void disconnect_callback(uint16_t conn_handle, uint8_t reason);
void characteristic_write_callback(BLECharacteristic& chr, uint8_t* data, uint16_t len, uint16_t offset);

BlePropService ammoPatternService = BlePropService(UUID16_SVC_SLINGER_AMMO, UUID16_CHR_SLINGER_AMMO_PATTERN, SLINGER_AMMO_PATTERN_DESCRIPTION,  characteristic_write_callback);

BlePropHelper propHelper = BlePropHelper(DEVICENAME, DEVICE_MODEL, DEVICE_MANUFACTURER, connect_callback, disconnect_callback); 

BlePropService propServices[] = {ammoPatternService};
  
// Power Reduction: https://github.com/adafruit/Adafruit_nRF52_Arduino/issues/165
// Serial seems to increase consumption by 500uA https://github.com/adafruit/Adafruit_nRF52_Arduino/issues/51#issuecomment-368289198
#define DEBUG

#ifdef DEBUG
#define DEBUG_PRINT(x)  Serial.print(x)
#define DEBUG_PRINTLN(x)  Serial.println(x)
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#endif

  
void setup() {

#ifdef DEBUG
  Serial.begin(115200);
  while ( !Serial ) delay(10);   // for nrf52840 with native usb
#endif
  // put your setup code here, to run once:
  // Setup Neopixels
  // Reduce brigthness 0-255
  int propServiceCount = sizeof(propServices)/sizeof(BlePropService);
  propHelper.setup(propServices, propServiceCount);
  
  strip.setBrightness(40);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}


void connect_callback(uint16_t conn_handle)
{
  char central_name[32] = { 0 };
  Bluefruit.Gap.getPeerName(conn_handle, central_name, sizeof(central_name));

  DEBUG_PRINT("Connected to ");
  DEBUG_PRINTLN(central_name);
  // Disable the BT connection LED to save battery.
  digitalWrite(STATUS_LED, LOW);
  
}

void disconnect_callback(uint16_t conn_handle, uint8_t reason)
{
  (void) conn_handle;
  (void) reason;

  DEBUG_PRINTLN("Disconnected");
}

void characteristic_write_callback(BLECharacteristic& chr, uint8_t* data, uint16_t len, uint16_t offset)
{
  // TODO Check if characteristice is pattern char and run the proper pattern.

    DEBUG_PRINTLN("Write Received!");

  if (chr.uuid == ammoPatternService.getPropCharacteristic().uuid)
  {
      // Handle Ammo Pattern.
      uint8_t pattern = data[0];
      
      if( pattern == 0)
      {
        // Turn off LEDs
        turnOffPattern();
      } 
      else if (pattern == 1)
      {
        activatePattern(pattern_ammopattern_watermoss);
      }
      else if (pattern == 2)
      {
        activatePattern(pattern_ammopattern_brightmoss);
      }
  }
}

void turnOffPattern()
{
  if(activePattern != NULL)
  {
    activePattern->stopPattern();
  }

  strip.clear();
  strip.show();

  activePattern = NULL;
  
}

void activatePattern(GimpLedPattern * pattern)
{
    if(activePattern != NULL)
    {
      activePattern->stopPattern();
    }

    activePattern = pattern;
}

void loop() 
{
  // put your main code here, to run repeatedly:

  // 3 - Paste inside loop() to run the pattern.
  if(activePattern != NULL)
  {
    activePattern->playPattern();
  }
  // 4 - Optional: Use this to stop the pattern while it is in the middle of running.
  //pattern_ammopattern_watermoss->stopPattern();

  int batt = propHelper.readBatteryLevel();

  // Notify battery level once we are low so we don't constantly notify the app.
  if(batt < LOW_BATTERY_THRESHOLD && batt != lastBatteryReading)
  {
    propHelper.notifyBatteryLevel(batt);
    lastBatteryReading = batt; 
  }
}
