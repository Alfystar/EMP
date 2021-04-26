//
// Created by alfyhack on 09/11/19.
//

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <UartDriver.h>

using namespace std;
using namespace Uart;
UartDriver *uart;

int main (__attribute__((unused)) int argc, __attribute__((unused)) char *argv[]){
    try{
        uart = new UartDriver("/dev/ttyACM0");
        uart->uartSpeed(B115200);
    }catch (std::exception &e){
        std::cerr << e.what() << "\n";
        exit(-1);
    }

    settingsBoard pData;
    SettingBoard_C *p = new SettingBoard_C(pData);

    //Setting per ora di default
    //curr neg => no limit
    p->setMotorLimit(Mot1, 2160, 24300, -19450);
    p->setMotorLimit(Mot2, 2960, 16200, -1000);
    p->setMotorLimit(Mot3, 2960, 21900, -7300);
    p->setMotorLimit(Mot4, 2160, 10000, -10000);
    p->setMotorLimit(Mot5, 2400, 10000, -10000);
    p->setMotorLimit(Mot6, 2160, 5770, -10);
    p->setAdcDiff(false);
    p->setAdcRef(in1V1);
    p->setPWMfreq(hz4k);
    p->printSetting();
    int i = 0;
    for (;;){
        // Leggo ultimo dato, se non è arrivato il thread viene messo in wait, fino all'arrivo
        std::cout << "Recived:\n";
        UartDriver::serialPackDb(*uart->getDataWait());
        std::cout << "\n";
        i++;

    }
    return 0;
}