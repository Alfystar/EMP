//
// Created by alfy on 04/05/21.
//

#ifndef EMP_LIB_UARTDATATRANSFERT_H
#define EMP_LIB_UARTDATATRANSFERT_H
#include "conf.h"
#include "includePlatform.h"
#include "macro.h"

namespace EMP {
// To perform a general data transfert, the class need to know the unions of the
// possible data Is responsibility of the caller undestand what pack are
// recived, this library think only about the transfert of the block
//  pIn := One or more than one type of pack (if multiple use union with a tag field at start can help) can be send
//  pIn := One or more than one type of pack (if multiple use union with a tag field at start can help) can be recive
template <typename pIn, typename pOut> class UARTdataTransfert {
  // Variabili della coda di ricezione dati
  int lastZeroIndex = -1;

  CircularBuffer<unsigned char, binaryBufElement * sizeof(pIn)> byteRecive; // space to save byte read before parsing
  CircularBuffer<pIn, cbSize> cbRecive;                                     // Succesfull pack recive

public:
  // Instance operation
  UARTdataTransfert();
  ~UARTdataTransfert();
  void bufClear();

  /// Data Send & Get
  virtual void packSend(pOut *pack, short size) = 0; // Il pacchetto potrebbe avere una dimensione minore della massima
  // Data
  void newByteRecive(unsigned char byte);
  unsigned short Available();
  bool getData(pIn *pack);
};

/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Instance operation

template <typename pIn, typename pOut> UARTdataTransfert<pIn, pOut>::UARTdataTransfert() { bufClear(); }

template <typename pIn, typename pOut> UARTdataTransfert<pIn, pOut>::~UARTdataTransfert() {}

template <typename pIn, typename pOut> void UARTdataTransfert<pIn, pOut>::bufClear() {
  byteRecive.memClean();
  cbRecive.memClean();
}
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/// Data Send & Get

template <typename pIn, typename pOut> void UARTdataTransfert<pIn, pOut>::newByteRecive(unsigned char byte) {}

template <typename pIn, typename pOut> unsigned short UARTdataTransfert<pIn, pOut>::Available() {
  return this->cbRecive->size();
}

// copy pack Logic,
// if possible, return true and in *pack are copied the data
// otherwise false are return and *pack aren't touch
template <typename pIn, typename pOut> bool UARTdataTransfert<pIn, pOut>::getData(pIn *pack) {
  if (!cbRecive->empty()){
    *pack = cbRecive.getHead();
    return true;
  }
  return false;
}

} // namespace EMP

#endif // EMP_LIB_UARTDATATRANSFERT_H
