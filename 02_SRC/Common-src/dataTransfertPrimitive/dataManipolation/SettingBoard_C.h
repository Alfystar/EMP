//
// Created by alfyhack on 15/10/19.
//

#ifndef SETTINGBOARD_C_H
#define SETTINGBOARD_C_H

#include <stdio.h>
#include <string.h>
#include "../dataPrimitive.h"
#include "../DataFactory.h"

#ifndef LinuxSide
#include "Arduino.h"
// Essendo linkata nel ".project" di sloeber, gcc la vede nella sua vera posizione
#include "../ScoreBoard_Uart_Firmware/CustomCodeDefine.h"
#endif

namespace DataManipolation {
    using namespace DataPrimitive;

    class SettingBoard_C{
    public:
        SettingBoard_C (settingsBoard &s);
        SettingBoard_C (settingsBoard *s);
        #ifdef LinuxSide
        SettingBoard_C ();   // creo al mio interno Pack
        ~SettingBoard_C ();
        #endif
        void changePack (settingsBoard &sets);
        void copyPack (SettingBoard_C &sets);
        void copyPack (settingsBoard &sets);
        settingsBoard &getSetting ();
        void setMotorLimit (motCode mot, short enMax, short enMin, short cur);
        void setMaxEn (motCode mot, short en);
        void setMinEn (motCode mot, short en);
        void setMaxCurrentMed (motCode mot, short current);
        void setAdcRef (adcRef adc);
        void setAdcDiff (bool diff);
        void setPWMfreq (pwmFreq freq);
#if defined LinuxSide || defined SETTING_CLASS_DB
        void printSetting ();
        static void printSetting (settingsBoard &set);
#endif
    private:
        settingsBoard *setPack;
        #ifdef LinuxSide
        bool setMine = false;
        #endif
    };
}
#endif //SETTINGBOARD_C_H
