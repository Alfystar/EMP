//
// Created by alfyhack on 15/10/19.
//

#ifndef DATAFACTORY_H
#define DATAFACTORY_H

#include "dataPrimitive.h"
#include <stdlib.h>
#include <string.h>

namespace dataFactory {
    using namespace DataPrimitive;
    /// Primitive factory
    mSpeed &makeMSpeed ();
    void freeMSpeed (mSpeed *p);
    settingsBoard &makeSettingBoard ();
    void freeSettingBoard (settingsBoard *p);
    mCurrent &makeMCurrent ();
    void freeMCurrent (mCurrent *p);
    mEncoder &makeMEncoder ();
    void freeMEncoder (mEncoder *p);
    mAll &makeMAll ();
    void freeMAll (mAll *p);
    uart2Ard &makeData2Ard ();
    void freeData2Ard (uart2Ard *p);
    uart2Linux &makeData2Rasp ();
    void freeData2Rasp (uart2Linux *p);
    uart2Ard &makeUart2Ard ();
    void freeUart2Ard (uart2Ard *p);
    uart2Linux &makeUart2Rasp ();
    void freeUart2Rasp (uart2Linux *p);
}
#endif //DATAFACTORY_H
