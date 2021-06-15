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
  if (list.size() == 1) {
    chose = 0;
    cout << "Connected with: " << list[0] << endl;

  } else {
    cout << "Possible device detected:" << endl;
    for (int i = 0; i < list.size(); i++) {
      cout << "[" << i << "] = " << list[i] << "\t{" << EMP::UartDeviceInfo(list[i]) << "}" << endl;
    }

    cout << "Please chose the index: ";
    cin >> chose;
  }

  auto *uart = new EMP::MP_Uart<packArd2Linux, packLinux2Ard, LinuxMP_ConfMed(true)>(list[chose], B115200,false);
  sleep(1); // Device can be reset after the connection
  packArd2Linux pRead;
  packLinux2Ard pWrite{0, "Hoy Arduino"};
  struct timespec now, old, diff;

  clock_gettime(CLOCK_MONOTONIC_RAW, &old);
  while (true) {
    std::cout << "Sending response pack..." << endl;
    pWrite.num = (pWrite.num + 1) % 100;
    uart->packSend(&pWrite);
    std::cout << "Waiting pack..." << endl;
    uart->getData_wait(&pRead);
    clock_gettime(CLOCK_MONOTONIC_RAW, &now);
    timeSpecSub(now, old, diff);
    old = now;

    std::cout << "Pack send: {Num = " << pWrite.num << "\tbuf = " << pWrite.buf << "}|\t|";
    std::cout << "Pack recive: {Num = " << pRead.num << "\tbuf = " << pRead.buf << "}|\t|";
    std::cout << "Expected result: [" << (pWrite.num + 1 == pRead.num) << "]|\t|";
    timeSpecPrint(diff, "diff");
  }
  delete uart;
  return 0;
}
