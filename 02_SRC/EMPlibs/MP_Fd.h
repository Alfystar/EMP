//
// Created by alfy on 06/05/21.
//

#ifndef EMP_LIB_02_SRC_EMPLIBS_MP_FD_H
#define EMP_LIB_02_SRC_EMPLIBS_MP_FD_H

#include <MP.h>

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

public:
  MP_Fd(int fdReadSide, int fdWriteSide);
  ~MP_Fd();

  virtual int16_t getData_wait(pIn *pack); // return the residual pack available after the remove
//  pIn *getDataWait () noexcept(false);
//  pIn *getDataWait_timePack (struct timespec *t) noexcept(false);
//  pIn *getDataWait (struct timespec *timeOut) noexcept(false);
//  pIn *getDataWait_timePack (struct timespec *timeOut, struct timespec *t) noexcept(false);
protected:
  virtual int packSend_Concrete(u_int8_t *stream, u_int16_t len);

private:
  static void readerFDTh(MP_Fd<pIn, pOut, conf> &mpPipe);
};

template <typename pIn, typename pOut, MPConf conf>
MP_Fd<pIn, pOut, conf>::MP_Fd(int fdReadSide, int fdWriteSide) : MP<pIn, pOut, conf>() {
  mpFd_db("[MP_Fd]: test Constuctor, fdRead %d, fdWrite %d\n", fdReadSide, fdWriteSide);
  fdR = fdReadSide;
  fdW = fdWriteSide;
  sem_init(&receivedPackToken, 0, 0);
  readerTh = new std::thread(this->readerFDTh, std::ref(*this));
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
  // readerTh->detach();
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
void MP_Fd<pIn, pOut, conf>::readerFDTh(MP_Fd<pIn, pOut, conf> &mpPipe) {
  mpFd_db("[MP_Fd::readerFDTh] ReaderPipe Thread start\n");
  usleep(50 * 1000U);
  long bRead;
  for (;;) {
    mpFd_db("[MP_Fd::readerFDTh] readerFDTh try read\n");
    bRead = read(mpPipe.fdR, mpPipe.byteRecive.getHeadPtr(), mpPipe.byteRecive.remaningSpaceLinear());
    if (bRead < 0) {
      mpFd_err(" [MP_Fd::readerFDTh] readerFDTh fd see: %ld; ", bRead);
      MP_FD_err perror("take error:");
      exit(-1);
    }
    mpFd_db("[MP_Fd::readerFDTh] readerFDTh read: %ld\n", bRead);
    mpPipe.byteRecive.headAdd(bRead);
    uint16_t packFind = mpPipe.byteParsing();
    for (; packFind > 0; packFind--)
      sem_post(&mpPipe.receivedPackToken);
  }
}

} // namespace EMP

#endif // EMP_LIB_02_SRC_EMPLIBS_MP_FD_H
