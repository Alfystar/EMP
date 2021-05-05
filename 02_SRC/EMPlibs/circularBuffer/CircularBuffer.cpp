/*
 * CircularBuffer.cpp
 *
 *  Created on: 07 nov 2019
 *      Author: alfyhack
 */
#include "CircularBuffer.h"


template <class T, unsigned short nElem> CircularBuffer<T, nElem>::CircularBuffer() : max_size_(nElem) {
  this->head_ = 0;
  this->tail_ = 0;
  memClean();
}

template <class T, unsigned short nElem> void CircularBuffer<T, nElem>::memClean() {
  for (u_int16_t i = 0; i < max_size_; i++)
    this->buf_[i] = 0;
  reset();
}

/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Puts metod
template <class T, unsigned short nElem> u_int16_t CircularBuffer<T, nElem>::put(T item) {
  buf_[head_] = item;
  return this->put_externalWrite(); // old head
}

template <class T, unsigned short nElem> u_int16_t CircularBuffer<T, nElem>::put(T *item) {
  memcpy((void *)&buf_[head_], &item, sizeof(T));
  return this->put_externalWrite(); // old head
}

template <class T, unsigned short nElem> u_int16_t CircularBuffer<T, nElem>::put(T *item, unsigned short len) {
  memcpy((void *)&buf_[head_], &item, len);
  return this->put_externalWrite(); // old head
}

// Operation to progress the head, return the head before increase
template <class T, unsigned short nElem> inline u_int16_t CircularBuffer<T, nElem>::put_externalWrite() {
  return put_externalWrite(1); // old head
}

template <class T, unsigned short nElem> inline u_int16_t CircularBuffer<T, nElem>::put_externalWrite(u_int16_t len) {
  int oldHead = this->head_;
  if (full_)
    tail_ = (tail_ + len) % max_size_;
  head_ = (head_ + len) % max_size_;
  full_ = (head_ == tail_);
  return oldHead; // old head
}

/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Gets metod

template <class T, unsigned short nElem> T CircularBuffer<T, nElem>::get() {
  if (empty()) {
    return T();
  }
  // Read data and advance the tail (we now have a free space)
  auto val = readTail();
  this->get_externalRead();
  return val;
}

template <class T, unsigned short nElem> T CircularBuffer<T, nElem>::get(u_int16_t *indexRet) {
  *indexRet = getTail();
  return get();
}

template <class T, unsigned short nElem> T *CircularBuffer<T, nElem>::getPtr() {
  if (empty()) {
    return nullptr;
  }
  // Read data and advance the tail (we now have a free space)
  auto val = getTailPtr();
  this->get_externalRead();
  return val;
}

template <class T, unsigned short nElem> T *CircularBuffer<T, nElem>::getPtr(u_int16_t *indexRet) {
  *indexRet = getTail();
  return getPtr();
}


template <class T, unsigned short nElem> inline u_int16_t CircularBuffer<T, nElem>::get_externalRead() {
  full_ = (head_ == tail_);
  tail_ = (tail_ + 1) % max_size_;
  return tail_;
}

template <class T, unsigned short nElem>
void CircularBuffer<T, nElem>::writeMemOut(T *mem, u_int16_t localTail, u_int16_t len) {
  for (u_int16_t i = 0; i < len; i++) {
    mem[i] = buf_[(localTail + i) % max_size_];
  }
}
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Get Head Structure information

template <class T, unsigned short nElem> inline u_int16_t CircularBuffer<T, nElem>::getHead() { return head_; }
template <class T, unsigned short nElem> inline T *CircularBuffer<T, nElem>::getHeadPtr() { return &buf_[head_]; }
template <class T, unsigned short nElem> inline T CircularBuffer<T, nElem>::readHead() { return buf_[head_]; }

// Get Tail Structure information
template <class T, unsigned short nElem> inline u_int16_t CircularBuffer<T, nElem>::getTail() { return tail_; }
template <class T, unsigned short nElem> inline T *CircularBuffer<T, nElem>::getTailPtr() { return &buf_[tail_]; }
template <class T, unsigned short nElem> inline T CircularBuffer<T, nElem>::readTail() { return this->buf_[tail_]; }

/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/// Structure operation

template <class T, unsigned short nElem> inline void CircularBuffer<T, nElem>::reset() {
  this->head_ = this->tail_;
  this->full_ = false;
}

template <class T, unsigned short nElem> inline bool CircularBuffer<T, nElem>::empty() const {
  // if head and tail are equal, we are empty
  return (!full_ && (head_ == tail_)); // true if empty
}

template <class T, unsigned short nElem> inline bool CircularBuffer<T, nElem>::full() const {
  // If tail is ahead the head by 1, we are full
  return full_;
}

template <class T, unsigned short nElem> inline u_int16_t CircularBuffer<T, nElem>::capacity() const {
  return max_size_;
}

template <class T, unsigned short nElem> inline u_int16_t CircularBuffer<T, nElem>::size() const {
  unsigned short size = max_size_;
  if (!full_) {
    if (head_ >= tail_) {
      size = head_ - tail_;
    } else {
      size = max_size_ + head_ - tail_;
    }
  }
  return size;
}

template <class T, unsigned short nElem> inline u_int16_t CircularBuffer<T, nElem>::linearEnd() const {
  return max_size_ - head_;
}