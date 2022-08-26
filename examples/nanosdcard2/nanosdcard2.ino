/*
  SD card read/write
 
 This example shows how to read and write data to and from an SD card file 	
 The circuit:
 * SD card attached to SPI bus as follows:
 ** UNO:  MOSI - pin 11, MISO - pin 12, CLK - pin 13, CS - pin 4 (CS pin can be changed)
  and pin #10 (SS) must be an output
 ** Mega:  MOSI - pin 51, MISO - pin 50, CLK - pin 52, CS - pin 4 (CS pin can be changed)
  and pin #52 (SS) must be an output
 ** Leonardo: Connect to hardware SPI via the ICSP header

 
 created   Nov 2010  by David A. Mellis
 modified 9 Apr 2012  by Tom Igoe
 
 This example code is in the public domain.
 	 
 */
 
#include <SPI.h>
#include <mySD.h>
#include "ChipSelectSPIManager.h"

uint16_t USER_CONFIG_ARDUINO_LOOP_STACK_SIZE = 10240;

mySD::File myFile;

// change this to match your SD shield or module;
//     Arduino Ethernet shield: pin 4
//     Adafruit SD shields and modules: pin 10
//     Sparkfun SD shield: pin 8
const int chipSelect = 25;

const char BUFFFFF[] = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";

void buzz(int ms){
  digitalWrite(13, 1);
  delay(ms);
  digitalWrite(13, 0);
  delay(ms);
}

void test(String fileName, unsigned long &tt1, unsigned long &tt2){
  // SD.remove((char*)fileName.c_str());


    // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = mySD::SD.open(fileName.c_str(), FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to ");
    Serial.print(fileName);
    Serial.print("...    ");
    unsigned long t1 = millis();
    myFile.print(BUFFFFF);
    unsigned long t2 = millis();
    // close the file:
    myFile.close();
    unsigned long t3 = millis();
    unsigned long writingTime = t2 - t1;
    unsigned long writingAndCloseTime = t3 - t1;
    Serial.print("wt: ");
    Serial.print(writingTime);
    Serial.print("       wct: ");
    Serial.print(writingAndCloseTime);
    tt1 += writingTime;
    tt2 += writingAndCloseTime;
    Serial.println("  done.");
  } else {
    // if the file didn't open, print an error:
    Serial.print("error opening ");
    Serial.println(fileName);
    buzz(100);
  }

  // re-open the file for reading:
  myFile = mySD::SD.open(fileName.c_str());
  if (myFile) {
    Serial.print(fileName);
    Serial.println(":");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.print("error opening ");
    Serial.println(fileName);
    buzz(100);
  }
}

void setup()
{
 // Open serial communications and wait for port to open:
  Serial.begin(115200);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  delay(3000);
  pinMode(13, OUTPUT);

    ChipSelectSPIManager.registerCS("SD", chipSelect);
  ChipSelectSPIManager.registerCS("RF", 32);

  Serial.print("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin 
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output 
  // or the SD library functions will not work. 
ChipSelectSPIManager.selectDevice("SD");
   
  if (!mySD::SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
}

void loop() {
  unsigned long tt1 = 0;
  unsigned long tt2 = 0;
  unsigned long t3 = 0;
  for (int kk = 0;kk<5;kk++){
  for (int y = 1; y <= 1; y++){
    mySD::SD.mkdir((char*)String(y).c_str());
    for (int i = 1;i <= 1;i++){
      String fN = String(y);
      fN += "/";
      fN += String(i);
      fN += ".txt";
      // Serial.print("Test read/write file ");
      // Serial.println(fN);
      test(fN, tt1, tt2);
      Serial.println("== delay 000ms ==");
      // delay(100);
      t3+=1;
    }
  }
  }
    Serial.print("tt write: ");
    Serial.println(tt1);
    Serial.print("tt write close: ");
    Serial.println(tt2);
    Serial.print("tt f: ");
    Serial.println(t3);
  while(true){

  };
}