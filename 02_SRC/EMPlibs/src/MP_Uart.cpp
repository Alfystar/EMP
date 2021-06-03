//
// Created by alfy on 13/05/21.
//

#include "MP_Uart.h"
#ifdef CMAKE_COMPILING
/*
 * Common names are:
 * /dev/ttyACM0 - ACM stands for the ACM modem on the USB bus. Arduino UNOs (and similar) will appear using this name.
 * /dev/ttyPS0  - Xilinx Zynq FPGAs running a Yocto-based Linux build will use this name for the
 *                default serial port that Getty connects to.
 * /dev/ttyS0   - Standard COM ports will have this name. These are less common these days with newer desktops
 *                and laptops not having actual COM ports.
 * /dev/ttyUSB0 - Most USB-to-serial cables will show up using a file named like this.
 * /dev/pts/0   - A pseudo terminal. These can be generated with socat.
 */
vector<string> EMP::UartDeviceList() {
  FILE *fp;

  // Open the command for reading.
  // fp = popen("realpath /dev/ttyUSB* /dev/ttyACM* -q -e   2>/dev/null", "r");
  fp = popen("realpath /dev/serial/by-id/* -q -e   2>/dev/null", "r"); // shoul be more general

  if (fp == NULL) {
    printf("Failed to run command\n");
    exit(-1);
  }
  // Read the output one line at time - output it.
  vector<string> devList;
  std::string s;
  char line[4096];
  while (fgets(line, sizeof(line), fp) != NULL) {
    // remove new line from return
    for (char *i = line; *i != 0; ++i) {
      if (*i == '\n') {
        *i = 0;
        break;
      }
    }
    s = line;
    devList.emplace_back(s);
  }
  /* close */
  pclose(fp);

  return devList;
}

#endif // CMAKE_COMPILING
