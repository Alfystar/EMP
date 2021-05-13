#ifndef PACK_H
#define PACK_H
#include <stdint.h>

struct _packLinux2Ard {
  int16_t num;
  char buf[20];
} __attribute__((packed));
typedef struct _packLinux2Ard packLinux2Ard;

struct _packArd2Linux {
  int16_t num;
  char buf[10];
} __attribute__((packed));
typedef struct _packArd2Linux packArd2Linux;

#endif
