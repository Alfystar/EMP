// Have to be the FIRST
#include <Arduino.h>

// Data Pack demo
#include "pack.h"
#include <MP_Serial.h>

using namespace EMP;

EMP::MP_Serial<packLinux2Ard, packArd2Linux, ArduinoMP_templateDefault()> mpSerial(Serial);

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
