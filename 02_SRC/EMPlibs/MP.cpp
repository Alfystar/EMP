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
  // If error are reach, the usedSpace of the byteRecive is realy HIGH, should reduce "binaryBufElement" inside conf.h,
  // or comment this line instead if is fine
  BUILD_BUG_ON((binaryBufElement * sizeof(pIn)) >= 4096);

  byteRecive.memClean();
  packRecive.memClean();
  lastZeroIndex = -1;
}
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/// Data Send & Get
/*    On success return 0
 *    On fail return -1
 */
template <typename pIn, typename pOut> int MP<pIn, pOut>::packSend(pOut *pack, u_int16_t bSize) {
  /// ######################## Packetize With CRC8  ########################
  u_int8_t packettize[bSize + 1];  // CRC8 add 1 byte
  memcpy(packettize,pack,bSize);
  packettize[bSize] = crc8_stream(pack, bSize); //todo: verify the index

  /// ###################### Encoding pack with COBS  ######################
  u_int8_t COBSEncoded[bSize + 2]; // Cobs add 1 byte, CRC8 add 1 byte
  cobs_encode_result res =  cobs_encode(COBSEncoded, bSize + 2, packettize, bSize + 1);
  if(res.status != COBS_ENCODE_OK)
    return -1;

  packSend_Concrete(COBSEncoded, bSize);
}

template <typename pIn, typename pOut> u_int16_t MP<pIn, pOut>::available() { return this->packRecive->usedSpace(); }

// copy pack Logic,
// if possible, return true and in *pack are copied the data
// otherwise false are return and *pack aren't touch
template <typename pIn, typename pOut> u_int16_t MP<pIn, pOut>::getData(pIn *pack) {
  if (packRecive->isEmpty())
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
  while (!byteRecive->isEmpty()) {
    // Get the byte and his position (if is a 0, i need to save)
    dato = byteRecive.get(&datoId);
    if (dato != 0)
      continue;
    if (lastZeroIndex == -1) {
      lastZeroIndex = datoId;
      continue;
    }
    /// ########################## COBS DECODE ##########################
    // NB:COBS protocol add 1 byte at the pack, At the start
    u_int16_t COBSsrcSize = byteRecive.countSlotBetween(lastZeroIndex + 1, datoId);
    lastZeroIndex = datoId; // From now, in any case, datoId are the new lastZeroIndex

    if (COBSsrcSize - 1 > MAXPackINsize) {
      // Someting wrong, no 0 was recived in time, the pack are lost
      continue;
    }
    // Fill the buffer for the decoding
    u_int8_t COBSEncoded[COBSsrcSize];
    byteRecive.memcpyCb(COBSEncoded, lastZeroIndex + 1, COBSsrcSize);
    u_int8_t COBSDecode[MAXPackINsize];

    cobs_decode_result res = cobs_decode(COBSDecode, MAXPackINsize, COBSEncoded, COBSsrcSize);
    if (res.status != COBS_DECODE_OK) {
      continue;
    }
    /// ######################## CRC8 VALIDATION ########################

    u_int8_t calcCRC = crc8_stream(COBSDecode, res.out_len - 1); // Last byte are the CRC
    if (calcCRC == COBSDecode[res.out_len - 1]) {                // todo: verify the index of CRC8
      // CRC8 SUCCESS!!!
      packRecive.put(COBSDecode, COBSsrcSize - 1);
    } else { // CRC8 fail
    }
  } //  while (!byteRecive->isEmpty())
}
