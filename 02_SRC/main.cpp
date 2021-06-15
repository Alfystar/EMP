//
// Created by alfy on 04/05/21.
//
#include <MP_Fd.h>
#include <iostream>
#include <unistd.h>

typedef struct pack_ {
  char buf[20];
} __attribute__((packed)) pack;

#define crc8En true

#define readEndPipe 0
#define writeEndPipe 1

// 1 is the Dad
// 2 is the Son
int p1to2[2]; // Father to son
int p2to1[2]; // Son to father

// Son Parameter

// The signature of the function HAVE TO BE consistent with the template parameter of the caller class
// NB in general case dadCallBack have different Config parameter so the 2 call-back aren't invertible always!!!
void sonCallBack(EMP::MP<pack, pack, LinuxMP_ConfMed(crc8En)> *instance) {
  // there is no garancy for the calling-order with 2 different thread, this function will call form the
  // MP_Fd readerThread, and the SO could delayed this call any-time he want.
  std::cout << "\tSon callback data available read: " << instance->dataAvailable() << std::endl;
}

void *son(void *) {
  std::cout << "Son Start" << std::endl;
  pack dataIncoming;

  // Call Back vector definition
  EMP::MP_Fd<pack, pack, LinuxMP_ConfMed(crc8En)>::callBacksMP sonClback;
  sonClback.pkDetect = sonCallBack;

  auto sonSide =
      new EMP::MP_Fd<pack, pack, LinuxMP_ConfMed(crc8En)>(p1to2[readEndPipe], p2to1[writeEndPipe], false, sonClback);

  /// Son first pack test
  std::cout << "Son wait First pack" << std::endl;
  sonSide->getData_wait(&dataIncoming);
  std::cout << "[SON]Test1: [message size] {" << dataIncoming.buf << "}" << std::endl;

  /// Son second pack test
  sonSide->getData_wait(&dataIncoming);
  std::cout << "[SON]Test2: [pack size] {" << dataIncoming.buf << "}" << std::endl;

  /// Son send test
  char text[] = "Son recive";
  pack data;
  memcpy(data.buf, text, sizeof(text));
  sonSide->packSend(&data, sizeof(text));
  std::cout << "Son END" << std::endl;
  delete sonSide;
  return nullptr;
}

// The signature of the function HAVE TO BE consistent with the template parameter of the caller class
// NB in general case sonCallBack have different Config parameter so the 2 call-back aren't invertible always!!!
void dadCallBack(EMP::MP<pack, pack, LinuxMP_ConfMed(crc8En)> *instance) {
  std::cout << "\tDAD callback data available read: " << instance->dataAvailable() << std::endl;
}

void *dad(void *) {
  sleep(1);
  std::cout << "dad Start" << std::endl;

  EMP::MP_Fd<pack, pack, LinuxMP_ConfMed(crc8En)>::callBacksMP dadClback;
  dadClback.pkDetect = dadCallBack;

  auto dadSide =
      new EMP::MP_Fd<pack, pack, LinuxMP_ConfMed(crc8En)>(p2to1[readEndPipe], p1to2[writeEndPipe],false, dadClback);

  char text[] = "Dad Talk";
  pack data;
  memcpy(data.buf, text, sizeof(text));

  /// Dad first Test
  dadSide->packSend(&data, sizeof(text));

  /// Dad second Test
  dadSide->packSend(&data, sizeof(data));
  pack dataIncoming;

  /// Dad reading test
  dadSide->getData_wait(&dataIncoming);
  std::cout << "[DAD]Test1: {" << dataIncoming.buf << "}" << std::endl;

  std::cout << "DAD END" << std::endl;
  delete dadSide;
  return nullptr;
}

int main(int argc, char *argv[]) {

  if (pipe(p1to2) == -1) {
    perror("p1to2");
    exit(EXIT_FAILURE);
  }

  if (pipe(p2to1) == -1) {
    perror("p2to1");
    exit(EXIT_FAILURE);
  }

  pthread_t pid1, pid2;
  pthread_create(&pid1, nullptr, son, nullptr);
  pthread_create(&pid2, nullptr, dad, nullptr);
  while (true)
    pause();
  return 0;
}
