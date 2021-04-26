//
// Created by alfyhack on 15/10/19.
//

#ifndef DATAPRIMITIVE_H
#define DATAPRIMITIVE_H

#include <stdint-gcc.h>

namespace DataPrimitive {
    /// Enum Declaration for the comunication
    enum packDest : unsigned char{
        pack4Ard, pack4Rasp
    };
    enum specialPwmCode : unsigned short{
        freeRun = 300, hardStop = 350, softStop = 400, ignore = 450
    };
    enum motCode : unsigned char{
        Mot1 = 0, Mot2, Mot3, Mot4, Mot5, Mot6, nMot
    };
    enum packType : unsigned char{
        //invalid = 0, prima presente ma forse inutile
        PWMsend_EnRet = 1, PWMsend_CurRet, PWMsend_AllRet, SettingGet, SettingSet, goHome
    };
    //sampleTimeEn && sampleTimeCur tempi in micro secondi (unsigned short), se devono essere inviati sia En che Cur, si invia un mAll
    enum adcRef : unsigned char{
        in1V1 = 0, in2V56, ext
    };
    // set timer divisor to     1 for PWM frequency of 31372.55 Hz
    // set timer divisor to     8 for PWM frequency of  3921.16 Hz
    // set timer divisor to    64 for PWM frequency of   490.20 Hz
    // set timer divisor to   256 for PWM frequency of   122.55 Hz
    // set timer divisor to  1024 for PWM frequency of    30.64 Hz
    enum pwmFreq : unsigned char{
        hz30, hz120, hz490, hz4k, hz30k
    };
    enum uartCode : unsigned char{
        StartCode = 192, EndCode = 168
    };
    enum uartPackType : unsigned char{
        FIRST_EXLUDE,        //Per rendere parametrico il check del tipo, aggiungere pacchetti tra FIRST_EXLUDE<--> LAST_EXLUDE
        mSpeedData = 1,
        settingBoardData,
        mCurrentData,
        mAllData,
        mEncoderData,
        sampleTimeEn,       //micro second (10^-6)
        sampleTimeCur,      //micro second (10^-6)
        goHomeUart,
        settingAsk,
        RESEND,                // Never used for now
        LAST_EXLUDE         //Per rendere parametrico il check del tipo, aggiungere pacchetti tra FIRST_EXLUDE<--> LAST_EXLUDE
    };
    ///#################################################################
    ///Pack information structure

    typedef short mSpeed[nMot];
    typedef short mCurrent[nMot];
    typedef short mEncoder[nMot];
    typedef struct settingsBoard_{
        mEncoder maxEn;          //valore massimo di passi prima di considerarsi fuori range di sicurezza (numeri pos)
        mEncoder minEn;          //valore minimo di passi prima di considerarsi fuori range di sicurezza (numeri neg)
        mCurrent maxCurrMed;     //valore massimo di corrente Efficace (con una media di 1 ms ~ ultime 8 letture), numero * 8 per semplificare i conti)
        adcRef adcVref;
        bool diff;                  //true = offset delete, false = basicValue
        pwmFreq freq;
    } __attribute__((packed)) settingsBoard; //https://www.geeksforgeeks.org/how-to-avoid-structure-padding-in-c/

    typedef struct mAll_{
        mEncoder en;
        mCurrent cur;
    } __attribute__((packed)) mAll;
    /*Struttura di Invio dati all'arduino*/
    typedef union{
        union sendType{
            mSpeed speed;
            settingsBoard prop;
            uint32_t sampleEn;
            uint32_t sampleCur;
        } up;
        unsigned char buf[sizeof(sendType)];
    }  __attribute__((packed)) data2Ard;
    /*Struttura di Invio dati al Rapsberry*/
    typedef union{
        union ricType{
            mEncoder en;
            mCurrent cur;
            mAll sens;
            settingsBoard prop;
        } up;
        unsigned char buf[sizeof(ricType)];
    }  __attribute__((packed)) data2Linux;
    ///#################################################################
    ///Sendable uart pack
    typedef struct{
        uartPackType type;
        //short checkSum; Prima vedere se serve
        data2Ard pack;
    } __attribute__((packed)) uart2Ard;
    typedef struct{
        uartPackType type;
        //short checkSum; Prima vedere se serve
        data2Linux pack;
    }  __attribute__((packed)) uart2Linux;
}
#endif //DATAPRIMITIVE_H
