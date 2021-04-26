//
// Created by alfyhack on 15/10/19.
//

#ifndef CURRENTMOT_H
#define CURRENTMOT_H

#include <stdio.h>
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

    class CurrentMot{
    public:
        CurrentMot (mCurrent &c);
        CurrentMot (mCurrent *c);
        #ifdef LinuxSide
        CurrentMot ();   // creo al mio interno il Pack
        ~CurrentMot ();
        #endif
        void changePack (mCurrent &c);
        void changePack (mCurrent *c);
        void copyCur (CurrentMot &c);
        void copyCur (mCurrent *c);
        void copyCur (mCurrent &c);
        mCurrent &getCurrent ();
        short getCurrent (motCode mot);
        void curSet (motCode mot, short cur);
#if defined LinuxSide || defined CURRENT_MOT_DB
        // <== Debug & Print ==>
        void printCurrent ();
        static void printCurrent (mCurrent *c);
        static void printCurrent (mCurrent &c);
#endif
    private:
        mCurrent *curPack = nullptr;
        #ifdef LinuxSide
        bool curMine = false;
        #endif
    };
}
#endif //CURRENTMOT_H
