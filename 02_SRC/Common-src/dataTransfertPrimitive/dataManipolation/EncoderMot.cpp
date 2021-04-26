//
// Created by alfyhack on 15/10/19.
//

#include "EncoderMot.h"

namespace DataManipolation {
    EncoderMot::EncoderMot (mEncoder &en){
        this->changePack(en);
    }

    EncoderMot::EncoderMot (mEncoder *en){
        this->changePack(en);
    }

    #ifdef LinuxSide

    EncoderMot::EncoderMot (){
        enPack = &dataFactory::makeMEncoder();
        enMine = true;
    }

    EncoderMot::~EncoderMot (){
        if (enMine)
            dataFactory::freeMEncoder(enPack);
    }

    #endif

    void EncoderMot::changePack (mEncoder &en){
        #ifdef LinuxSide
        if (enMine){
            enMine = false;
            dataFactory::freeMEncoder(enPack);
        }
        #endif
        this->enPack = &en;
    }

    void EncoderMot::changePack (mEncoder *en){
        #ifdef LinuxSide
        if (enMine){
            enMine = false;
            dataFactory::freeMEncoder(enPack);
        }
        #endif
        this->enPack = en;
    }

    void EncoderMot::copyEn (EncoderMot &en){
        this->copyEn(en.getEn());
    }

    void EncoderMot::copyEn (mEncoder *en){
        memcpy(this->enPack, en, sizeof(mEncoder));
    }

    void EncoderMot::copyEn (mEncoder &en){
        memcpy(this->enPack, &en, sizeof(mEncoder));
    }

    mEncoder &EncoderMot::getEn (){
        return *this->enPack;
    }

    short EncoderMot::getEn (motCode mot){
        return (*this->enPack)[mot];
    }

    void EncoderMot::enSet (motCode mot, short en){
        (*this->enPack)[mot] = en;
    }

    void EncoderMot::enAdd (EncoderMot &add){
        for (unsigned char i = Mot1; i < nMot; i++){
            this->enSet((motCode) i, this->getEn((motCode) i) + add.getEn((motCode) i));
        }
    }

    void EncoderMot::enSub (EncoderMot &sub){
        for (unsigned char i = Mot1; i < nMot; i++){
            this->enSet((motCode) i, this->getEn((motCode) i) - sub.getEn((motCode) i));
        }
    }


    #if defined LinuxSide || defined ENCODER_MOT_DB
    void EncoderMot::printEncoder (){
        this->printEncoder(this->getEn());
    }

    void EncoderMot::printEncoder (mEncoder *en){
        EncoderMot::printEncoder(*en);
    }

    void EncoderMot::printEncoder (mEncoder &en){
        #ifndef LinuxSide
        Db.flush();
        Db.println("Sended Encoder Step:");
        for (byte i = Mot1; i < nMot; i++) {
            Db.print("\ten[Mot");
            Db.print(i+1);
            Db.print("]:");
            Db.println(en[i]);
        }
        #else
        printf("En:\t");
        for (unsigned char i = Mot1; i < nMot; i++){
            printf("%d)%hd\t", i + 1, en[i]);
        }
        printf("\n");
        #endif
    }
#endif
}
