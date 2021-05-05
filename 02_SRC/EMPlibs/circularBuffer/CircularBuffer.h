/*
 * CircularBuffer.h
 *
 *  Created on: 07 nov 2019
 *      Author: alfyhack
 */

#ifndef PROJECT_LIB_CIRCULARBUFFER_CIRCULARBUFFER_H_
#define PROJECT_LIB_CIRCULARBUFFER_CIRCULARBUFFER_H_

#include <cstdint>
#include <cstdlib>
#include <iostream>

template <class T, unsigned short nElem> class CircularBuffer {
private:
  T buf_[nElem];
  unsigned short head_;
  unsigned short tail_;

  const unsigned short max_size_;
  bool full_ = false;

public:
  CircularBuffer();
  void memClean();

  // Puts metod, it copy the item inside the buffer and return the old head
  u_int16_t put(T item);
  u_int16_t put(T *item);
  u_int16_t put(T *item, unsigned short len); // copy only len byte of the *item object (to optimize)

  // Operation to progress the head, return the head before increase
  u_int16_t put_externalWrite();              // add 1 to head
  u_int16_t put_externalWrite(u_int16_t len); // add len to head

  // Gets metod
  T get();
  T get(u_int16_t *indexRet);        // save in *indexRet the index of the retured data
  T *getPtr();
  T *getPtr(u_int16_t *indexRet);    // save in *indexRet the index of the retured data
  unsigned short get_externalRead(); // return new tail

  // Special write, to take out char buf in specific structure
  // salvano in una mem i valori partendo da localTail --> localTail+len
  void writeMemOut(T *mem, u_int16_t localTail, u_int16_t len);

  // Get Head Structure information
  u_int16_t getHead();
  T *getHeadPtr(); // Return the memory area where are the current head
  T readHead();

  // Get Tail Structure information
  u_int16_t getTail();
  T *getTailPtr(); // Return the memory area where are the current head
  T readTail();

  // Structure operation
  void reset();
  bool empty() const; // true if empty
  bool full() const;  // true if full
  u_int16_t capacity() const;
  u_int16_t size() const;
  u_int16_t linearEnd() const; // return space between last array index (ignore tail position)
};

#endif /* PROJECT_LIB_CIRCULARBUFFER_CIRCULARBUFFER_H_ */
