#pragma once
#include <iostream>
#include <memory>
#include "../KBDebug/Debug.h"
using namespace std;

/**
 * @brief Fixed-size queue that is always full.
 * The StaticFullQueue is a FIFO Collection with a compile-time static size, that always holds SIZE elements.  
 * It means that it will already be full upon creation, and the only way for an element to be removed is to be remplaced by a new element.
 * @tparam T 
 * @tparam SIZE 
 */
template <typename T, int SIZE>
class StaticFullQueue {
    protected:
    T elements[SIZE];
    int head_;   //index where a value is GOING to be written (OLDEST index (FIRST in iteration))
                //tail is head_-1
    public:
    StaticFullQueue() : 
        head_(0) 
    {}

    StaticFullQueue(T initial_value) : 
        StaticFullQueue()
    {  
        //stupid dumb C ass for loop idk how to do it better
        Debug::log(initial_value);
        for (T* i = elements; i < elements + SIZE; i++){
            *i = initial_value;
        }
    }

    inline int inc(int i) {
        return (i + 1 >= SIZE) ? 0 : i + 1; 
    }
    inline int tail() {
        return (head_ - 1 < 0) ? SIZE - 1 : head_ - 1;
    }

    void add(T element){
        elements[head_] = element;
        head_ = inc(head_);
    }

    const T* data() const {
        return elements;
    }

    T head(){
        return elements[head_];
    }

    const T operator[](std::size_t i){
        return elements[(head_ + i) % SIZE];
    }

    class Iterator;
    Iterator iterator(){
        return Iterator(*this);
    }

    Iterator before_start(){
        return Iterator(*this, tail());
    }

    inline Iterator begin(){
        return iterator();
    }

    inline Iterator end(){
        return Iterator(*this, nullptr);
    }
};

template<typename T, int SIZE>
class StaticFullQueue<T, SIZE>::Iterator {
    friend class StaticFullQueue<T, SIZE>;

    protected:
    using SFQ = StaticFullQueue<T, SIZE>;
    SFQ& sfq;
    T* cursor;

    Iterator(SFQ& sfq_, T* cursor_) : 
        sfq(sfq_), 
        cursor(cursor_)
    {
    }

    Iterator(SFQ& sfq_, int pos) : 
        Iterator(sfq_, sfq_.elements + pos)
    {}

    Iterator(SFQ& sfq_) : 
        Iterator(sfq_, sfq_.head_)
    {}
    public:
    inline void advance(){
        if (cursor == sfq.elements + sfq.tail()){
            cursor = nullptr;
            return;
        }
        cursor++;
        if (cursor >= sfq.elements + SIZE)
            cursor = sfq.elements;
    }

    bool past_end(){
        return !cursor;
    }

    inline const T& value() const {
        return *cursor;
    }

    //invalid if we advanced past the end
    const T& next(){
        advance();
        return value();
    }
    
    const T& operator++(){
        T* tmp = cursor;
        advance();
        return *tmp;
    }

    const T& operator++(int){
        return next();
    }

    const T& operator*(){
        return value();
    }

    bool operator==(Iterator& it){
        return cursor == it.cursor;
    }

    bool operator!=(Iterator& it){
        return cursor != it.cursor;
    }

    bool operator==(T* p){
        return cursor == p;
    }
};