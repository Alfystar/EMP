//
// Created by alfy on 04/05/21.
//

#ifndef EMP_LIB_CONF_H
#define EMP_LIB_CONF_H
#include <cstdint>
/*

 */
/*
#if defined(ARDUINO_H)
#define cbSize 8
#define binaryBufElement 4
//#elif //to add Other architetture
#else// Default ENV are inside Linux
#define cbSize 256
#define binaryBufElement 32
#endif
*/
/* This structure are use to describe the behaviour of EMP classes, not have to be use in the Concrete Class
 * CRC8_enable        := True to enable CRC8 Calculation and Verification, add 1 byte to all pack
 * VarSizePack_enable := If 0, COBS Encoding are use to send variable Pack size, otherwise set the fixed pack size
 * cbSize             := Capacity (in term of pIn items) of the Circular Buffer use to store the correctly handle pIn
 * binaryBufElement   := Capacity (in term of pIn items) of the Circular Buffer use to store the received byte
 *                       (in transfert byte)
 *
 * N.B. True HAVE TO BE 1 and False 0, other wise, the logic will fail
 */
typedef struct MPConf_ {
  bool CRC8_enable;
  uint16_t VarSizePack_enable;
  uint16_t cbSize;
  uint16_t binaryBufElement;
} MPConf;

#define configDeclare(name, CRC8_enable, VarSizePack_enable, cbSize, binaryBufElement)                                 \
  constexpr MPConf name { CRC8_enable, VarSizePack_enable, cbSize, binaryBufElement }

#define ArduinoMP_ConfDefault(name) configDeclare(name, true, 0, 8, 4)
#define LinuxMP_ConfDefault(name) configDeclare(name, true, 0, 256, 32)

// MPConf test = ArduinoMP_ConfDefault();
#endif // EMP_LIB_CONF_H
