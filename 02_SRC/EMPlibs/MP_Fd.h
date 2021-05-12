//
// Created by alfy on 06/05/21.
//

#ifndef EMP_LIB_02_SRC_EMPLIBS_MP_FD_H
#define EMP_LIB_02_SRC_EMPLIBS_MP_FD_H

#include <MP.h>
#include <timeSpecOp.h>
// Specific include for the class
#include <cerrno>
#include <mutex>
#include <semaphore.h>
#include <thread>
#include <unistd.h>

#define printf_STD(Level, str, ...)                                                                                    \
  do {                                                                                                                 \
    Level printf("[" #Level "]: " str, ##__VA_ARGS__);                                                                 \
  } while (0)

// Level 0 := No Message
// Level 1 := Error msg
// Level 2 := Notice msg
// Level 3 := Info msg
// Level 4 := DB msg
#define MP_FD_VervoseLevel 5
#define MP_FD_err _codeActive(1, MP_FD_VervoseLevel)
#define MP_FD_notice _codeActive(2, MP_FD_VervoseLevel)
#define MP_FD_info _codeActive(3, MP_FD_VervoseLevel)
#define MP_FD_Db _codeActive(4, MP_FD_VervoseLevel)

#define mpFd_err(str, ...) printf_STD(MP_FD_err, str, ##__VA_ARGS__)
#define mpFd_notice(str, ...) printf_STD(MP_FD_notice, str, ##__VA_ARGS__)
#define mpFd_info(str, ...) printf_STD(MP_FD_info, str, ##__VA_ARGS__)
#define mpFd_db(str, ...) printf_STD(MP_FD_Db, str, ##__VA_ARGS__)

namespace EMP {
/* This class, extend the MP to the linux OS, it perform the operation at the File-Descriptor abstraction level,
 * big part of the device have their pseudo-file in /dev/, so this solution should be very general.
 *
 * N.B. This class aren't reentrant !!! Outside only 1 Thread can talk with this, because every getData_try or
 * getData_wait the circular buffer progress and different thread whatch different output. If is important multiply the
 * output, create another layer or use same data-distribution service (dds)
 */
template <typename pIn, typename pOut, MPConf conf> class MP_Fd : public MP<pIn, pOut, conf> {
  int fdR, fdW;
  std::thread *readerTh; // Reader Thread, started by the constructor
  sem_t receivedPackToken;
  struct timespec lastPackTime;

public:
  MP_Fd(int fdReadSide, int fdWriteSide);
  ~MP_Fd();

  int16_t getData_wait(pIn *pack); // return the residual pack available after the remove

protected:
  int packSend_Concrete(u_int8_t *stream, u_int16_t len) override;
  __always_inline void packTimeRefresh() override;

public:
  unsigned long lastPackElapsed() override;

private:
  static void readerFDTh(MP_Fd<pIn, pOut, conf> &mpFd);
};

template <typename pIn, typename pOut, MPConf conf>
MP_Fd<pIn, pOut, conf>::MP_Fd(int fdReadSide, int fdWriteSide) : MP<pIn, pOut, conf>() {
  mpFd_db("[MP_Fd]: test Constuctor, fdRead %d, fdWrite %d\n", fdReadSide, fdWriteSide);
  fdR = fdReadSide;
  fdW = fdWriteSide;
  packTimeRefresh();
  sem_init(&receivedPackToken, 0, 0);
  readerTh = new std::thread(this->readerFDTh, std::ref(*this));
  if (conf.RT_THREAD) {
    // encrease priority
    sched_param sch{};
    int policy;
    pthread_getschedparam(this->readerTh->native_handle(), &policy, &sch);
    sch.sched_priority = sched_get_priority_max(SCHED_FIFO);
    if (pthread_setschedparam(this->readerTh->native_handle(), SCHED_FIFO, &sch)) {
      // todo: Gestire la comunicazione dell'errore
      //            throw UartException("Failed to setschedparam: ", errno);
    }
  }
}

template <typename pIn, typename pOut, MPConf conf> MP_Fd<pIn, pOut, conf>::~MP_Fd() {
  mpFd_db("[MP_Fd]test Destructor\n");
  mpFd_db("Getting handler readerTh ...\n");
  pthread_t tid = readerTh->native_handle();
  mpFd_db("Cancellation request readerTh ...\n");
  pthread_cancel(tid);
  mpFd_db("Waiting the end of readerTh ...\n");
  readerTh->join();
  mpFd_db("Deleting semaphore of receivedPackToken ...\n");
  sem_destroy(&receivedPackToken);
}

template <typename pIn, typename pOut, MPConf conf> int16_t MP_Fd<pIn, pOut, conf>::getData_wait(pIn *pack) {
  sem_wait(&receivedPackToken);
  return this->getData_try(pack);
}

template <typename pIn, typename pOut, MPConf conf>
int MP_Fd<pIn, pOut, conf>::packSend_Concrete(u_int8_t *stream, u_int16_t len) {
  size_t i = 0;
  size_t bWrite;
  while (len > 0) {
    bWrite = write(fdW, &stream[i], len);
    if (bWrite < 0) {
      perror("[MP_Fd::packSend_Concrete]Reading take error:");
      return -1;
    }
    i += bWrite;
    len -= bWrite;
  }
  return 0;
}
template <typename pIn, typename pOut, MPConf conf>
void MP_Fd<pIn, pOut, conf>::readerFDTh(MP_Fd<pIn, pOut, conf> &mpFd) {
  mpFd_db("[MP_Fd::readerFDTh] ReaderPipe Thread start\n");
  usleep(50 * 1000U);
  long bRead;
  for (;;) {
    mpFd_db("[MP_Fd::readerFDTh] readerFDTh try read\n");
    bRead = read(mpFd.fdR, mpFd.byteRecive.getHeadPtr(), mpFd.byteRecive.remaningSpaceLinear());
    if (bRead < 0) {
      mpFd_err(" [MP_Fd::readerFDTh] readerFDTh fd see: %ld; ", bRead);
      MP_FD_err perror("take error:");
      exit(-1);
    }
    mpFd_db("[MP_Fd::readerFDTh] readerFDTh read: %ld\n", bRead);
    if (mpFd.byteRecive.headAdd(bRead) == errorRet)
      mpFd_err("[MP_Fd::readerFDTh] byteRecive circular buffer end space!!!");
    uint16_t packFind = mpFd.byteParsing();
    for (; packFind > 0; packFind--)
      sem_post(&mpFd.receivedPackToken);
  }
}
template <typename pIn, typename pOut, MPConf conf> void MP_Fd<pIn, pOut, conf>::packTimeRefresh() {
  clock_gettime(CLOCK_MONOTONIC_RAW, &lastPackTime);
}
template <typename pIn, typename pOut, MPConf conf> unsigned long MP_Fd<pIn, pOut, conf>::lastPackElapsed() {
  struct timespec now, res;
  clock_gettime(CLOCK_MONOTONIC_RAW, &now);
  timersubSpec(now, lastPackTime, res);
  return res.tv_sec * 1000000UL + res.tv_nsec / 1000UL;
}

} // namespace EMP

#endif // EMP_LIB_02_SRC_EMPLIBS_MP_FD_H
