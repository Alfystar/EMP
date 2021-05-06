/*
 * CircularBuffer.cpp
 *
 *  Created on: 07 nov 2019
 *      Author: alfyhack
 */
#include "CircularBuffer.h"

template <class T, u_int16_t nElem> CircularBuffer<T, nElem>::CircularBuffer() {
  this->head_ = 0;
  this->tail_ = 0;
  memClean();
}

template <class T, u_int16_t nElem> void CircularBuffer<T, nElem>::memClean() {
  for (u_int16_t i = 0; i < real_nElem; i++)
    this->buf_[i] = 0;
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

/* copy only len byte of the *item object (to optimize),
 * On success are return old head
 * Fail with -1 if the buffer are full
 * Fail with -2 if len is over the sizeOf(T)
 */
template <class T, u_int16_t nElem> u_int16_t CircularBuffer<T, nElem>::put(T *item, u_int16_t len) {
  if (isFull())
    return -1;
  if (len > sizeof(T))
    return -2;
  memcpy((void *)&buf_[head_], item, len);
  return headInc(); // old head
}

template <class T, u_int16_t nElem>
void CircularBuffer<T, nElem>::memcpyCb(T *memDestArray, u_int16_t localTail, u_int16_t len) {
  for (u_int16_t i = 0; i < len; i++) {
    memDestArray[i] = buf_[(localTail + i) % real_nElem];
  }
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

/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Get Head Structure information

template <class T, u_int16_t nElem> inline u_int16_t CircularBuffer<T, nElem>::getHead() { return head_; }
template <class T, u_int16_t nElem> inline T *CircularBuffer<T, nElem>::getHeadPtr() { return &buf_[head_]; }
template <class T, u_int16_t nElem> inline T CircularBuffer<T, nElem>::readHead() { return buf_[head_]; }

// Get Tail Structure information
template <class T, u_int16_t nElem> inline u_int16_t CircularBuffer<T, nElem>::getTail() { return tail_; }
template <class T, u_int16_t nElem> inline T *CircularBuffer<T, nElem>::getTailPtr() { return &buf_[tail_]; }
template <class T, u_int16_t nElem> inline T CircularBuffer<T, nElem>::readTail() { return this->buf_[tail_]; }

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
