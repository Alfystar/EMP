/*
 * CircularBuffer.h
 *
 *  Created on: 07 nov 2019
 *      Author: alfyhack
 */

#ifndef PROJECT_LIB_CIRCULARBUFFER_CIRCULARBUFFER_H_
#define PROJECT_LIB_CIRCULARBUFFER_CIRCULARBUFFER_H_
#ifndef LinuxSide
#include "Arduino.h"
#else

#include <stdlib.h>
#include <iostream>

#endif

template <class T>
class CircularBuffer{
public:
    CircularBuffer (T buf[], size_t size);
    //Puts metod
    int put (T item);        //return new head
    int put (T *item);    //take enObj pointer and copy in internal buffer, return new head
    int put (T *item, size_t len);    //take enObj pointer and copy in internal buffer, return new head
    int put_externalWrite ();            //return old head
    int put_externalWrite (int len);     //add len to head, return old head

    //Gets metod
    T get ();
    T *getPtr ();
    size_t get_externalRead ();    //return new tail

    //Get Head Structure information
    size_t getHead ();
    T readHead ();
    T *getHeadPtr ();
    //Get Tail Structure information
    size_t getTail ();
    T readTail ();
    T *getTailPtr ();
    //Special write, to take out char buf in specific structure
    void
    writeMemOut (T *mem, size_t mytail, size_t len);    //salvano in una mem i valori partendo da mytail --> mytail+len


    //
    void reset ();
    bool empty () const;    //true if empty
    bool full () const;
    size_t capacity () const;
    size_t size () const;
    size_t
    linearEnd () const;   //return how many slot have the head before encounter the last array index (ignore tail position)

private:
    T *buf_;
    #ifndef LinuxSide
    int head_;
    int tail_;
    #else
    size_t head_;
    size_t tail_;
    #endif
    const int max_size_;
    bool full_ = 0;
};

template <class T>
CircularBuffer <T>::CircularBuffer (T buf[], size_t nElem):
        max_size_(nElem){
    this->buf_ = buf;
    this->head_ = 0;
    this->tail_ = 0;
}

template <class T>
inline void CircularBuffer <T>::reset (){
    this->head_ = this->tail_;
    this->full_ = false;
}

template <class T>
inline bool CircularBuffer <T>::empty () const{
    //if head and tail are equal, we are empty
    return (!full_ && (head_ == tail_));    //true if empty
}

template <class T>
inline bool CircularBuffer <T>::full () const{
    //If tail is ahead the head by 1, we are full
    return full_;
}

template <class T>
inline size_t CircularBuffer <T>::capacity () const{
    return max_size_;
}

template <class T>
inline T CircularBuffer <T>::readTail (){
    return this->buf_[tail_];
}

template <class T>
inline T CircularBuffer <T>::readHead (){
    return this->buf_[head_];
}

template <class T>
void CircularBuffer <T>::writeMemOut (T *mem, size_t mytail, size_t len){
    for (unsigned int i = 0; i < len; i++){
        mem[i] = buf_[(mytail + i) % max_size_];
    }
    full_ = false;
}

template <class T>
inline size_t CircularBuffer <T>::size () const{
    size_t size = max_size_;
    if (!full_){
        if (head_ >= tail_){
            size = head_ - tail_;
        }
        else{
            size = max_size_ + head_ - tail_;
        }
    }
    return size;
}

template <class T>
int CircularBuffer <T>::put (T item){
    buf_[head_] = item;
    return this->put_externalWrite(); //old head
}

template <class T>
int CircularBuffer <T>::put (T *item){
    memcpy((void *) &buf_[head_], &item, sizeof(T));
    return this->put_externalWrite(); //old head
}

template <class T>
int CircularBuffer <T>::put (T *item, size_t len){
    memcpy((void *) &buf_[head_], &item, len);
    return this->put_externalWrite(); //old head
}

template <class T>
inline int CircularBuffer <T>::put_externalWrite (){
    return put_externalWrite(1); //old head
}

template <class T>
T CircularBuffer <T>::get (){
    if (empty()){
        return T();
    }

    //Read data and advance the tail (we now have a free space)
    auto val = buf_[tail_];
    this->get_externalRead();
    return val;
}

template <class T>
T *CircularBuffer <T>::getPtr (){
    if (empty()){
        return nullptr;
    }
    //Read data and advance the tail (we now have a free space)
    auto val = getTailPtr();
    get_externalRead();
    return val;
}

template <class T>
inline size_t CircularBuffer <T>::get_externalRead (){
    full_ = false;
    tail_ = (tail_ + 1) % max_size_;
    return tail_;
}

template <class T>
inline size_t CircularBuffer <T>::getHead (){
    return head_;
}

template <class T>
inline T *CircularBuffer <T>::getHeadPtr (){
    return &buf_[head_];
}

template <class T>
inline size_t CircularBuffer <T>::getTail (){
    return tail_;
}

template <class T>
inline T *CircularBuffer <T>::getTailPtr (){
    return &buf_[tail_];
}

template <class T>
inline size_t CircularBuffer <T>::linearEnd () const{
    return max_size_ - head_;
}

template <class T>
inline int CircularBuffer <T>::put_externalWrite (int len){
    int oldHead = this->head_;
    if (full_)
        tail_ = (tail_ + len) % max_size_;
    head_ = (head_ + len) % max_size_;
    full_ = (head_ == tail_);
    return oldHead; //old head
}

#endif /* PROJECT_LIB_CIRCULARBUFFER_CIRCULARBUFFER_H_ */
