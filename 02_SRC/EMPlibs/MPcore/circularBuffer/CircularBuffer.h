/*
 * CircularBuffer.h
 *
 *  Created on: 07 nov 2019
 *      Author: alfyhack
 */

#ifndef PROJECT_LIB_CIRCULARBUFFER_CIRCULARBUFFER_H_
#define PROJECT_LIB_CIRCULARBUFFER_CIRCULARBUFFER_H_

#include "opDef.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>

#define real_nElem (nElem + 1) // +1 are the ONE SLOT OPEN

/* In this implementation of the Circular Buffer, to perform the operation correctly, we use the
 * "ONE SLOT OPEN" TECHNICS inside the class, outside the buffer are watch like full buffer.
 * "ONE SLOT OPEN" TECHNICS use the element before the tail as end, and so it are always unreached,
 * but data consistency are safe.
 *
 * ATTENTION: This implementation not override data if the buffer are FULL
 */
template <class T, u_int16_t nElem> class CircularBuffer {
private:
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
  u_int16_t put(T *item, u_int16_t bSize); // copy only bSize byte of the *item object (to optimize)

  // Gets metod
  T get();
  T get(u_int16_t *indexRet); // save in *indexRet the index of the returned data
  T *getPtr();
  T *getPtr(u_int16_t *indexRet); // save in *indexRet the index of the returned data
  // Special read, to take out char buf in specific structure
  // save in *memDestArray  the value starting from localTail --> localTail+len (obviously in circular buffer logic)
  void memcpyCb(T *memDestArray, u_int16_t localTail, u_int16_t len);

  //  Get Head Structure information and contenent without change notting
  u_int16_t getHead() const;
  T *getHeadPtr(); // Return the memory area where are the current head
  T readHead() const;

  // Get Tail Structure information
  u_int16_t getTail() const;
  T *getTailPtr(); // Return the memory area where are the current head
  T readTail() const;

public:
  // Structure operation
  // Return the real usable slot information (ONE SLOT OPEN is hiden outside)
  bool isEmpty() const; // true if isEmpty
  bool isFull() const;  // true if full (ONE SLOT OPEN logic hiden inside)
  u_int16_t capacity() const;

  u_int16_t countSlotBetween(u_int16_t localTail, u_int16_t localHead) const;
  u_int16_t usedSpace() const;
  u_int16_t availableSpace() const;
  u_int16_t remaningSpaceLinear() const;

  // Operation to progress the head,
  // On SUCCESS: return the head before increase,
  // On Fail: return "errorRet" if with the increase go over the limit, and nothing are do
  u_int16_t headInc();
  u_int16_t headAdd(u_int16_t len);

  // Operation to progress the tail, return tail after increase, "errorRet" if tail jump over the head
  u_int16_t tailInc();
  u_int16_t tailAdd(u_int16_t len);
};

template <class T, u_int16_t nElem> CircularBuffer<T, nElem>::CircularBuffer() {
  this->head_ = 0;
  this->tail_ = 0;
  memClean();
}

template <class T, u_int16_t nElem> void CircularBuffer<T, nElem>::memClean() {
  for (u_int16_t i = 0; i < real_nElem; i++)
    memset(&this->buf_[i], 0, sizeof(T));
  reset();
}

template <class T, u_int16_t nElem> inline void CircularBuffer<T, nElem>::reset() { head_ = tail_; }
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Puts metod
template <class T, u_int16_t nElem> u_int16_t CircularBuffer<T, nElem>::put(T item) {
  if (isFull())
    return -1;
  buf_[head_] = item;
  return headInc(); // old head
}

template <class T, u_int16_t nElem> u_int16_t CircularBuffer<T, nElem>::put(T *item) {
  return put(item, sizeof(T)); // old head of -1 if full
}

/* copy only bSize byte of the *item object (to optimize),
 * On success are return old head
 * Fail with -1 if the buffer are full
 * Fail with -2 if bSize is over the sizeOf(T)
 */
template <class T, u_int16_t nElem> u_int16_t CircularBuffer<T, nElem>::put(T *item, u_int16_t bSize) {
  if (isFull())
    return -1;
  if (bSize > sizeof(T))
    return -2;
  memcpy((void *)&buf_[head_], item, bSize);
  return headInc(); // old head
}

/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Gets metod

template <class T, u_int16_t nElem> T CircularBuffer<T, nElem>::get() {
  if (isEmpty()) {
    return T();
  }
  // Read data and advance the tail (we now have a free space)
  auto val = readTail();
  this->tailInc();
  return val;
}

template <class T, u_int16_t nElem> T CircularBuffer<T, nElem>::get(u_int16_t *indexRet) {
  *indexRet = getTail();
  return get();
}

template <class T, u_int16_t nElem> T *CircularBuffer<T, nElem>::getPtr() {
  if (isEmpty()) {
    return nullptr;
  }
  // Read data and advance the tail (we now have a free space)
  auto val = getTailPtr();
  this->tailInc();
  return val;
}

template <class T, u_int16_t nElem> T *CircularBuffer<T, nElem>::getPtr(u_int16_t *indexRet) {
  *indexRet = getTail();
  return getPtr();
}

template <class T, u_int16_t nElem>
void CircularBuffer<T, nElem>::memcpyCb(T *memDestArray, u_int16_t localTail, u_int16_t len) {
  for (u_int16_t i = 0; i < len; i++) {
    memDestArray[i] = buf_[(localTail + i) % real_nElem];
  }
}
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Get Head Structure information

template <class T, u_int16_t nElem> inline u_int16_t CircularBuffer<T, nElem>::getHead() const { return head_; }
template <class T, u_int16_t nElem> inline T *CircularBuffer<T, nElem>::getHeadPtr() { return &buf_[head_]; }
template <class T, u_int16_t nElem> inline T CircularBuffer<T, nElem>::readHead() const { return buf_[head_]; }

// Get Tail Structure information
template <class T, u_int16_t nElem> inline u_int16_t CircularBuffer<T, nElem>::getTail() const { return tail_; }
template <class T, u_int16_t nElem> inline T *CircularBuffer<T, nElem>::getTailPtr() { return &buf_[tail_]; }
template <class T, u_int16_t nElem> inline T CircularBuffer<T, nElem>::readTail() const { return this->buf_[tail_]; }

/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/// Structure operation

template <class T, u_int16_t nElem> inline bool CircularBuffer<T, nElem>::isEmpty() const { return head_ == tail_; }

template <class T, u_int16_t nElem> inline bool CircularBuffer<T, nElem>::isFull() const {
  return head_ == modSub(tail_, 1, real_nElem);
}

template <class T, u_int16_t nElem> inline u_int16_t CircularBuffer<T, nElem>::capacity() const { return nElem; }

/* This function return how many block are inside the space pointed by localTail e localHead
 * Local Tail := first unread slot
 * Local Head := first "Free slot" (slot not neaded)
 *       #############  "F"
 * | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 |
 *       ^               ^
 *   Local Tail       localHead
 *   countSlotBetween = 4
 */
template <class T, u_int16_t nElem>
inline u_int16_t CircularBuffer<T, nElem>::countSlotBetween(u_int16_t localTail, u_int16_t localHead) const {
  return modSub(localHead, localTail, real_nElem); // One Free Slot Logic
}

template <class T, u_int16_t nElem> inline u_int16_t CircularBuffer<T, nElem>::usedSpace() const {
  // This 2 block are only to speed-up, formula always function
  if (isFull())
    return nElem;
  if (isEmpty())
    return 0;

  return modSub(head_, tail_, real_nElem); // One Free Slot Logic
}

template <class T, u_int16_t nElem> u_int16_t CircularBuffer<T, nElem>::availableSpace() const {
  return nElem - usedSpace();
}

// return space between Head and last VALID array index position
// VALID is respect tail and respect one free slot logic
template <class T, u_int16_t nElem> inline u_int16_t CircularBuffer<T, nElem>::remaningSpaceLinear() const {
  if (head_ < tail_) // the end of the buffer are reach before the end of array
    return availableSpace();
  if (tail_ == 0)         // One Slot Free are at the-end of the array
    return nElem - head_; // is like the last slot are ouside the buffer

  return real_nElem - head_;
}

/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Operation to progress the head, return the head before increase
template <class T, u_int16_t nElem> inline u_int16_t CircularBuffer<T, nElem>::headInc() {
  return headAdd(1); // old head
}

template <class T, u_int16_t nElem> inline u_int16_t CircularBuffer<T, nElem>::headAdd(u_int16_t len) {
  if (availableSpace() < len)
    return -1;
  u_int16_t oldHead = head_;
  head_ = (head_ + len) % real_nElem;
  return oldHead; // old head
}

template <class T, u_int16_t nElem> inline u_int16_t CircularBuffer<T, nElem>::tailInc() { return tailAdd(1); }

template <class T, u_int16_t nElem> inline u_int16_t CircularBuffer<T, nElem>::tailAdd(u_int16_t len) {
  if (usedSpace() < len)
    return -1;
  tail_ = (tail_ + len) % real_nElem;
  return tail_;
}

#endif /* PROJECT_LIB_CIRCULARBUFFER_CIRCULARBUFFER_H_ */
