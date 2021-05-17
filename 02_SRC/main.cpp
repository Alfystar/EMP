//
// Created by alfy on 04/05/21.
//
#include <MP_Uart.h>
#include <unistd.h>
#include <iostream>

typedef struct pack_ {
  char buf[20];
} __attribute__((packed)) pack;

int main(int argc, char *argv[]) {
  std::cout << __cplusplus << std::endl;
  std::cout << "Uart Start" << std::endl;
  auto list = EMP::UartDeviceList();
  if(list.empty()){
    cout << "No device Detected" << endl;
    exit(-1);
  }
  cout << "Possible device detected:" << endl;
  for (int i = 0; i < list.size(); i++) {
    cout << "[" << i << "] = " << list[i] << endl;
  }
  int chose;
  cout << "Please chose the index: ";
  cin >> chose;
  LinuxMP_ConfMed(uartConf, true);
  auto *uart = new EMP::MP_Uart<pack, pack, uartConf>(list[chose],9600);


  return 0;
}