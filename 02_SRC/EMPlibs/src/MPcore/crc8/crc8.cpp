/*
    CRC-8 routines

        Adapted from code found at:

        http://www.dattalo.com/technical/software/pic/crc_8bit.c

    From Maxim/Dallas AP Note 27

    "Understanding and Using Cyclic Redundancy Checks with
    Dallas Semiconductor iButton Products"

    The Ap note describes the CRC-8 algorithm used in the
    iButton products. Their implementation involves a 256 byte
    CRC table. This algorithm is implemented here. In addition
    two other algorithms are shown. One uses nibble arrays and
    the other uses boolean arithmetic.

    18 January 03   - T. Scott Dattalo
        26 July 10	    - David Cannings
 */

#include "crc8.h"


// Lets external code initialise the CRC variable, required if
// callers use crc8_byte

#ifdef CRC_MATH
// A basic implementation of CRC8 using only arithmetic

uint8_t crc_math(uint8_t data, uint8_t crc) {
  uint8_t i = (data ^ crc);

  crc = 0;

  if (i & 1)
    crc ^= 0x5e;
  if (i & 2)
    crc ^= 0xbc;
  if (i & 4)
    crc ^= 0x61;
  if (i & 8)
    crc ^= 0xc2;
  if (i & 0x10)
    crc ^= 0x9d;
  if (i & 0x20)
    crc ^= 0x23;
  if (i & 0x40)
    crc ^= 0x46;
  if (i & 0x80)
    crc ^= 0x8c;

  return crc;
}
#endif

#ifdef CRC_TABLE

uint8_t crc_table(uint8_t data, uint8_t crc) {
  int i = (data ^ crc);

  crc = crc_array[(data ^ crc) & 0xff];

  return crc;
}
#endif

#ifdef CRC_NIBBLES

uint8_t crc_nibbles(uint8_t data, uint8_t crc) {
  uint8_t i = (data ^ crc);

  crc = r1[i & 0xf] ^ r2[i >> 4];

  return crc;
}
#endif

/* Calculate the CRC for an individual byte using the method you have been chosen inside "crc8_inc.h"
 * data   :=    New byte use to calculate the crc
 * crc    :=    Previus CRC
 * return :=    CRC for the [oldCRC|data]
 */
uint8_t crc8_byte(uint8_t data, uint8_t crc) {
#ifdef CRC_TABLE
  crc = crc_table(data, crc);
#endif

#ifdef CRC_NIBBLES
  crc = crc_nibbles(data, crc);
#endif

#ifdef CRC_MATH
  crc = crc_math(data, crc);
#endif

  return crc;
}

uint8_t crc8_stream(uint8_t *data, uint16_t length) {
  uint8_t crc = 0xff;
  for (uint16_t i = 0; i < length; i++) {
    crc = crc8_byte(data[i], crc);
  }
  return crc;
}

