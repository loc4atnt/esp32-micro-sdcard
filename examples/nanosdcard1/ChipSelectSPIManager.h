#ifndef _ChipSelectSPIManager_H_
#define _ChipSelectSPIManager_H_

#include "Arduino.h"

#define MAX_DEVICE 10

uint16_t calHash(const String &str);

struct CSPinWithHash {
  uint16_t hash;
  uint8_t cs;
};

class ChipSelectSPIManagerClass {
public:
  ChipSelectSPIManagerClass();

  bool registerCS(String devName, int csPin);

  bool selectDevice(String devName);

  void unselectAllDev();
private:
  CSPinWithHash hashMap[MAX_DEVICE];
  int registeredDevAmount = 0;

  int findInHash(const String &str, CSPinWithHash &csWithHash);
};

extern ChipSelectSPIManagerClass ChipSelectSPIManager;

#endif