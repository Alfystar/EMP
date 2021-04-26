//
// Created by alfyhack on 10/11/19.
//

#ifndef PCLISTENUART_DATATRANSFERT_ALLINCLUDE_H
#define PCLISTENUART_DATATRANSFERT_ALLINCLUDE_H

#ifndef LinuxSide
// Essendo linkata dentro sloeber, gcc la vede nella sua vera posizione
#include "../ScoreBoard_Uart_Firmware/CustomCodeDefine.h"
#endif

#include "dataPrimitive.h"
#include "DataFactory.h"
#include "dataManipolation/AllSensor.h"
#include "dataManipolation/CurrentMot.h"
#include "dataManipolation/EncoderMot.h"
#include "dataManipolation/SpeedMot.h"
#include "dataManipolation/SettingBoard_C.h"

#endif //PCLISTENUART_DATATRANSFERT_ALLINCLUDE_H
