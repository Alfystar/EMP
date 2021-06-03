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
//#include <cstdint>
#include <stdint.h>

#define Kib (1024)

#if __cplusplus > 201703L // superiore al C++17
#define templatePar() template <typename pIn, typename pOut, MPConf conf>
#define templateParCall() pIn, pOut, conf

#define CRC8_enable() conf.CRC8_enable
#define cdBinStore() conf.cdBinStore
#define cbPackStore() conf.cbPackStore

#else
#define templatePar()                                                                                                  \
  template <typename pIn, typename pOut, bool CRC8_enable, uint16_t cdBinStore, uint16_t cbPackStore>
#define templateParCall() pIn, pOut, CRC8_enable, cdBinStore, cbPackStore

#define CRC8_enable() CRC8_enable
#define cdBinStore() cdBinStore
#define cbPackStore() cbPackStore

#endif

namespace EMP {
/* The class MP (Message Pack) perform a GENERAL and PLATFORM AGNOSTIC Data Pack Transfer
 * NB: IS responsibility of the caller understand what pack are received, this library only send/receive the pack.
 * Anyway, if you need to send different type of message, YOU CAN!!
 * The only thinks to do are create a unions between all of the the possible data , and use one field to tag him.
 *
 * Template Parameter:
 * pIn := One or more than one type of pack (if multiple use union with a tag field at start can help) can be send
 * pOut:= One or more than one type of pack (if multiple use union with a tag field at start can help) can be receive
 *
 * Son Classes HAVE TO:
 *  - implement the packSend_Concrete, specific for the architecture
 *  - fill byteReceive when byte are dataAvailable and than parse him with byteParsing() function
 */
templatePar() class MP {
#define MAXPackINsize (uint16_t)(sizeof(pIn) + CRC8_enable())   // pack plus CRC8
#define MAXPackOUTsize (uint16_t)(sizeof(pOut) + CRC8_enable()) // pack plus CRC8
#define cbBinSize cdBinStore() * MAXPackINsize

protected:
  //#################################################################################################################//
  // All the Call-back will be execute from the thread who calls the function.
  // - In a multi-thread context (like linux) this can be a problem if the call-back are slow, so is method aren't
  //   recommended, if possible, put some thread in polling waiting could be more efficient solution.
  // - In mono-thread context, like embedded system bare metal, this technic is probably the most appropriate method to
  //   respond promptly at the system request.

  /// Call back function define ## FOR the specific template Class##
  /// (Pay attention here!!! Differente template class need different callback function with a DIFFERENT object pointer)
  typedef void (*packDetectCallBack)(MP<templateParCall()> *myInstance);  // Call every times a pack are detected

  typedef struct _callBacks {
    packDetectCallBack pkDetect = nullptr;
  } callBacksMP;
  //#################################################################################################################//
protected:
  callBacksMP clback;
  // Buffering received byte
  uint16_t lastStartIndex = 0;
  CircularBuffer<uint8_t, cbBinSize> byteRecive; // space to save byte read before parsing

private:
  // Successfully pack received
  CircularBuffer<pIn, cbPackStore()> packRecive;

public:
  // Instance operation
  MP();
  MP(callBacksMP clback_);
  void bufClear();

  /// Data Send & Get
  // Data Send
  // return 0 on success, -1 cobs error,  other wise, the error code from the packSend_Concrete
  __attribute__((always_inline)) int packSend(pOut *pack);
  int packSend(pOut *pack, uint16_t bSize); // Il pacchetto potrebbe avere una dimensione minore della massima

  // Data get
  uint16_t dataAvailable();
  virtual int16_t getData_try(pIn *pack);      // return the residual pack available after the remove
  virtual int16_t getData_wait(pIn *pack) = 0; // return the residual pack available after the remove

protected:
  // 0 on success, other wise, error code (!=-1) and data are discard
  virtual int packSend_Concrete(uint8_t *stream, uint16_t len) = 0;
  __attribute__((always_inline)) int packSend_Concrete(uint8_t byteSend);

  // Son have to call after the insertion inside the byteParsing buffer
  uint16_t byteParsing();             // return How many pack are found
  virtual void packTimeRefresh() = 0; // Call by byteParsing when new pack are recived, used to measure the time
public:
  virtual unsigned long lastPackElapsed() = 0; // retur micro second (10^-6 sec) elapsed since last pack recived
};

/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Instance operation

templatePar() MP<templateParCall()>::MP() : MP(nullptr) {}

templatePar() MP<templateParCall()>::MP(callBacksMP clback_) : clback(clback_) { bufClear(); }

templatePar() void MP<templateParCall()>::bufClear() {
  // If error are reach, the usedSpace of the byteRecive is realy HIGH, should reduce "cdBinStore" inside conf.h,
  // or comment this line instead if is fine
  BUILD_BUG_ON((cbPackStore() * sizeof(pIn)) >= 4 * Kib); // Warning, buffer use Too memory, use less config size
  byteRecive.memClean();
  packRecive.memClean();
  lastStartIndex = 0;
}
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/// Data Send & Get
/*    On success return 0
 *    On fail return -1
 */

templatePar() int MP<templateParCall()>::packSend(pOut *pack) { return packSend(pack, sizeof(pOut)); }

templatePar() int MP<templateParCall()>::packSend(pOut *pack, uint16_t bSize) {

  int ret = 0;

  /// ######################## Packetize With CRC8  ########################
  uint16_t packSize = bSize + CRC8_enable();
  uint8_t packBuf[packSize]; // CRC8 may add 1 byte
  memcpy(packBuf, pack, bSize);

  if (CRC8_enable()) {
    packBuf[packSize - 1] = crc8_stream((uint8_t *)pack, bSize);
  }

  /// ###################### Encoding pack with COBS  ######################
  uint16_t sendSize = packSize + 1; // Cobs add 1 byte

  uint8_t sendBuf[sendSize + 1]; // +1 for the final 0

  cobs_encode_result res = cobs_encode(sendBuf, sendSize, packBuf, packSize);
  if (res.status != COBS_ENCODE_OK)
    return -1;

  sendBuf[sendSize] = '\0';
  // Send complete pack anyway, the concrete sub-system have to bufferize the pack
  if ((ret = packSend_Concrete(sendBuf, sendSize + 1)) != 0)
    return ret;

  return 0;
}

templatePar() uint16_t MP<templateParCall()>::dataAvailable() { return this->packRecive.usedSpace(); }

// On success: copy pack Logic, inside *pack are saved the tail data if possible,
// On fail: return -1 and *pack aren't touch
templatePar() int16_t MP<templateParCall()>::getData_try(pIn *pack) {
  if (packRecive.isEmpty())
    return -1;
  // If data are dataAvailable
  memcpy(pack, packRecive.getPtr(), sizeof(pIn));
  return dataAvailable();
}

templatePar() int MP<templateParCall()>::packSend_Concrete(uint8_t byteSend) { return packSend_Concrete(&byteSend, 1); }

/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/// Byte parsing using CRC8 and COBS to
templatePar() uint16_t MP<templateParCall()>::byteParsing() {
  uint8_t dato;
  uint16_t datoId;
  uint16_t packFound = 0;
  while (!byteRecive.isEmpty()) {
    // Get the byte and his position (if is a 0, need to be saved)
    dato = byteRecive.get(&datoId);

    if (dato != 0)
      continue;

    /// ########################## COBS DECODE ##########################
    // NB:COBS protocol add 1 byte at the pack, At the start
    uint16_t COBSsrcSize = byteRecive.countSlotBetween(lastStartIndex, datoId);

    if (COBSsrcSize - 1 > MAXPackINsize || COBSsrcSize < 2) {
      // Someting wrong, no 0 was recived in time, or too many zero are received
      // Anyway the pack are lost
      lastStartIndex = datoId + 1; // restart the logic from the next byte
      continue;
    }
    // Fill the buffer for the decoding
    uint8_t COBSEncoded[COBSsrcSize];
    byteRecive.memcpyCb(COBSEncoded, lastStartIndex, COBSsrcSize);
    uint8_t COBSDecode[MAXPackINsize];

    cobs_decode_result res = cobs_decode(COBSDecode, MAXPackINsize, COBSEncoded, COBSsrcSize);
    lastStartIndex = datoId + 1; // From now, in any case, datoId are the new lastStartIndex

    if (res.status != COBS_DECODE_OK) {
      continue;
    }
    /// ######################## CRC8 VALIDATION ########################
    if (CRC8_enable()) {
      uint8_t calcCRC = crc8_stream(COBSDecode, res.out_len - 1); // Last byte are the CRC
      if (calcCRC != COBSDecode[res.out_len - 1])
        continue; // CRC8 Fail!!!
    }
    packRecive.put((pIn *)COBSDecode, res.out_len - 1);
    packTimeRefresh(); // from now, the pack are available to the system
    packFound++;
    if (clback.pkDetect)
      clback.pkDetect(this);
  } //  while (!byteRecive->isEmpty())
  return packFound;
}

} // namespace EMP

#endif // EMP_LIB_UARTDATATRANSFERT_H
