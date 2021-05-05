//
// Created by alfy on 04/05/21.
//

#ifndef EMP_LIB_INCLUDEPLATFORM_H
#define EMP_LIB_INCLUDEPLATFORM_H

#ifdef ARDUINO_H
//#include "Arduino.h"
#include <stdlib.h>
#include "circularBuffer/CircularBuffer.h"
#else   // Default code are for linux
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string>
#include <CircularBuffer.h>
#include <thread>
#include <semaphore.h>
#include <mutex>
#include "UartException.h"

#endif

#endif //EMP_LIB_INCLUDEPLATFORM_H
