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


void *son(void *) {
  std::cout << "Son Start" << std::endl;
  pack dataIncoming;

  auto sonSide =
      new EMP::MP_Fd<pack, pack, LinuxMP_ConfMed(crc8En)>(p1to2[readEndPipe], p2to1[writeEndPipe], false);

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

void *dad(void *) {
  sleep(1);
  std::cout << "dad Start" << std::endl;

  auto dadSide =
      new EMP::MP_Fd<pack, pack, LinuxMP_ConfMed(crc8En)>(p2to1[readEndPipe], p1to2[writeEndPipe],false);

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
