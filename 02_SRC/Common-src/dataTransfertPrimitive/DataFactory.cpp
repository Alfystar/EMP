//
// Created by alfyhack on 15/10/19.
//


#include "DataFactory.h"

namespace dataFactory {

    mSpeed &makeMSpeed (){
        mSpeed *p = (mSpeed *) malloc(sizeof(mSpeed));
        memset(p, 0, sizeof(mSpeed));
        return *p;
    }

    void freeMSpeed (mSpeed *p){
        free(p);
    }

    settingsBoard &makeSettingBoard (){
        settingsBoard *p = (settingsBoard *) malloc(sizeof(settingsBoard));
        memset(p, 0, sizeof(settingsBoard));
        return *p;
    }

    void freeSettingBoard (settingsBoard *p){
        free(p);
    }

    mCurrent &makeMCurrent (){
        mCurrent *p = (mCurrent *) malloc(sizeof(mCurrent));
        memset(p, 0, sizeof(mCurrent));
        return *p;
    }

    void freeMCurrent (mCurrent *p){
        free(p);
    }

    mEncoder &makeMEncoder (){
        mEncoder *p = (mEncoder *) malloc(sizeof(mEncoder));
        memset(p, 0, sizeof(mEncoder));
        return *p;
    }

    void freeMEncoder (mEncoder *p){
        free(p);
    }

    mAll &makeMAll (){
        mAll *p = (mAll *) malloc(sizeof(mAll));
        memset(p, 0, sizeof(mAll));
        return *p;
    }

    void freeMAll (mAll *p){
        free(p);
    }

    uart2Ard &makeData2Ard (){
        uart2Ard *p = (uart2Ard *) malloc(sizeof(uart2Ard));
        memset(p, 0, sizeof(uart2Ard));
        return *p;
    }

    void freeData2Ard (uart2Ard *p){
        free(p);
    }

    uart2Linux &makeData2Rasp (){
        uart2Linux *p = (uart2Linux *) malloc(sizeof(uart2Linux));
        memset(p, 0, sizeof(uart2Linux));
        return *p;
    }

    void freeData2Rasp (uart2Linux *p){
        free(p);
    }

    uart2Ard &makeUart2Ard (){
        uart2Ard *p = (uart2Ard *) malloc(sizeof(uart2Ard));
        memset(p, 0, sizeof(uart2Ard));
        return *p;
    }

    void freeUart2Ard (uart2Ard *p){
        free(p);
    }

    uart2Linux &makeUart2Rasp (){
        uart2Linux *p = (uart2Linux *) malloc(sizeof(uart2Linux));
        memset(p, 0, sizeof(uart2Linux));
        return *p;
    }

    void freeUart2Rasp (uart2Linux *p){
        free(p);
    }
}