//
// Created by alfy on 06/05/21.
//

#ifndef EMP_LIB_02_SRC_EMPLIBS_EMP_INCLUDE_H
#define EMP_LIB_02_SRC_EMPLIBS_EMP_INCLUDE_H

// Specialize class included based on the Platform in use
#if defined(ARDUINO_H)
//#include "MP_Arduino.h"
#elif defined(STM32)
//#include "MP_STM32.h"
#else// Default ENV are inside Linux
#include "MP_Pipe.h"


#endif // EMP_LIB_02_SRC_EMPLIBS_EMP_INCLUDE_H
