//
// Created by alfy on 04/05/21.
//
#include "../pack.h"
#include <MP_Uart.h>
#include <iostream>
#include <unistd.h>

int main(int argc, char *argv[]) {
  std::cout << "Uart Start" << std::endl;
  auto list = EMP::UartDeviceList();
  if (list.empty()) {
    cout << "No device Detected" << endl;
    exit(-1);
  }
  int chose;
  if (list.size() == 1)
    chose = 0;
  else {
    cout << "Possible device detected:" << endl;
    for (int i = 0; i < list.size(); i++) {
      cout << "[" << i << "] = " << list[i] << endl;
    }

    cout << "Please chose the index: ";
    cin >> chose;
  }
  LinuxMP_ConfMed(uartConf, true);
  auto *uart = new EMP::MP_Uart<packArd2Linux, packLinux2Ard, uartConf>(list[chose], B115200);

  packArd2Linux pRead;
  packLinux2Ard pWrite {0, "Hoy Arduino"};
  struct timespec now, old, diff;

  clock_gettime(CLOCK_MONOTONIC_RAW, &old);
  while (true) {
    //std::cout << "Waiting pack..." << endl;
    uart->getData_wait(&pRead);

    clock_gettime(CLOCK_MONOTONIC_RAW, &now);
    timeSpecSub(now,old,diff);
    old = now;
    std::cout << "Pack recive: {Num = " << pRead.num << "\tbuf = " << pRead.buf << "}|\t|";
    timeSpecPrint(diff,"diff");

    //std::cout << "Sending response pack..." << endl;
    pWrite.num++;
    uart->packSend(&pWrite);
  }
  delete uart;
  return 0;
}