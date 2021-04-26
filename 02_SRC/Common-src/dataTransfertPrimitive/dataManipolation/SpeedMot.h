//
// Created by alfyhack on 15/10/19.
//

#ifndef SPEEDMOT_H
#define SPEEDMOT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../dataPrimitive.h"
#include "../DataFactory.h"

#ifndef LinuxSide
#include "Arduino.h"
// Essendo linkata nel ".project" di sloeber, gcc la vede nella sua vera posizione
#include "../../../ScoreBoard_Uart_Firmware/CustomCodeDefine.h"
#endif

namespace DataManipolation {
    using namespace DataPrimitive;

    class SpeedMot{
    public:
        SpeedMot (mSpeed &s);
        SpeedMot (mSpeed *s);
        #ifdef LinuxSide
        SpeedMot ();   // creo al mio interno Pack
        ~SpeedMot ();
        #endif
        void changePack (mSpeed &speed);
        void copyPack (SpeedMot &speed);
        void copyPack (SpeedMot *speed);
        void copyPack (mSpeed &speed);
        mSpeed &getPwm ();
        short getPwm (motCode mot);
        void pwmSet (mSpeed &pwms);
        void pwmSet (mSpeed *pwms);
        void pwmSet (short m1, short m2, short m3, short m4, short m5, short m6);
        void pwmSet (motCode mot, short pwm);
        void softAllBreak ();
        void printSpeed ();
#if defined LinuxSide || defined SPEED_MOT_DB
        static void printSpeed (mSpeed *speed);
        static void printSpeed (mSpeed &speed);
#endif
    private:
        mSpeed *speedPack;
        #ifdef LinuxSide
        bool speedMine = false;
        #endif
    };
}
#endif //SPEEDMOT_H
