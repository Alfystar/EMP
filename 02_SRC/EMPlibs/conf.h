//
// Created by alfy on 04/05/21.
//

#ifndef EMP_LIB_CONF_H
#define EMP_LIB_CONF_H

/*
 * cbSize           := Capacity (in term of pIn) of the Circular Buffer use to store the correctly handle pIn
 * binaryBufElement := Capacity (in term of pIn) of the Circular Buffer use to store the receved byte (in transfert byte)
 */

#ifdef ARDUINO_H
#define cbSize 8
#define binaryBufElement 4
//#elif //to add Other architetture
#else// Default ENV are inside Linux
#define cbSize 256
#define binaryBufElement 32
#endif

#endif //EMP_LIB_CONF_H
