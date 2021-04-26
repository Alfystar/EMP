//
// Created by alfyhack on 15/10/19.
//

#include "AllSensor.h"

namespace DataManipolation {

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Waddress-of-packed-member"

    AllSensor::AllSensor (mAll &all) : DataManipolation::CurrentMot(&all.cur), DataManipolation::EncoderMot(&all.en){
        this->changePack(all);
    }

    AllSensor::AllSensor (mAll *all) : DataManipolation::CurrentMot(&all->cur), DataManipolation::EncoderMot(&all->en){
        this->changePack(*all);
    }

    #pragma GCC diagnostic pop

    #ifdef LinuxSide

    AllSensor::AllSensor () : AllSensor(&dataFactory::makeMAll()){
        allMine = true;
    }

    AllSensor::~AllSensor (){
        if (allMine)
            dataFactory::freeMAll(allPack);
    }

    #endif

    void AllSensor::changePack (mAll &all){
        #ifdef LinuxSide
        if (allMine){
            allMine = false;
            dataFactory::freeMAll(allPack);
        }
        #endif
        this->allPack = &all;
    }

    void AllSensor::copyPack (mAll *all){
        memcpy(this->allPack, all, sizeof(mAll));
    }

    void AllSensor::copyPack (mAll &all){
        memcpy(this->allPack, &all, sizeof(mAll));
    }

    void AllSensor::copyPack (AllSensor &all){
        this->copyPack(all.getSens());
    }

    mAll &AllSensor::getSens (){
        return *this->allPack;
    }

#if defined LinuxSide || defined ALL_SENDOR_DB
    void AllSensor::printAll (){
        this->printAll(this->getSens());
    }

    void AllSensor::printAll (mAll *all){
        AllSensor::printAll(*all);
    }

    void AllSensor::printAll (mAll &all){
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Waddress-of-packed-member"
        AllSensor::printEncoder(&all.en);
        AllSensor::printCurrent(&all.cur);
        #pragma GCC diagnostic pop
    }
#endif
}
