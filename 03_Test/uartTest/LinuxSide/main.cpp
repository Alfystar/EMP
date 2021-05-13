//
// Created by alfy on 04/05/21.
//
#include <MP_Uart.h>
#include <unistd.h>

typedef struct pack_ {
  char buf[20];
} __attribute__((packed)) pack;

int main(int argc, char *argv[]) {
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
  LinuxMP_ConfDefault(uartConf);
  auto *uart = new EMP::MP_Uart<pack, pack, uartConf>(list[chose]);

  return 0;
}