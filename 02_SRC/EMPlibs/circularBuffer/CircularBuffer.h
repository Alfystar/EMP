/*
 * CircularBuffer.h
 *
 *  Created on: 07 nov 2019
 *      Author: alfyhack
 */

#ifndef PROJECT_LIB_CIRCULARBUFFER_CIRCULARBUFFER_H_
#define PROJECT_LIB_CIRCULARBUFFER_CIRCULARBUFFER_H_

#include <iostream>
#include <stdlib.h>

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

  // Puts metod
  int put(T item);                      // return new head
  int put(T *item);                     // take enObj pointer and copy in internal buffer, return new head
  int put(T *item, unsigned short len); // take enObj pointer and copy in internal buffer, return new head

  int put_externalWrite();        // return old head
  int put_externalWrite(int len); // add len to head, return old head

  // Gets metod
  T get();
  T *getPtr();
  unsigned short get_externalRead(); // return new tail

  // Special write, to take out char buf in specific structure
  // salvano in una mem i valori partendo da mytail --> mytail+len
  void writeMemOut(T *mem, unsigned short mytail, unsigned short len);

  // Get Head Structure information
  unsigned short getHead();
  T readHead();
  T *getHeadPtr();

  // Get Tail Structure information
  unsigned short getTail();
  T readTail();
  T *getTailPtr();

  // Structure operation
  void reset();
  bool empty() const; // true if empty
  bool full() const;  // true if full
  unsigned short capacity() const;
  unsigned short size() const;
  unsigned short linearEnd() const; // return space between last array index (ignore tail position)
};

template <class T, unsigned short nElem> CircularBuffer<T, nElem>::CircularBuffer() : max_size_(nElem) {
  this->head_ = 0;
  this->tail_ = 0;
  memClean();
}

template <class T, unsigned short nElem> void CircularBuffer<T, nElem>::memClean() {
  for (unsigned int i = 0; i < max_size_; i++)
    this->buf_[i] = 0;
  reset();
}

/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Puts metod
template <class T, unsigned short nElem> int CircularBuffer<T, nElem>::put(T item) {
  buf_[head_] = item;
  return this->put_externalWrite(); // old head
}

template <class T, unsigned short nElem> int CircularBuffer<T, nElem>::put(T *item) {
  memcpy((void *)&buf_[head_], &item, sizeof(T));
  return this->put_externalWrite(); // old head
}

template <class T, unsigned short nElem> int CircularBuffer<T, nElem>::put(T *item, unsigned short len) {
  memcpy((void *)&buf_[head_], &item, len);
  return this->put_externalWrite(); // old head
}

template <class T, unsigned short nElem> inline int CircularBuffer<T, nElem>::put_externalWrite() {
  return put_externalWrite(1); // old head
}

template <class T, unsigned short nElem> inline int CircularBuffer<T, nElem>::put_externalWrite(int len) {
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
  auto val = buf_[tail_];
  this->get_externalRead();
  return val;
}

template <class T, unsigned short nElem> T *CircularBuffer<T, nElem>::getPtr() {
  if (empty()) {
    return nullptr;
  }
  // Read data and advance the tail (we now have a free space)
  auto val = getTailPtr();
  get_externalRead();
  return val;
}

template <class T, unsigned short nElem> inline unsigned short CircularBuffer<T, nElem>::get_externalRead() {
  full_ = false;
  tail_ = (tail_ + 1) % max_size_;
  return tail_;
}

template <class T, unsigned short nElem>
void CircularBuffer<T, nElem>::writeMemOut(T *mem, unsigned short mytail, unsigned short len) {
  for (unsigned int i = 0; i < len; i++) {
    mem[i] = buf_[(mytail + i) % max_size_];
  }
  full_ = false;
}
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Get Head Structure information

template <class T, unsigned short nElem> inline unsigned short CircularBuffer<T, nElem>::getHead() { return head_; }
template <class T, unsigned short nElem> inline T CircularBuffer<T, nElem>::readHead() { return this->buf_[head_]; }
template <class T, unsigned short nElem> inline T *CircularBuffer<T, nElem>::getHeadPtr() { return &buf_[head_]; }

// Get Tail Structure information
template <class T, unsigned short nElem> inline unsigned short CircularBuffer<T, nElem>::getTail() { return tail_; }
template <class T, unsigned short nElem> inline T CircularBuffer<T, nElem>::readTail() { return this->buf_[tail_]; }
template <class T, unsigned short nElem> inline T *CircularBuffer<T, nElem>::getTailPtr() { return &buf_[tail_]; }

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

template <class T, unsigned short nElem> inline unsigned short CircularBuffer<T, nElem>::capacity() const {
  return max_size_;
}

template <class T, unsigned short nElem> inline unsigned short CircularBuffer<T, nElem>::size() const {
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

template <class T, unsigned short nElem> inline unsigned short CircularBuffer<T, nElem>::linearEnd() const {
  return max_size_ - head_;
}

#endif /* PROJECT_LIB_CIRCULARBUFFER_CIRCULARBUFFER_H_ */
