//
// Created by alfyhack on 15/10/19.
//

#ifndef ALLSENSOR_H
#define ALLSENSOR_H

#include "EncoderMot.h"
#include "CurrentMot.h"
#include "../dataPrimitive.h"
#include "../DataFactory.h"

namespace DataManipolation {
    using namespace DataPrimitive;

    class AllSensor : public CurrentMot, public EncoderMot{
    public:
        AllSensor (mAll &all);
        AllSensor (mAll *all);
        #ifdef LinuxSide
        AllSensor ();   // creo al mio interno il Pack
        ~AllSensor ();
        #endif
        void changePack (mAll &all);
        void copyPack (mAll *all);
        void copyPack (mAll &all);
        void copyPack (AllSensor &all);
        mAll &getSens ();

#if defined LinuxSide || defined ALL_SENDOR_DB
        // <== Debug & Print ==>
        void printAll ();
        static void printAll (mAll *all);
        static void printAll (mAll &all);
#endif
    private:
        mAll *allPack;
        #ifdef LinuxSide
        bool allMine = false;
        #endif
    };
}
#endif //ALLSENSOR_H
