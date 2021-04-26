//
// Created by alfyhack on 15/10/19.
//

#ifndef ENCODERMOT_H
#define ENCODERMOT_H

#include <string.h>
#include <stdio.h>
#include "../dataPrimitive.h"
#include "../DataFactory.h"

#ifndef LinuxSide
#include "Arduino.h"
// Essendo linkata nel ".project" di sloeber, gcc la vede nella sua vera posizione
#include "../ScoreBoard_Uart_Firmware/CustomCodeDefine.h"
#endif

namespace DataManipolation {
    using namespace DataPrimitive;

    class EncoderMot{
    public:
        EncoderMot (mEncoder &en);
        EncoderMot (mEncoder *en);
        #ifdef LinuxSide
        EncoderMot ();   // creo al mio interno Pack
        ~EncoderMot ();
        #endif
        void changePack (mEncoder &en);
        void changePack (mEncoder *en);
        void copyEn (EncoderMot &en);
        void copyEn (mEncoder *en);
        void copyEn (mEncoder &en);
        mEncoder &getEn ();
        short getEn (motCode mot);
        void enSet (motCode mot, short en);
        void enAdd (EncoderMot &add);
        void enSub (EncoderMot &sub);
#if defined LinuxSide || defined ENCODER_MOT_DB
        void printEncoder ();
        static void printEncoder (mEncoder *en);
        static void printEncoder (mEncoder &en);
#endif
    private:
        mEncoder *enPack = nullptr;
        #ifdef LinuxSide
        bool enMine = false;
        #endif
    };
}
#endif //ENCODERMOT_H
