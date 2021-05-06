//
// Created by alfy on 04/05/21.
//

#ifndef EMP_LIB_UARTDATATRANSFERT_H
#define EMP_LIB_UARTDATATRANSFERT_H
#include "conf.h"
#include "circularBuffer/CircularBuffer.h"
#include "cobs-c/cobs.h"
#include "crc8/crc8.h"
#include "macro.h"
#include <cstdint>

namespace EMP {
/* The class MP (Message Pack) perform a GENEARAL and PLATFORM AGNOSTIC Data Pack Transfert
 * NB: IS responsibility of the caller undestand what pack are recived, this library only send/recive the pack.
 * Anyway, if you need to send different type of message, YOU CAN!!
 * The only thinks to do are create a unions between all of the the possible data , and use one field to tag him.
 *
 * Template Parameter:
 * pIn := One or more than one type of pack (if multiple use union with a tag field at start can help) can be send
 * pOut:= One or more than one type of pack (if multiple use union with a tag field at start can help) can be receive
 *
 * Son Classes HAVE TO:
 *  - implement the packSend, specific for the architetture
 *  - fill byteRecive when byte are available and than parse him with byteParsing() function
 */
template <typename pIn, typename pOut> class MP {
#define MAXPackINsize (sizeof(pIn) + 1)   // pack plus CRC8
#define MAXPackOUTsize (sizeof(pOut) + 1) // pack plus CRC8
protected:
  // Buffering recived byte
  u_int16_t lastZeroIndex = -1; // -1 unsigned mean all bit at 1, but the max usedSpace are low than this, so ok
  CircularBuffer<u_int8_t, binaryBufElement * MAXPackINsize> byteRecive; // space to save byte read before parsing

private:
  // Succesfull pack recive
  CircularBuffer<pIn, cbSize> packRecive;

public:
  // Instance operation
  MP();
  ~MP();
  void bufClear();

  /// Data Send & Get
  int packSend(pOut *pack, u_int16_t bSize); // Il pacchetto potrebbe avere una dimensione minore della massima
  // Data
  u_int16_t available();
  u_int16_t getData(pIn *pack); // return the available pack after the remotion of the current

protected:
  virtual void packSend_Concrete(u_int8_t *stream, u_int16_t len) = 0;
  void byteParsing(); // Son have to call after the insertion inside the byteRecive buffer
};

} // namespace EMP

#endif // EMP_LIB_UARTDATATRANSFERT_H
