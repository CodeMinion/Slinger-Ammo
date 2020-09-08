#ifndef BLE_PROP_SERVICE_H
#define BLE_PROP_SERVICE_H
#include <bluefruit.h>
#include "BlePropHelper.h"

class BlePropService
{
  public:
    BlePropService(int propServiceUuid, int propCharacteristicUuid, char * userDescription, BLECharacteristic::write_cb_t characteristicWriteCallback)
    {
      mPropService = BLEService(propServiceUuid);
      mPropCharacteristic = BLECharacteristic(propCharacteristicUuid);
      mPropCharacteristicUserDescription = userDescription;
      mCharacteristicWriteCallback = characteristicWriteCallback;
    }

    ~BlePropService() {}

    BLEService & getPropService()
    {
      return mPropService;
    }

    BLECharacteristic & getPropCharacteristic()
    {
      return mPropCharacteristic;
    }

  private:
    friend class BlePropHelper;

    BLEService mPropService;
    BLECharacteristic mPropCharacteristic;
    BLECharacteristic::write_cb_t mCharacteristicWriteCallback = NULL;
    char * mPropCharacteristicUserDescription;

    void setup()
    {
      mPropService.begin();

      // Note: You must call .begin() on the BLEService before calling .begin() on
      // any characteristic(s) within that service definition. Calling .begin() on
      // a BLECharacteristic will cause it to be added to the last BLEService that
      // was 'begin()'ed!
      
      mPropCharacteristic.setProperties(CHR_PROPS_READ | CHR_PROPS_WRITE | CHR_PROPS_NOTIFY);
      // Read Permissions, Write Permission
      mPropCharacteristic.setPermission(SECMODE_OPEN, SECMODE_OPEN);
      mPropCharacteristic.setFixedLen(2);
      mPropCharacteristic.setWriteCallback(mCharacteristicWriteCallback);
      mPropCharacteristic.setUserDescriptor(mPropCharacteristicUserDescription);
      mPropCharacteristic.begin();

      uint8_t lsdata[1] = { 0 }; // Set the characteristic to use 8-bit values, with the sensor connected and detected
      mPropCharacteristic.notify(lsdata, 1);     // Use .notify instead of .write!

    }
};


#endif
