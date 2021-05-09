//
// Created by alfy on 04/05/21.
//

#ifndef EMP_LIB_UARTDATATRANSFERT_H
#define EMP_LIB_UARTDATATRANSFERT_H
#include "circularBuffer/CircularBuffer.h"
#include "cobs-c/cobs.h"
#include "conf.h"
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
 *  - implement the packSend_Concrete, specific for the architetture
 *  - fill byteRecive when byte are dataAvailable and than parse him with byteParsing() function
 */
template <typename pIn, typename pOut, MPConf conf> class MP {
#define MAXPackINsize (sizeof(pIn) + 1)   // pack plus CRC8
#define MAXPackOUTsize (sizeof(pOut) + 1) // pack plus CRC8
#define noLastStartIndex ((u_int16_t)-1)  // Variable is unsigned, so the result will be 65535
protected:
  // Buffering recived byte
  u_int16_t lastStartIndex = 0;
  CircularBuffer<u_int8_t, conf.binaryBufElement * MAXPackINsize> byteRecive; // space to save byte read before parsing

private:
  // Succesfull pack recive
  CircularBuffer<pIn, conf.cbSize> packRecive;

public:
  // Instance operation
  MP();
  ~MP();
  void bufClear();

  /// Data Send & Get
  int packSend(pOut *pack, u_int16_t bSize); // Il pacchetto potrebbe avere una dimensione minore della massima
  int packSend(pOut *pack);                  // Il pacchetto potrebbe avere una dimensione minore della massima
  // Data
  u_int16_t dataAvailable();
  int16_t getData_try(pIn *pack);              // return the residual pack available after the remove
  virtual int16_t getData_wait(pIn *pack) = 0; // return the residual pack available after the remove

protected:
  virtual int packSend_Concrete(u_int8_t *stream, u_int16_t len) = 0; // return -1 if error occult
  int packSend_Concrete(u_int8_t byteSend);                           // return -1 if error occult

  // Son have to call after the insertion inside the byteParsing buffer
  u_int16_t byteParsing(); // return How many pack are found
};

/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Instance operation

template <typename pIn, typename pOut, MPConf conf> MP<pIn, pOut, conf>::MP() { bufClear(); }

template <typename pIn, typename pOut, MPConf conf> MP<pIn, pOut, conf>::~MP() {}

template <typename pIn, typename pOut, MPConf conf> void MP<pIn, pOut, conf>::bufClear() {
  // If error are reach, the usedSpace of the byteRecive is realy HIGH, should reduce "binaryBufElement" inside conf.h,
  // or comment this line instead if is fine
  BUILD_BUG_ON((conf.binaryBufElement * sizeof(pIn)) >= 4096);

  byteRecive.memClean();
  packRecive.memClean();
  lastStartIndex = 0;
}
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/// Data Send & Get
/*    On success return 0
 *    On fail return -1
 */
template <typename pIn, typename pOut, MPConf conf> int MP<pIn, pOut, conf>::packSend(pOut *pack, u_int16_t bSize) {

  int ret = 0;
  if (conf.VarSizePack_enable != 0) { // Fixed Size pack
    // todo: Pensare il protocollo fixed size
    return -1;
    //    int zerosLeft = conf.VarSizePack_enable - bSize; // if zerosLeft are negative, not enter in the for
    //    ret = packSend_Concrete(pack, bSize);
    //    for (int i = 0; i < zerosLeft; i++) {
    //      ret = packSend_Concrete(0);
    //      if (ret)
    //        return ret;
    //    }
    //    if (conf.CRC8_enable)
    //      ret = packSend_Concrete(crc8_stream((u_int8_t *)pack, bSize));
    //    return ret;
  }

  /// ######################## Packetize With CRC8  ########################
  u_int16_t packSize = bSize + conf.CRC8_enable;
  u_int8_t packBuf[packSize]; // CRC8 may add 1 byte
  memcpy(packBuf, pack, bSize);

  if (conf.CRC8_enable) {
    packBuf[packSize - 1] = crc8_stream((u_int8_t *)pack, bSize);
  }

  /// ###################### Encoding pack with COBS  ######################
  u_int16_t sendSize = packSize + 1; // Cobs add 1 byte

  u_int8_t sendBuf[sendSize];

  cobs_encode_result res = cobs_encode(sendBuf, sendSize, packBuf, packSize);
  if (res.status != COBS_ENCODE_OK)
    return -1;
  ret = packSend_Concrete(sendBuf, sendSize);
  ret = packSend_Concrete(0); // Delimit code
  return ret;
}

template <typename pIn, typename pOut, MPConf conf> int MP<pIn, pOut, conf>::packSend(pOut *pack) {
  return packSend(pack, sizeof(pOut));
}

template <typename pIn, typename pOut, MPConf conf> u_int16_t MP<pIn, pOut, conf>::dataAvailable() {
  return this->packRecive.usedSpace();
}

// On success: copy pack Logic, inside *pack are saved the tail data if possible,
// On fail: return -1 and *pack aren't touch
template <typename pIn, typename pOut, MPConf conf> int16_t MP<pIn, pOut, conf>::getData_try(pIn *pack) {
  if (packRecive.isEmpty())
    return -1;
  // If data are dataAvailable
  memcpy(pack, packRecive.getPtr(), sizeof(pIn));
  return dataAvailable();
}

template <typename pIn, typename pOut, MPConf conf> int MP<pIn, pOut, conf>::packSend_Concrete(u_int8_t byteSend) {
  return packSend_Concrete(&byteSend, 1);
}

/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/// Byte parsing using CRC8 and COBS to
template <typename pIn, typename pOut, MPConf conf> u_int16_t MP<pIn, pOut, conf>::byteParsing() {
  u_int8_t dato;
  u_int16_t datoId;
  u_int16_t packFound = 0;
  while (!byteRecive.isEmpty()) {
    // Get the byte and his position (if is a 0, i need to save)
    dato = byteRecive.get(&datoId);

    if (conf.VarSizePack_enable != 0) { // Fixed Size pack
      // todo: Pensare il protocollo fixed size
      continue;
    }

    if (dato != 0)
      continue;

    /// ########################## COBS DECODE ##########################
    // NB:COBS protocol add 1 byte at the pack, At the start
    u_int16_t COBSsrcSize = byteRecive.countSlotBetween(lastStartIndex, datoId);

    if (COBSsrcSize - 1 > MAXPackINsize) {
      // Someting wrong, no 0 was recived in time, the pack are lost
      continue;
    }
    // Fill the buffer for the decoding
    u_int8_t COBSEncoded[COBSsrcSize];
    byteRecive.memcpyCb(COBSEncoded, lastStartIndex, COBSsrcSize);
    u_int8_t COBSDecode[MAXPackINsize];

    cobs_decode_result res = cobs_decode(COBSDecode, MAXPackINsize, COBSEncoded, COBSsrcSize);
    lastStartIndex = datoId + 1; // From now, in any case, datoId are the new lastStartIndex

    if (res.status != COBS_DECODE_OK) {
      continue;
    }
    /// ######################## CRC8 VALIDATION ########################
    if (conf.CRC8_enable) {
      u_int8_t calcCRC = crc8_stream(COBSDecode, res.out_len - 1); // Last byte are the CRC
      if (calcCRC != COBSDecode[res.out_len - 1])
        continue; // CRC8 Fail!!!
    }
    packRecive.put((pIn *)COBSDecode, res.out_len - 1);
    packFound++;
  } //  while (!byteRecive->isEmpty())
  return packFound;
}

} // namespace EMP

#endif // EMP_LIB_UARTDATATRANSFERT_H