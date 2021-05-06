//
// Created by alfy on 06/05/21.
//

#ifndef EMP_LIB_02_SRC_EMPLIBS_MP_PIPE_H
#define EMP_LIB_02_SRC_EMPLIBS_MP_PIPE_H

#include <MP.h>

// Specific include for the class
#include <thread>
#include <semaphore.h>
#include <mutex>
#include <cerrno>
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

using namespace EMP;

template <typename pIn, typename pOut> class MP_Pipe : public MP<pIn, pOut> {
  int fdpipeReadSide, fdpipeWriteSide;
  std::thread *readerTh;      //Puntatore al Thread avviato

public:
  MP_Pipe(int fdReadSide, int fdWriteSide);
  ~MP_Pipe();

protected:
  virtual int packSend_Concrete(u_int8_t *stream, u_int16_t len);

private:
  static void readerPipe(MP_Pipe<pIn, pOut> &mpPipe);
};

#endif // EMP_LIB_02_SRC_EMPLIBS_MP_PIPE_H
