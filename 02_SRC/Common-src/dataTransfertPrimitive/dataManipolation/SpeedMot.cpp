//
// Created by alfyhack on 15/10/19.
//

#include "SpeedMot.h"

namespace DataManipolation {
    SpeedMot::SpeedMot (mSpeed &s){
        this->changePack(s);
    }

    SpeedMot::SpeedMot (mSpeed *s){
        this->changePack(*s);
    }

    #ifdef LinuxSide

    SpeedMot::SpeedMot (){
        speedPack = &dataFactory::makeMSpeed();
        speedMine = true;
    }

    SpeedMot::~SpeedMot (){
        if (speedMine)
            dataFactory::freeMSpeed(speedPack);
    }

    #endif

    void SpeedMot::changePack (mSpeed &speed){
        #ifdef LinuxSide
        if (speedMine){
            speedMine = false;
            dataFactory::freeMEncoder(speedPack);
        }
        #endif
        this->speedPack = &speed;
    }

    void SpeedMot::copyPack (SpeedMot &speed){
        this->copyPack(speed.getPwm());
    }

    void SpeedMot::copyPack (SpeedMot *speed){
        this->copyPack(speed->getPwm());
    }

    void SpeedMot::copyPack (mSpeed &speed){
        memcpy(this->speedPack, &speed, sizeof(mSpeed));
    }

    mSpeed &SpeedMot::getPwm (){
        return *(this->speedPack);
        //return *this->speedPack;
    }

    short SpeedMot::getPwm (motCode mot){
        return (*this->speedPack)[mot];
    }

    void SpeedMot::pwmSet (mSpeed &pwms){
        this->pwmSet(pwms[Mot1], pwms[Mot2], pwms[Mot3], pwms[Mot4], pwms[Mot5], pwms[Mot6]);
    }
    void SpeedMot::pwmSet (mSpeed *pwms){
        this->pwmSet(*pwms);
    }

    void SpeedMot::pwmSet (short m1, short m2, short m3, short m4, short m5, short m6){
        this->pwmSet(Mot1, m1);
        this->pwmSet(Mot2, m2);
        this->pwmSet(Mot3, m3);
        this->pwmSet(Mot4, m4);
        this->pwmSet(Mot5, m5);
        this->pwmSet(Mot6, m6);
    }

    void SpeedMot::pwmSet (motCode mot, short mVal){
        mSpeed &s = this->getPwm();
        if (mVal == freeRun || mVal == hardStop || mVal == softStop || mVal == ignore || abs(mVal) <= 255)
            s[mot] = mVal;
        else
            s[mot] = ignore;
    }

    void SpeedMot::softAllBreak (){
        pwmSet(softStop, softStop, softStop, softStop, softStop, softStop);
    }

#if defined LinuxSide || defined SPEED_MOT_DB
    void SpeedMot::printSpeed (){
        this->printSpeed(this->getPwm());
    }

    void SpeedMot::printSpeed (mSpeed *speed){
        SpeedMot::printSpeed(*speed);
    }

    void SpeedMot::printSpeed (mSpeed &speed){
        #ifndef LinuxSide
        Db.flush();
        Db.println("Receving Speed Settings:");
        for (byte i = Mot1; i < nMot; i++) {
            Db.print("\tspeed[Mot");
            Db.print(i+1);
            Db.print("]:");
            Db.println(speed[i]);
        }
        #else
        printf("Speed:\t");
        for (unsigned char i = Mot1; i < nMot; i++){
            printf("%d)%hd\t", i + 1, (speed[i]));
        }
        printf("\n");
        #endif
    }
#endif
}

