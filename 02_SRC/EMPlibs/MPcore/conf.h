//
// Created by alfy on 04/05/21.
//

#ifndef EMP_LIB_CONF_H
#define EMP_LIB_CONF_H
#include <cstdint>

/* This structure are use to describe the behaviour of EMP classes, not have to be use in the Concrete Class
 * CRC8_enable        := True to enable CRC8 Calculation and Verification, add 1 byte to all pack
 * cbSize             := Capacity (in term of pIn items) of the Circular Buffer use to store the correctly handle pIn
 * binaryBufElement   := Capacity (in term of pIn items) of the Circular Buffer use to store the received byte
 *                       (in transfert byte)
 *
 * N.B. True HAVE TO BE 1 and False 0, other wise, the logic will fail
 */
typedef struct MPConf_ {
  // Compatibility features
  bool CRC8_enable;

  // local storage configuration
  uint16_t cbSize;
  uint16_t binaryBufElement;
} MPConf;

#define configDeclare(name, CRC8_enable, cbSize, binaryBufElement)                                                     \
  constexpr MPConf name { CRC8_enable, cbSize, binaryBufElement }

#define ArduinoMP_ConfDefault(name) configDeclare(name, true, 8, 4)
#define LinuxMP_ConfDefault(name) configDeclare(name, true, 256, 32)

// MPConf test = ArduinoMP_ConfDefault();
#endif // EMP_LIB_CONF_H
