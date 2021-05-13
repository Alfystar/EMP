//
// Created by alfy on 13/05/21.
//

#ifndef EMP_LIB_EMPLIBS_MP_SERIAL_H
#define EMP_LIB_EMPLIBS_MP_SERIAL_H

#include <Arduino.h>
#include <MPcore/MP.h>

#define templatePar()                                                                                                  \
  template <typename pIn, typename pOut, bool CRC8_enable, uint16_t cdBinStore, uint16_t cbPackStore>
#define templateParCall() pIn, pOut, CRC8_enable, cdBinStore, cbPackStore

namespace EMP {
templatePar() class MP_Serial : public MP<templateParCall()> {};




} // namespace EMP

#endif // EMP_LIB_EMPLIBS_MP_SERIAL_H
