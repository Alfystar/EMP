//
// Created by alfy on 13/05/21.
//

#ifndef EMP_LIB_EMPLIBS_MP_SERIAL_H
#define EMP_LIB_EMPLIBS_MP_SERIAL_H
#ifdef ARDUINO
#include <Arduino.h>
#include <MPcore/MP.h>

#ifdef HAVE_HWSERIAL1
#define DBserial Serial1
#else
#define DBserial Serial
#endif
#define print_STD(Level, text)                                                                                         \
  do {                                                                                                                 \
    Level {                                                                                                            \
      DBserial.print("[" #Level "]: ");                                                                                \
      DBserial.print(text);                                                                                            \
    }                                                                                                                  \
  } while (0)

// Level 0 := No Message
// Level 1 := Error msg
// Level 2 := Notice msg
// Level 3 := Info msg
// Level 4 := DB msg
#define MP_SERIAL_VervoseLevel 5
#define MP_SERIAL_err _codeActive(1, MP_SERIAL_VervoseLevel)
#define MP_SERIAL_notice _codeActive(2, MP_SERIAL_VervoseLevel)
#define MP_SERIAL_info _codeActive(3, MP_SERIAL_VervoseLevel)
#define MP_SERIAL_Db _codeActive(4, MP_SERIAL_VervoseLevel)

#define mpSerial_err(text) print_STD(MP_SERIAL_err, text)
#define mpSerial_notice(text) print_STD(MP_SERIAL_notice, text)
#define mpSerial_info(text) print_STD(MP_SERIAL_info, text)
#define mpSerial_db(text) print_STD(MP_SERIAL_Db, text)

namespace EMP {
templatePar() class MP_Serial : public MP<templateParCall()> {

  HardwareSerial &com;
  CircularBuffer<uint8_t, cbBinSize> byteSend;
  unsigned long lastDecodeTime;

public:
  explicit MP_Serial(HardwareSerial &serial);
  ~MP_Serial();
  void begin(unsigned long comVel);
  void updateState();

private:
  void trySend();
  void poolRead();

public:
  int16_t getData_try(pIn *pack) override; // return the residual pack available after the remove
  int16_t getData_wait(pIn *pack) override;

protected:
  // return -2  :=  Not enough space
  int packSend_Concrete(uint8_t *stream, uint16_t len) override;
  void packTimeRefresh() override;

public:
  unsigned long lastPackElapsed() override;
};

templatePar() MP_Serial<templateParCall()>::MP_Serial(HardwareSerial &serial) : MP<templateParCall()>(), com(serial) {
  mpSerial_db("[MP_Serial] beginning...");
  mpSerial_db("[MP_Serial] byteSend cleaning...");
  byteSend.memClean();
}

templatePar() MP_Serial<templateParCall()>::~MP_Serial() { com.end(); }

templatePar() void MP_Serial<templateParCall()>::begin(unsigned long comVel) { com.begin(comVel); }

templatePar() void MP_Serial<templateParCall()>::updateState() {
  trySend();
  poolRead();
}

templatePar() void MP_Serial<templateParCall()>::trySend() {
  while (com.availableForWrite() > 0 && !byteSend.isEmpty()) {
    com.write(byteSend.get());
  }
}

templatePar() void MP_Serial<templateParCall()>::poolRead() {
  // reload the byteRecive buffer form the Serial Subsystem
  while (com.available())
    this->byteRecive.put((uint8_t)com.read());
  // analizzo i dati
  this->byteParsing();
}

templatePar() int16_t MP_Serial<templateParCall()>::getData_try(pIn *pack) {
  updateState();
  return MP<templateParCall()>::getData_try(pack);
}

templatePar() int16_t MP_Serial<templateParCall()>::getData_wait(pIn *pack) {
  while (this->dataAvailable() < 1) {
    updateState();
  }
  return getData_try(pack);
}

templatePar() unsigned long MP_Serial<templateParCall()>::lastPackElapsed() {
  return micros() - lastDecodeTime; // Over Flow proof thanks the Unsigned Long type
}

templatePar() int MP_Serial<templateParCall()>::packSend_Concrete(uint8_t *stream, uint16_t len) {
  if (com.availableForWrite() + byteSend.availableSpace() < len)
    return -2;
  trySend(); // Push out the buffered byte
  uint16_t i = 0;
  while (com.availableForWrite() > 0 && len > 0) {
    com.write(stream[i++]);
    len--;
  }

  while (len > 0) {
    byteSend.put(stream[i++]);
    len--;
  }

  return 0;
}

templatePar() void MP_Serial<templateParCall()>::packTimeRefresh() { lastDecodeTime = micros(); }

} // namespace EMP
#endif //#ifdef ARDUINO
#endif // EMP_LIB_EMPLIBS_MP_SERIAL_H
