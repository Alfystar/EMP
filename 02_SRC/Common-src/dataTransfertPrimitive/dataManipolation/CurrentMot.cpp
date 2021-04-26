//
// Created by alfyhack on 15/10/19.
//

#include "CurrentMot.h"

namespace DataManipolation {
    CurrentMot::CurrentMot (mCurrent &c){
        this->changePack(c);
    }

    CurrentMot::CurrentMot (mCurrent *c){
        this->changePack(c);
    }

    #ifdef LinuxSide

    CurrentMot::CurrentMot (){
        curPack = &dataFactory::makeMCurrent();
        curMine = true;
    }

    CurrentMot::~CurrentMot (){
        if (curMine)
            dataFactory::freeMCurrent(curPack);
    }

    #endif

    void CurrentMot::changePack (mCurrent &c){
        #ifdef LinuxSide
        if (curMine){
            curMine = false;
            dataFactory::freeMCurrent(curPack);
        }
        #endif
        this->curPack = &c;
    }

    void CurrentMot::changePack (mCurrent *c){
        #ifdef LinuxSide
        if (curMine){
            curMine = false;
            dataFactory::freeMCurrent(curPack);
        }
        #endif
        this->curPack = c;
    }

    void CurrentMot::copyCur (CurrentMot &c){
        this->copyCur(c.getCurrent());
    }

    void CurrentMot::copyCur (mCurrent *c){
        memcpy(this->curPack, c, sizeof(mCurrent));
    }

    void CurrentMot::copyCur (mCurrent &c){
        memcpy(this->curPack, &c, sizeof(mCurrent));
    }

    mCurrent &CurrentMot::getCurrent (){
        return *this->curPack;
    }

    short CurrentMot::getCurrent (motCode mot){
        return (*this->curPack)[mot];
    }

    void CurrentMot::curSet (motCode mot, short cur){
        (*this->curPack)[mot] = cur;
    }
#if defined LinuxSide || defined CURRENT_MOT_DB
    void CurrentMot::printCurrent (){
        this->printCurrent(this->getCurrent());
    }

    void CurrentMot::printCurrent (mCurrent *c){
        CurrentMot::printCurrent(*c);
    }

    void CurrentMot::printCurrent (mCurrent &c){
        #ifndef LinuxSide
        Db.flush();
        Db.println("Sended Current Value:");
        for (byte i = Mot1; i < nMot; i++) {
            Db.print("\tcur[Mot");
            Db.print(i+1);
            Db.print("]:");
            Db.println(c[i]);
        }
        #else
        printf("Curr:\t");
        for (unsigned char i = Mot1; i < nMot; i++){
            printf("%d)%hd\t", i + 1, c[i]);
        }
        printf("\n");
        #endif
    }
#endif
}
