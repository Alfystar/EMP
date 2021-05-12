//
// Created by alfyhack on 29/11/19.
//

#ifndef PIDCTRLDEMO_TIMESPECOP_H
#define PIDCTRLDEMO_TIMESPECOP_H

#include <time.h>

// Macro thinked for: struct timespec
///###################################################################################################
///################################# Macro for Time Operation ########################################
///###################################################################################################
#define timerissetSpec(tv) (tv.tv_sec || (tv.tv_nsec)

#define timerclearSpec(tv) (tv.tv_sec = (tv.tv_nsec = 0)

#define timercmpSpec(a, b, CMP)                                                                                        \
  ((a.tv_sec == (b.tv_sec) ? (a.tv_nsec CMP(b.tv_nsec) : (a.tv_sec CMP(b.tv_sec))

#define timeraddSpec(a, b, result)                                                                                     \
  do {                                                                                                                 \
    result.tv_sec = a.tv_sec + b.tv_sec;                                                                               \
    result.tv_nsec = a.tv_nsec + b.tv_nsec;                                                                            \
    if (result.tv_nsec >= 1000000000) {                                                                                \
      ++result.tv_sec;                                                                                                 \
      result.tv_nsec -= 1000000000;                                                                                    \
    }                                                                                                                  \
  } while (0)

#define timersubSpec(a, b, result)                                                                                     \
  do {                                                                                                                 \
    result.tv_sec = a.tv_sec - b.tv_sec;                                                                               \
    result.tv_nsec = a.tv_nsec - b.tv_nsec;                                                                            \
    if (result.tv_nsec < 0) {                                                                                          \
      --result.tv_sec;                                                                                                 \
      result.tv_nsec += 1000000000;                                                                                    \
    }                                                                                                                  \
  } while (0)

#define timeWriteSpec(ts, sec, nanoSec)                                                                                \
  do {                                                                                                                 \
    ts.tv_sec = (long)sec + (long)nanoSec / (1000UL * 1000UL * 1000UL);                                                \
    ts.tv_nsec = (long)nanoSec % (1000UL * 1000UL * 1000UL);                                                           \
  } while (0)

#define timeStampSpec(ts, name)                                                                                        \
  std::cout << name << ".tv_sec=" << ts.tv_sec << "\t" << name << ".tv_nsec=" << ts.tv_nsec << " ("                    \
            << (ts.tv_nsec + (500UL * 1000UL)) / (1000UL * 1000UL) << "ms)\n"
///###################################################################################################

#endif // PIDCTRLDEMO_TIMESPECOP_H
