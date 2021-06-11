//
// Created by alfy on 04/05/21.
//

#ifndef EMP_LIB_CONF_H
#define EMP_LIB_CONF_H
#include <stdint.h> // More general than <cstdint>

/* This Parameter are use to describe the behaviour of EMP classes, not have to be use in the Concrete Class
 * CRC8_enable  := True to enable CRC8 Calculation and Verification, add 1 byte to all pack
 * cbPackStore  := Capacity (in term of pIn items) of the Circular Buffer use to store the correctly handle pIn
 * cdBinStore   := Capacity (in term of pIn items) of the Circular Buffer use to store the byte not yet analyze/sended
 * RT_THREAD    := If True, the priority of the readerThread are move to FIFO
 * N.B. True HAVE TO BE 1 and False 0, other wise, the logic will fail
 */

#define configDeclare(CRC8_enable, cbPackStore, cdBinStore) CRC8_enable, cbPackStore, cdBinStore

#define ArduinoMP_template(crcEn) configDeclare(crcEn, 8, 4)
#define ArduinoMP_templateDefault() ArduinoMP_template(true)

#define STM32MP_template(crcEn) configDeclare(crcEn, 16, 4)
#define STM32MP_templateDefault() STM32MP_template(true)

#define LinuxMP_ConfBig(crcEn) configDeclare(crcEn, 256, 32)
#define LinuxMP_ConfMed(crcEn) configDeclare(crcEn, 64, 16)
#define LinuxMP_ConfSmall(crcEn) configDeclare(crcEn, 16, 8)

#endif // EMP_LIB_CONF_H
