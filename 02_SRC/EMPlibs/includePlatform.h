//
// Created by alfy on 04/05/21.
//

#ifndef EMP_LIB_INCLUDEPLATFORM_H
#define EMP_LIB_INCLUDEPLATFORM_H

#ifdef ARDUINO_H
//#include "Arduino.h"
#include "circularBuffer/CircularBuffer.h"
#include <stdlib.h>
#else // Default code are for linux
#include "UartException.h"
#include <CircularBuffer.h>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <mutex>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <termios.h>
#include <thread>
#include <unistd.h>

#endif

#endif // EMP_LIB_INCLUDEPLATFORM_H
