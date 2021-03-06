//
// Created by alfy on 06/05/21.
//

#ifndef EMP_LIB_02_SRC_EMPLIBS_MP_FD_H
#define EMP_LIB_02_SRC_EMPLIBS_MP_FD_H
#ifdef CMAKE_COMPILING

// Exeption include
#include <cstring>
#include <exception>
#include <string>
using namespace std;
// System include
#include <MP.h>
#include <timeSpecOp.h>

// Specific include for the class
#include <cerrno>
#include <mutex>
#include <semaphore.h>
#include <thread>
#include <unistd.h>

/*
#if __cplusplus <= 201703L // superiore al C++17
#error Please use C++20, your version is too old
#endif
*/

/*
#define templatePar()                                                                                                  \
  template <typename pIn, typename pOut, bool RT_THREAD, bool CRC8_enable, uint16_t cdBinStore, uint16_t cbPackStore>
#define templateParCall() pIn, pOut, RT_THREAD, CRC8_enable, cdBinStore, cbPackStore
#define RT_THREAD_enable() RT_THREAD
*/

#define printf_STD(Level, str, ...)                                                                                    \
  do {                                                                                                                 \
    Level printf("[" #Level "]: " str, ##__VA_ARGS__);                                                                 \
  } while (0)

// Level 0 := No Message
// Level 1 := Error msg
// Level 2 := Notice msg
// Level 3 := Info msg
// Level 4 := DB msg
#define MP_FD_VervoseLevel 2
#define MP_FD_err _codeActive(1, MP_FD_VervoseLevel)
#define MP_FD_notice _codeActive(2, MP_FD_VervoseLevel)
#define MP_FD_info _codeActive(3, MP_FD_VervoseLevel)
#define MP_FD_Db _codeActive(4, MP_FD_VervoseLevel)

#define mpFd_err(str, ...) printf_STD(MP_FD_err, str, ##__VA_ARGS__)
#define mpFd_notice(str, ...) printf_STD(MP_FD_notice, str, ##__VA_ARGS__)
#define mpFd_info(str, ...) printf_STD(MP_FD_info, str, ##__VA_ARGS__)
#define mpFd_db(str, ...) printf_STD(MP_FD_Db, str, ##__VA_ARGS__)

namespace EMP {

/*
 * The exeption are used to signal a terminal error, not catch him, the system will crash in any case, but so are
 * possible watch on the terminal the error, if you need to send back an error, the error code in the return value
 * might be better solution for this library
 */
class MP_FDexept : public exception {
  const string _msg;
  const int _errCode = 0;
  string ret;

public:
  MP_FDexept(const string &msg, int errCode) : _msg(msg), _errCode(errCode) {
    ret = "|" + _msg + "|";
    ret += "\terror code: ";
    ret += to_string(_errCode);
    char buf[4096];
    ret += "|\t|errno description:";
    ret += strerror_r(_errCode, buf, sizeof(buf));
  };

  MP_FDexept(const string &msg) : MP_FDexept(msg, 0){};

  virtual const char *what() const noexcept override { return ret.c_str(); }
  virtual int errNum() const noexcept { return _errCode; }
}; // class MP_exept

/* This class, extend the MP to the linux OS, it perform the operation at the File-Descriptor abstraction level,
 * big part of the device have their pseudo-file in /dev/, so this solution should be very general.
 *
 * N.B. This class aren't reentrant !!! Outside only 1 Thread can talk with this, because every getData_try or
 * getData_wait the circular buffer progress and different thread whatch different output. If is important multiply the
 * output, create another layer or use same data-distribution service (dds)
 *
 * Concrete son have to readyReader.unlock(), to signal the finish of the constructor
 */

templatePar() class MP_Fd : public MP<templateParCall()> {
public:
  typedef typename MP<templateParCall()>::callBacksMP callBacksMP;

protected:
  int fdR, fdW;
  std::mutex readyReader; // The concrete Constructor of the class HAVE TO unlock to start the reader thread properly

private:
  std::thread *readerTh; // Reader Thread, started by the constructor
  sem_t receivedPackToken;

  callBacksMP userCallback;

  struct timespec lastDecodeTime;

public:
  MP_Fd(int fdReadSide, int fdWriteSide, bool RT_THREAD);
  MP_Fd(int fdReadSide, int fdWriteSide, bool RT_THREAD, callBacksMP clback);

protected:
  MP_Fd(bool RT_THREAD);
  MP_Fd(bool RT_THREAD, callBacksMP clback);

public:
  ~MP_Fd();

  int16_t getData_try(pIn *pack);  // return the residual pack available after the remove
  int16_t getData_wait(pIn *pack); // return the residual pack available after the remove
  int16_t getData_wait(pIn *pack,
                       u_long ns); // return the residual pack available after the remove, of fail if timeOut reach

protected:
  int packSend_Concrete(u_int8_t *stream, u_int16_t len) override;
  __always_inline void packTimeRefresh() override;

public:
  unsigned long lastPackElapsed() override;

private:
  static void readerFDTh(MP_Fd<templateParCall()> &mpFd);
  static void packReciveAdd(MP<templateParCall()> *mpFd);
};

templatePar() MP_Fd<templateParCall()>::MP_Fd(int fdReadSide, int fdWriteSide, bool RT_THREAD)
    : MP_Fd<templateParCall()>(fdReadSide, fdWriteSide, RT_THREAD, callBacksMP()) {}

templatePar() MP_Fd<templateParCall()>::MP_Fd(int fdReadSide, int fdWriteSide, bool RT_THREAD, callBacksMP clback)
    : MP_Fd<templateParCall()>(RT_THREAD, clback) {
  mpFd_db("[MP_Fd]: Creating MP_Fd object with, fdRead %d, fdWrite %d\n", fdReadSide, fdWriteSide);
  fdR = fdReadSide;
  fdW = fdWriteSide;
  readyReader.unlock();
}

templatePar() MP_Fd<templateParCall()>::MP_Fd(bool RT_THREAD) : MP_Fd<templateParCall()>(RT_THREAD, callBacksMP()) {}

templatePar() MP_Fd<templateParCall()>::MP_Fd(bool RT_THREAD, callBacksMP clback) : MP<templateParCall()>(clback) {
  // User callback override:
  userCallback = clback;
  this->clback.pkDetect = packReciveAdd;

  packTimeRefresh();
  sem_init(&receivedPackToken, 0, 0);
  readyReader.try_lock();
  readerTh = new std::thread(this->readerFDTh, std::ref(*this));
  if (RT_THREAD) {
    // encrease priority
    sched_param sch{};
    int policy;
    pthread_getschedparam(this->readerTh->native_handle(), &policy, &sch);
    sch.sched_priority = sched_get_priority_max(SCHED_FIFO);
    if (pthread_setschedparam(this->readerTh->native_handle(), SCHED_FIFO, &sch)) {
      throw MP_FDexept("Failed to pthread_setschedparam", errno);
    }
  }
}

templatePar() MP_Fd<templateParCall()>::~MP_Fd() {
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

templatePar() int16_t MP_Fd<templateParCall()>::getData_try(pIn *pack) {
retry:
  if (sem_trywait(&receivedPackToken) == -1) {
    switch (errno) {
    case EAGAIN:
      return -1;
    case EINTR:
      goto retry;
    default:
      throw MP_FDexept("[getData_try] sem_trywait FAIL", errno);
    }
  }
  return MP<templateParCall()>::getData_try(pack);
}
templatePar() int16_t MP_Fd<templateParCall()>::getData_wait(pIn *pack) {
retry:
  if (sem_wait(&receivedPackToken) == -1) {
    switch (errno) {
    case EINTR:
      goto retry;
    default:
      throw MP_FDexept("[getData_try] getData_wait FAIL", errno);
    }
  }
  return MP<templateParCall()>::getData_try(pack);
}

templatePar() int16_t MP_Fd<templateParCall()>::getData_wait(pIn *pack, u_long ns) {
  timespec now, add, timeOut;
  clock_gettime(CLOCK_REALTIME, &now);
  timeSpecSet(add, 0, ns);
  timeSpecAdd(timeOut, now, add);

retry:
  if (sem_timedwait(&receivedPackToken, &timeOut) == -1) {
    switch (errno) {
    case EINTR:
      goto retry;
    case ETIMEDOUT:
      return -2;
    default:
      throw MP_FDexept("[getData_try] getData_wait FAIL", errno);
    }
  }
  return MP<templateParCall()>::getData_try(pack);
}

templatePar() int MP_Fd<templateParCall()>::packSend_Concrete(u_int8_t *stream, u_int16_t len) {
  size_t i = 0;
  long bWrite;
  while (len > 0) {
    bWrite = write(fdW, &stream[i], len);
    if (bWrite < 0) {
      throw MP_FDexept("[MP_Fd::packSend_Concrete]Reading take error", errno);
    }
    i += bWrite;
    len -= bWrite;
  }
  return 0;
}
templatePar() void MP_Fd<templateParCall()>::readerFDTh(MP_Fd<templateParCall()> &mpFd) {
  mpFd.readyReader.lock();
  usleep(500 * 1000U); // Grace time to end the configuration
  mpFd_db("[MP_Fd::readerFDTh] ReaderPipe Thread start\n");
  long bRead;
  for (;;) {
    mpFd_db("[MP_Fd::readerFDTh] readerFDTh try read\n");
  retry:
    bRead = read(mpFd.fdR, mpFd.byteRecive.getHeadPtr(), mpFd.byteRecive.availableSpaceLinear());
    if (bRead < 0) {
      switch (errno) {
      case EINTR:
        goto retry;
      default:
        MP_FD_err {
          mpFd_err("[MP_Fd::readerFDTh] readerFDTh fd see: %ld; ", bRead);
          perror("take error:");
        }
        break;
      }
      sleep(1);
      continue;
    }
    mpFd_db("[MP_Fd::readerFDTh] readerFDTh read: %ld\n", bRead);
    if (mpFd.byteRecive.headAdd(bRead) == errorRet)
      mpFd_err("[MP_Fd::readerFDTh] byteRecive circular buffer end space!!!");
    mpFd.byteParsing();
  }
}

templatePar() void MP_Fd<templateParCall()>::packReciveAdd(MP<templateParCall()> *mp) {
  mpFd_db("[MP_Fd::packReciveAdd] callBack inside class start\n");
  MP_Fd<templateParCall()> *mpFd = (MP_Fd<templateParCall()> *)(mp); // is for sure my-self, see constructor
  sem_post(&mpFd->receivedPackToken);
  if (mpFd->userCallback.pkDetect)
    mpFd->userCallback.pkDetect(mp);
}

templatePar() void MP_Fd<templateParCall()>::packTimeRefresh() { clock_gettime(CLOCK_MONOTONIC_RAW, &lastDecodeTime); }

templatePar() unsigned long MP_Fd<templateParCall()>::lastPackElapsed() {
  struct timespec now, res;
  clock_gettime(CLOCK_MONOTONIC_RAW, &now);
  timeSpecSub(now, lastDecodeTime, res);
  return res.tv_sec * 1000000UL + res.tv_nsec / 1000UL;
}

} // namespace EMP

#endif // CMAKE_COMPILING
#endif // EMP_LIB_02_SRC_EMPLIBS_MP_FD_H
