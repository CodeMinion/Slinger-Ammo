#ifndef BLE_PROP_HELPER_H
#define BLE_PROP_HELPER_H
#include <bluefruit.h>
#include "BlePropService.h"


#define VBAT_PIN          (A7)
#define VBAT_MV_PER_LSB   (0.73242188F)   // 3.0V ADC range and 12-bit ADC resolution = 3000mV/4096
#define VBAT_DIVIDER      (0.71275837F)   // 2M + 0.806M voltage divider on VBAT = (2M / (0.806M + 2M))
#define VBAT_DIVIDER_COMP (1.403F)        // Compensation factor for the VBAT divider


typedef void (*ble_connect_callback_t    ) (uint16_t conn_hdl);
typedef void (*ble_disconnect_callback_t ) (uint16_t conn_hdl, uint8_t reason);

class BlePropHelper
{
  public:
    BlePropHelper(const char*  deviceNameBase, const char * deviceModel, const char * manufacturer,
                  ble_connect_callback_t connectCallback, ble_disconnect_callback_t disconnect_callback)
      : mDeviceNameBase(deviceNameBase), mDeviceModel(deviceModel), mManufacturer(manufacturer)
    {
      mConnect_cb = connectCallback;
      mDisconnect_cb = disconnect_callback;
    }

    ~BlePropHelper() {}

    void setup(BlePropService propServices [], int propServiceCount)
    {
      Bluefruit.begin();

      // We'll control the LED so we can save some power.
      Bluefruit.autoConnLed(false);

     
      // Configure the Device Name
      uint8_t address [6];
      Bluefruit.Gap.getAddr(address);
      char nameBuff[50] = "";
      sprintf(nameBuff, DEVICE_NAME_FORMAT, mDeviceNameBase, address[1], address[0]);
      Bluefruit.setName(nameBuff);

      // Set the connect/disconnect callback handlers
      Bluefruit.setConnectCallback(mConnect_cb);
      Bluefruit.setDisconnectCallback(mDisconnect_cb);

      // Configure Device Information
      bledis.setManufacturer(mManufacturer);
      bledis.setModel(mDeviceModel);
      bledis.begin();

      // Configure battery service and start with 100%
      blebas.begin();
      blebas.write(100);

      for(int i = 0; i < propServiceCount; i++)
      {
        propServices[i].setup();
      }
      
      // Start Advertising
      startAdv(propServices, propServiceCount);
    }

    void notifyBatteryLevel(int level)
    {
      blebas.notify(level);
    }

    int readBatteryLevel()
    {
      int vbat_raw = readVBAT();

      // Convert from raw mv to percentage (based on LIPO chemistry)
      uint8_t vbat_per = mvToPercent(vbat_raw * VBAT_MV_PER_LSB);
      return vbat_per;
    }


  protected:

    BLEDis bledis;    // DIS (Device Information Service) helper class instance
    BLEBas blebas;    // BAS (Battery Service) helper class instance

    const char * DEVICE_NAME_FORMAT = "%s %X%X";
    const char * mDeviceNameBase;
    const char * mDeviceModel;
    const char * mManufacturer;
    
    ble_connect_callback_t mConnect_cb = NULL;
    ble_disconnect_callback_t mDisconnect_cb = NULL;

    void startAdv(BlePropService propServices[], int propServiceCount )
    {
      // Advertising packet
      Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
      Bluefruit.Advertising.addTxPower();

      // Add all services that were added.
      for(int i = 0; i < propServiceCount; i++)
      {
        Bluefruit.Advertising.addService(propServices[i].getPropService());
      }
      // Include Name
      Bluefruit.Advertising.addName();

      /* Start Advertising
        - Enable auto advertising if disconnected
        - Interval:  fast mode = 20 ms, slow mode = 152.5 ms
        - Timeout for fast mode is 30 seconds
        - Start(timeout) with timeout = 0 will advertise forever (until connected)

        For recommended advertising interval
        https://developer.apple.com/library/content/qa/qa1931/_index.html
      */
      Bluefruit.Advertising.restartOnDisconnect(true);
      Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
      Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
      Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds
    }

    int readVBAT(void)
    {
      int raw;

      // Set the analog reference to 3.0V (default = 3.6V)
      analogReference(AR_INTERNAL_3_0);

      // Set the resolution to 12-bit (0..4095)
      analogReadResolution(12); // Can be 8, 10, 12 or 14

      // Let the ADC settle
      delay(1);

      // Get the raw 12-bit, 0..3000mV ADC value
      raw = analogRead(VBAT_PIN);

      // Set the ADC back to the default settings
      analogReference(AR_DEFAULT);
      analogReadResolution(10);

      return raw;
    }

    uint8_t mvToPercent(float mvolts) {
      uint8_t battery_level;

      if (mvolts >= 3000)
      {
        battery_level = 100;
      }
      else if (mvolts > 2900)
      {
        battery_level = 100 - ((3000 - mvolts) * 58) / 100;
      }
      else if (mvolts > 2740)
      {
        battery_level = 42 - ((2900 - mvolts) * 24) / 160;
      }
      else if (mvolts > 2440)
      {
        battery_level = 18 - ((2740 - mvolts) * 12) / 300;
      }
      else if (mvolts > 2100)
      {
        battery_level = 6 - ((2440 - mvolts) * 6) / 340;
      }
      else
      {
        battery_level = 0;
      }

      return battery_level;
    }

};

#endif
