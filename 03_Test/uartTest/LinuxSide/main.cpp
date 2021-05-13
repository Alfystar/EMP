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
  auto *uart =
      new EMP::MP_Uart<packArd2Linux, packLinux2Ard, uartConf>(list[chose],B9600);

  packArd2Linux pRead;
  uart->getData_wait(&pRead);
  std::cout << "Num = " << pRead.num << "buf = " << pRead.buf << std::endl;

  packLinux2Ard pWrite{5, "Hoy Arduino"};
  uart->packSend(&pWrite);
  return 0;
}