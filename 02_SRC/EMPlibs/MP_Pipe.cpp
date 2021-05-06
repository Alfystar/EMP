//
// Created by alfy on 06/05/21.
//

#include "MP_Pipe.h"

using namespace EMP;
template <typename pIn, typename pOut> MP_Pipe<pIn, pOut>::MP_Pipe(int fdReadSide, int fdWriteSide) {
  mpPipe_db("test Constuctor");
  readerTh = new std::thread(this->readerPipe, std::ref(*this));

}

template <typename pIn, typename pOut> MP_Pipe<pIn, pOut>::~MP_Pipe() {
  mpPipe_db("test Destructor");
  readerTh->detach();
}

template <typename pIn, typename pOut> int MP_Pipe<pIn, pOut>::packSend_Concrete(u_int8_t *stream, u_int16_t len) {
  size_t i = 0;
  size_t bWrite;
  while (len > 0) {
    bWrite = write(this->fd, &stream[i], len);
    if (bWrite < 0) {
      perror("Reading take error:");
      return -1;
    }
    i += bWrite;
    len -= bWrite;
  }
  return 0;
}
template <typename pIn, typename pOut> void MP_Pipe<pIn, pOut>::readerPipe(MP_Pipe<pIn, pOut> &mpPipe) {
  mpPipe_db("readerPipe Thread start\n");
  usleep(50 * 1000U);
  long bRead;
  for (;;) {
    mpPipe_db("readerPipe try read\n");
    bRead = read(mpPipe.fdpipeReadSide, mpPipe.byteRecive.getHeadPtr(), mpPipe.byteRecive.remaningSpaceLinear());
    if (bRead < 0) {
      mpPipe_err("uartReader fd see: %d\n",bRead);
      MP_PIPE_err perror("\tuartReader take error:");
      exit(-1);
    }
    mpPipe_db("uartReader read: %d\n",bRead);
    mpPipe.byteRecive.headAdd(bRead);
    mpPipe.byteParsing();
  }
}
