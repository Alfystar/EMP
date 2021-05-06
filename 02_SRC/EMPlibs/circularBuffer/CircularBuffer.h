/*
 * CircularBuffer.h
 *
 *  Created on: 07 nov 2019
 *      Author: alfyhack
 */

#ifndef PROJECT_LIB_CIRCULARBUFFER_CIRCULARBUFFER_H_
#define PROJECT_LIB_CIRCULARBUFFER_CIRCULARBUFFER_H_

#include "modularOperation.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>

#define real_nElem (nElem + 1) // +1 are the ONE SLOT OPEN

template <class T, u_int16_t nElem> class CircularBuffer {
private:
  /* In this implementation fo the Circular Buffer, to perform the operation correctly, we use the
   * "ONE SLOT OPEN" TECHNICS inside the class, outside the buffer are watch like full buffer.
   * "ONE SLOT OPEN" TECHNICS use the element before the tail as end, and so it are always unreached,
   * but data consistency are safe.
   *
   * ATTENTION: This implementation not override data if the buffer are FULL
   */
  T buf_[real_nElem];
  u_int16_t head_; // Contain next "free" cell
  u_int16_t tail_; // Contain last unread cell, exept if (head == tail) in this case buffer are emtpy

public:
  CircularBuffer();
  void memClean();
  void reset();

  // Puts metod, it copy the item inside the buffer and return the old head, return -1 if full
  u_int16_t put(T item);
  u_int16_t put(T *item);
  u_int16_t put(T *item, u_int16_t len); // copy only len byte of the *item object (to optimize)

  // Special write, to take out char buf in specific structure
  // save in *memDestArray  the value starting from localTail --> localTail+len (obviously in circular buffer logic)
  void memcpyCb(T *memDestArray, u_int16_t localTail, u_int16_t len);

  // Gets metod
  T get();
  T get(u_int16_t *indexRet); // save in *indexRet the index of the returned data
  T *getPtr();
  T *getPtr(u_int16_t *indexRet); // save in *indexRet the index of the returned data

public:
  // Get Head Structure information
  u_int16_t getHead();
  T *getHeadPtr(); // Return the memory area where are the current head
  T readHead();

  // Get Tail Structure information
  u_int16_t getTail();
  T *getTailPtr(); // Return the memory area where are the current head
  T readTail();

  // Structure operation
  // Return the real usable slot information (ONE SLOT OPEN is hiden outside)
  bool isEmpty() const; // true if isEmpty
  bool isFull() const;  // true if full (ONE SLOT OPEN logic hiden inside)
  u_int16_t capacity() const;

  u_int16_t countSlotBetween(u_int16_t localHead, u_int16_t localTail) const;
  u_int16_t usedSpace() const;
  u_int16_t availableSpace() const;
  u_int16_t remaningSpaceLinear() const;

private:
  // Operation to progress the head,
  // On SUCCESS: return the head before increase,
  // On Fail: return -1 if with the increase go over the limit, and nothing are do
  u_int16_t headInc();
  u_int16_t headAdd(u_int16_t len);

  // Operation to progress the tail, return tail after increase, -1 if tail jump over the head
  u_int16_t tailInc();
  u_int16_t tailAdd(u_int16_t len);
};

#endif /* PROJECT_LIB_CIRCULARBUFFER_CIRCULARBUFFER_H_ */
