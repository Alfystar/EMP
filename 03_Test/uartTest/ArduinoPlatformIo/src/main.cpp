#include "pack.h"
#include <Arduino.h>
#include <MP_Serial.h>

using namespace EMP;

EMP::MP_Serial<packLinux2Ard, packArd2Linux, ArduinoMP_template(true)> mpSerial(Serial);

void setup() {
  // write your initialization code here
  pinMode(13,OUTPUT);
  mpSerial.begin(9600);
}

void loop() {
  digitalWrite(13, 1);
  packArd2Linux pWrite {5, "Hi linux"};
  mpSerial.packSend(&pWrite);
  mpSerial.updateState();
  delay(1000);
  mpSerial.updateState();
//  Serial.print("Num = ");
//  Serial.println(pRead.num);
//  Serial.print("buf = ");
//  Serial.println(pRead.buf);

  digitalWrite(13, 0);
  packLinux2Ard pRead;
  mpSerial.getData_wait(&pRead);

  delay(1000);


}