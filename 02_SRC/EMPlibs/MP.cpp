//
// Created by alfy on 04/05/21.
//

#include "MP.h"

using namespace EMP;

/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Instance operation

template <typename pIn, typename pOut> MP<pIn, pOut>::MP() { bufClear(); }

template <typename pIn, typename pOut> MP<pIn, pOut>::~MP() {}

template <typename pIn, typename pOut> void MP<pIn, pOut>::bufClear() {
  // If error are reach, the size of the byteRecive is realy HIGH, should reduce "binaryBufElement" inside conf.h,
  // or comment this line instead if is fine
  BUILD_BUG_ON((binaryBufElement * sizeof(pIn)) >= 4096);

  byteRecive.memClean();
  packRecive.memClean();
  lastZeroIndex = -1;
}
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/// Data Send & Get

template <typename pIn, typename pOut> void MP<pIn, pOut>::packSend(pOut *pack, u_int16_t size) {
  // todo: calc CRC8 and flop to COBS format, then send using packSend_Concrete()
}

template <typename pIn, typename pOut> u_int16_t MP<pIn, pOut>::available() {
  return this->packRecive->size();
}

// copy pack Logic,
// if possible, return true and in *pack are copied the data
// otherwise false are return and *pack aren't touch
template <typename pIn, typename pOut> u_int16_t MP<pIn, pOut>::getData(pIn *pack) {
  if (packRecive->empty())
    return 0;
  // If data are available
  *pack = packRecive.getHead();
  return available();
}

/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/// Byte parsing using CRC8 and COBS to
template <typename pIn, typename pOut> void MP<pIn, pOut>::byteParsing() {
  u_int8_t dato;
  u_int16_t datoId;
  while (!byteRecive->empty()) {
    // Get the byte and his position (if is a 0, i need to save)
    dato = byteRecive.get(&datoId);
    if (dato != 0)
      continue;
    if (lastZeroIndex == -1) {
      lastZeroIndex = datoId;
      continue;
    }
    /// ########################## COBS DECODE ##########################
    // NB:COBS protocol add 2 byte at the pack, At start and 0 at the END
    u_int16_t srcSize = modSub(datoId, lastZeroIndex, byteRecive.capacity()) - 2;
    if (srcSize > MAXPackINsize) {
      // Someting wrong, no 0 was recived in time, the pack are lost, restart from here
      lastZeroIndex = datoId;
      continue;
    }
    // Fill the buffer for the decoding
    u_int8_t srcRead[srcSize + 1]; // need space for the PRE-byte
    byteRecive.writeMemOut(srcSize, lastZeroIndex + 1, srcSize + 1);
    u_int8_t bufRead[srcSize];

    lastZeroIndex = datoId; // From now, in any case, datoId are teh lastZeroIndex

    cobs_decode_result res = cobs_decode(bufRead, MAXPackINsize, srcRead, srcSize);
    if (res.status != COBS_DECODE_OK) {
      continue;
    }
    /// ######################## CRC8 VALIDATION ########################

    u_int8_t calcCRC = crc8_stream(bufRead, srcSize - 1); // Last byte are the CRC
    if (calcCRC == bufRead[srcSize - 1]) {
      // CRC8 SUCCESS!!!
      packRecive.put(bufRead, srcSize - 1);
    } else { // CRC8 fail
    }
  } //  while (!byteRecive->empty())
}
