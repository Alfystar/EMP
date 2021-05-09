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
#define MP_PIPE_VervoseLevel 2
#define MP_PIPE_err _codeActive(1, MP_PIPE_VervoseLevel)
#define MP_PIPE_notice _codeActive(2, MP_PIPE_VervoseLevel)
#define MP_PIPE_info _codeActive(3, MP_PIPE_VervoseLevel)
#define MP_PIPE_Db _codeActive(4, MP_PIPE_VervoseLevel)

#define mpPipe_err(str, ...) printf_STD(MP_PIPE_err, str, ##__VA_ARGS__)
#define mpPipe_notice(str, ...) printf_STD(MP_PIPE_notice, str, ##__VA_ARGS__)
#define mpPipe_info(str, ...) printf_STD(MP_PIPE_info, str, ##__VA_ARGS__)
#define mpPipe_db(str, ...) printf_STD(MP_PIPE_Db, str, ##__VA_ARGS__)

namespace EMP {

template <typename pIn, typename pOut, MPConf conf> class MP_Fd : public MP<pIn, pOut, conf> {
  int fdpipeReadSide, fdpipeWriteSide;
  std::thread *readerTh; // Puntatore al Thread avviato

public:
  MP_Fd(int fdReadSide, int fdWriteSide);
  ~MP_Fd();

protected:
  virtual int packSend_Concrete(u_int8_t *stream, u_int16_t len);

private:
  static void readerPipe(MP_Fd<pIn, pOut, conf> &mpPipe);
};

template <typename pIn, typename pOut, MPConf conf>
MP_Fd<pIn, pOut, conf>::MP_Fd(int fdReadSide, int fdWriteSide) : MP<pIn, pOut, conf>() {
  mpPipe_db("[MP_Fd]: test Constuctor, fdRead %d, fdWrite %d\n", fdReadSide, fdWriteSide);
  fdpipeReadSide = fdReadSide;
  fdpipeWriteSide = fdWriteSide;
  readerTh = new std::thread(this->readerPipe, std::ref(*this));
}

template <typename pIn, typename pOut, MPConf conf> MP_Fd<pIn, pOut, conf>::~MP_Fd() {
  mpPipe_db("[MP_Fd]test Destructor\n");
  // readerTh->detach();
}

template <typename pIn, typename pOut, MPConf conf>
int MP_Fd<pIn, pOut, conf>::packSend_Concrete(u_int8_t *stream, u_int16_t len) {
  size_t i = 0;
  size_t bWrite;
  while (len > 0) {
    bWrite = write(fdpipeWriteSide, &stream[i], len);
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
void MP_Fd<pIn, pOut, conf>::readerPipe(MP_Fd<pIn, pOut, conf> &mpPipe) {
  mpPipe_db("[MP_Fd::readerPipe] ReaderPipe Thread start\n");
  usleep(50 * 1000U);
  long bRead;
  for (;;) {
    mpPipe_db("[MP_Fd::readerPipe] readerPipe try read\n");
    bRead = read(mpPipe.fdpipeReadSide, mpPipe.byteRecive.getHeadPtr(), mpPipe.byteRecive.remaningSpaceLinear());
    if (bRead < 0) {
      mpPipe_err(" [MP_Fd::readerPipe] readerPipe fd see: %ld; ", bRead);
      MP_PIPE_err perror("take error:");
      exit(-1);
    }
    mpPipe_db("[MP_Fd::readerPipe] readerPipe read: %ld\n", bRead);
    mpPipe.byteRecive.headAdd(bRead);
    mpPipe.byteParsing();
  }
}

} // namespace EMP

#endif // EMP_LIB_02_SRC_EMPLIBS_MP_FD_H
