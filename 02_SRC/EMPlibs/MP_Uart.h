//
// Created by alfy on 13/05/21.
//

#ifndef EMP_LIB_02_SRC_EMPLIBS_MP_UART_H
#define EMP_LIB_02_SRC_EMPLIBS_MP_UART_H
#ifdef CMAKE_COMPILING
#include <MP_Fd.h>
#include <string>
#include <vector>

// Include for the serial
#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>

// Level 0 := No Message
// Level 1 := Error msg
// Level 2 := Notice msg
// Level 3 := Info msg
// Level 4 := DB msg
#define MP_UART_VervoseLevel 5
#define MP_UART_err _codeActive(1, MP_UART_VervoseLevel)
#define MP_UART_notice _codeActive(2, MP_UART_VervoseLevel)
#define MP_UART_info _codeActive(3, MP_UART_VervoseLevel)
#define MP_UART_Db _codeActive(4, MP_UART_VervoseLevel)

#define mpUart_err(str, ...) printf_STD(MP_UART_err, str, ##__VA_ARGS__)
#define mpUart_notice(str, ...) printf_STD(MP_UART_notice, str, ##__VA_ARGS__)
#define mpUart_info(str, ...) printf_STD(MP_UART_info, str, ##__VA_ARGS__)
#define mpUart_db(str, ...) printf_STD(MP_UART_Db, str, ##__VA_ARGS__)
using namespace std;
namespace EMP {

vector<string> UartDeviceList();

template <typename pIn, typename pOut, MPConf conf> class MP_Uart : public MP_Fd<pIn, pOut, conf> {
  struct termios uartConf;

public:
  MP_Uart(string device, unsigned long vel);
  ~MP_Uart();


};

template <typename pIn, typename pOut, MPConf conf>
MP_Uart<pIn, pOut, conf>::MP_Uart(string device, unsigned long vel) : MP_Fd<pIn, pOut, conf>() {
  mpUart_db("[MP_Uart] Opening pipe...\n");
  int fd = open(device.c_str(), O_RDWR | O_NOCTTY); //| O_NDELAY

  if (fd == -1) {
    // todo gestire la comunicazione dell'errore
    // system("ls /dev/ttyACM* -l");
    //    throw UartException("Failed to open port and get FD", errno);
    exit(-1);
  }
  if (!isatty(fd)) {
    // todo gestire la comunicazione dell'errore
    //    throw UartException("Not Uart", errno);
    exit(-1);
  }
  if (tcgetattr(fd, &uartConf)) {
    // todo gestire la comunicazione dell'errore
    //    throw UartException("Impossibile leggere la configurazione", errno);
    exit(-1);
  }
  /// *Input* flags - Turn off input processing
  uartConf.c_iflag &= ~(IGNBRK | BRKINT | ICRNL | INLCR | PARMRK | INPCK | ISTRIP | IXON);
  /// *Output* flags - Turn off output processing
  uartConf.c_oflag = 0;
  // Output flags - Turn off output processing
  uartConf.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);
  // Turn off character processing
  uartConf.c_cflag &= ~(CSIZE | PARENB);
  uartConf.c_cflag |= CS8;
  uartConf.c_cc[VMIN] = 1;   // One input byte is enough to return from read()
  uartConf.c_cc[VTIME] = 0;  // Inter-character timer off
  // Communication speed (simple version, using the predefined constants)
  if (cfsetispeed(&uartConf, vel) || cfsetospeed(&uartConf, vel)) {
    // todo gestire la comunicazione dell'errore
    // throw UartException("Impossibile Impostare velocità di cominicazione", errno);
    exit(-1);
  }
  // Finally, apply the configuration
  if (tcsetattr(fd, TCSANOW, &uartConf)) {
    // todo gestire la comunicazione dell'errore
    //    throw UartException("Impossibile Impostare i parametri selezionati", errno);
    exit(-1);
  }

  // Exclusive Access
  // If you wish ensure exclusive access to the serial device then use ioctl to set TIOCEXCL.
  // If your system includes programs like ModemManager then you should set this attribute.
  if (ioctl(fd, TIOCEXCL, NULL) < 0) {
    // todo gestire la comunicazione dell'errore
    //    throw UartException("Impossibile Ottenere l'uso esclusivo", errno);
    exit(-1);
  }

  // Ripulisco la memoria del driver
  tcflush(fd, TCIOFLUSH);
  this->fdR = fd;
  this->fdW = fd;
}

template <typename pIn, typename pOut, MPConf conf> MP_Uart<pIn, pOut, conf>::~MP_Uart() { close(this->fdR); }

} // namespace EMP

#endif // CMAKE_COMPILING
#endif // EMP_LIB_02_SRC_EMPLIBS_MP_UART_H
