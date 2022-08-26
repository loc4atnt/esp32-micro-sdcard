#include "ChipSelectSPIManager.h"

uint16_t calHash(const String &str) {
	uint16_t res = 0;
	for (uint16_t i = 0 ; i < str.length(); i++) {
		uint16_t a = str[i];
		a *= i + 1;
		res += a;
	}
	return res;
}

ChipSelectSPIManagerClass::ChipSelectSPIManagerClass() {
  this->registeredDevAmount = 0;
}

bool ChipSelectSPIManagerClass::registerCS(String devName, int csPin) {
  CSPinWithHash cSPinWithHash;
  int devIdx = findInHash(devName, cSPinWithHash);
  
  if (devIdx < 0) {
    if (this->registeredDevAmount >= MAX_DEVICE) return false;
    devIdx = (this->registeredDevAmount++);
  }

  pinMode(csPin, OUTPUT);
  digitalWrite(csPin, 1);
  cSPinWithHash.cs = csPin;
  hashMap[devIdx] = cSPinWithHash;
  return true;
}

bool ChipSelectSPIManagerClass::selectDevice(String devName) {
  CSPinWithHash cSPinWithHash;
  if (findInHash(devName, cSPinWithHash) < 0) return false;
  unselectAllDev();
  digitalWrite(cSPinWithHash.cs, 0);
}

void ChipSelectSPIManagerClass::unselectAllDev(){
  for (int i = 0; i < this->registeredDevAmount; i++){
    digitalWrite(hashMap[i].cs, 1);
  }
}

int ChipSelectSPIManagerClass::findInHash(const String &str, CSPinWithHash &csWithHash){
  uint16_t h = calHash(str);
  csWithHash.hash = h;
  csWithHash.cs = -1;
  for (int i = 0; i < this->registeredDevAmount; i++){
    if (hashMap[i].hash == h){
      csWithHash = hashMap[i];
      return i;
    }
  }
  return -1;
}

ChipSelectSPIManagerClass ChipSelectSPIManager = ChipSelectSPIManagerClass();