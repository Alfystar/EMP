// before link/copy in the user-library folder the EMPlibs direcotry
// cd <path of the userLibrary arduino>
// ln -s ~/Documents/uniGitHub/EMP/02_SRC/EMPlibs/ # for my computer path
// Data Pack demo
#include <MP_Serial.h>
#include "pack.h"

using namespace EMP;
// packLinux2Ard, packArd2Linux define in pack.h
// ArduinoMP_templateDefault() define in MPcore/conf.h to compleate the template parameter
MP_Serial<packLinux2Ard, packArd2Linux, ArduinoMP_templateDefault()> mpSerial(Serial);

void setup() {
  // write your initialization code here
  pinMode(13, OUTPUT);
  mpSerial.begin(115200);
  delay(100);
}

packArd2Linux pWrite{100, "Hi linux"};
packLinux2Ard pRead;

void loop() {
  mpSerial.getData_wait(&pRead);
  pWrite.num = pRead.num + 1;
  mpSerial.packSend(&pWrite);
}
