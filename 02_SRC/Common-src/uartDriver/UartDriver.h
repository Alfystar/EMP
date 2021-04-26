//
// Created by alfyhack on 10/11/19.
//

#ifndef PCLISTENUART_UARTDRIVER_H
#define PCLISTENUART_UARTDRIVER_H

//#define UartDriverDebug 1
//#define UartDriverDebug_thread 1
//#define CMD_Send_PRINT 1

#ifdef LinuxSide
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string>
#include "UartException.h"
#include <CircularBuffer.h>
#include <thread>
#include <semaphore.h>
#include <mutex>
#include <dataTransfert_AllInclude.h>

#else
//#include "Arduino.h"
#include <stdlib.h>
#include "../ScoreBoard_Uart_Firmware/CustomCodeDefine.h"
#include "../dataTransfertPrimitive/dataTransfert_AllInclude.h"
#include "../circularBuffer/CircularBuffer.h"
#endif

#define sizeofArray(x)  sizeof(x)/sizeof(x[0])

#ifdef LinuxSide
#define cbSize 256
#define dataSize 4096
#else
#define cbSize 8
#define dataSize 4*sizeof(uart2Ard)
#endif

namespace Uart {
    using namespace DataPrimitive;
    using namespace DataManipolation;
    #ifdef LinuxSide
    typedef data2Ard dOut;
    typedef uart2Ard pOut;
    typedef data2Linux dIn;
    typedef uart2Linux pIn;
    #else
    typedef data2Ard dIn;
    typedef uart2Ard pIn;
    typedef data2Linux dOut;
    typedef uart2Linux pOut;

    #endif
    enum uartState : char{
        waitStart, waitType, waitEnd
    };

    //todo: Su linux, disattivare il reset della scheda automatico, e aggiungere un comando che esplicitamente resetta l'arduino
    class UartDriver{
    public:
        #ifdef LinuxSide

        UartDriver (const std::string &device) noexcept(false);
        ~UartDriver ();  //should be called only by singleton Parametric factory
        void uartSpeed (speed_t vel) noexcept(false);
        void bufClear ();

        #else
        UartDriver(HardwareSerial *serial, long vel);
        #endif

        /// Data Send
        // &pack è l'indirizzo da dove il sender si va a copiare i dati
        void packSend (uartPackType type, dOut *pack);

        #ifndef LinuxSide
        void serialIsr();
        void serialTrySend();
        #endif

        /// Data get
        size_t Available (); // su ard uartAvailable
        pIn *getData ();

        #ifdef LinuxSide

        pIn *getDataWait () noexcept(false);
        pIn *getDataWait_timePack (struct timespec *t) noexcept(false);
        pIn *getDataWait (struct timespec *timeOut) noexcept(false);
        pIn *getDataWait_timePack (struct timespec *timeOut, struct timespec *t) noexcept(false);

        #endif

        /// Data print for debug
#if defined LinuxSide || defined SERIAL_PACK_DB
        static void serialPackDb (uart2Ard &p);
        static void serialPackDb (uart2Linux &p);
#endif
    private:
        //Variabili della porta Seriale
        #ifdef LinuxSide
        int fd;                     //file descriptor della com
        struct termios config;      //Struttura di controllo della uart
        std::thread *readerTh;      //Puntatore al Thread avviato
        #else
        HardwareSerial *com;
        #endif
        //Variabili della coda di ricezione dati
        unsigned char reciveBuf[dataSize];   //4096
        CircularBuffer <unsigned char> *cbByteRecive;
        uartState stateUart = waitStart;
        size_t potPackStart;
        size_t potPackType;
        size_t expettedEnd;
        unsigned char dato;
        size_t datoId;


        //Variabili della coda di pacchetti riconosciuti

        pIn cbReciveBuf[cbSize];
        CircularBuffer <pIn> *cbRecive;
        #ifdef LinuxSide
        //std::mutex readCb_mutex;  // protects gets data from concurrency
        sem_t recivedPackSem;
        struct timespec tPack[cbSize];
        #endif

        //Variabili della coda di invio
        #ifdef LinuxSide
        //Probabilmente non servono poichè oltre la write c'e un buffer del S.O.
        std::mutex writeUart_mutex;  // protects packSend from concurrency
        #else
        pOut cbSendPackBuf[cbSize];
        CircularBuffer<pOut> *cbSend;
        #endif

        //Reader Thread
        #ifdef LinuxSide
        static void uartReader (UartDriver &d);
        #endif

        //State machine to undestand pack
        void dataDiscover ();
        bool typeCheck (int p);
        size_t sizeMessage (uartPackType t);
    };
}

#undef dataSize
#endif //PCLISTENUART_UARTDRIVER_H
