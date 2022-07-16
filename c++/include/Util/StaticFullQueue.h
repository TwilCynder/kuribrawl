#pragma once
#include <iostream>
#include <memory>
#include "../KBDebug/Debug.h"
using namespace std;

/**
 * @brief Fixed-size queue that is always full.
 * The StaticFullQueue (or SFQ) is a FIFO Collection with a compile-time static size, that always holds SIZE elements.  
 * It means that it will already be full upon creation, and the only way for an element to be removed is to be remplaced by a new element.
 * @tparam T 
 * @tparam SIZE 
 */
template <typename T, size_t SIZE>
class StaticFullQueue {
    protected:
    T elements[SIZE];
    size_t head_;   //index where a value is GOING to be written (OLDEST index (FIRST in iteration))
                //tail is head_-1

    /**
     * @brief return the next array index in the queue :   
     * either i + 1 or 0 if the given offset was the last valid offset
     * @param i a memory offset (divided by sizeof(T)), i.e. index in the elements array
     * @return size_t the offset of the next element
     */
    inline static size_t inc(size_t i) {
        return (i + 1 >= SIZE) ? 0 : i + 1; 
    }

    public:
    constexpr static size_t size = SIZE; ///< Size of this queue
    /**
     * @brief Construct a new Static Full Queue with no content initialization : 
     * it is considered full, but contains garbage data. Use this constructor with care.
     */
    StaticFullQueue() : 
        head_(0) 
    {}

    /**
     * @brief Construct a new Static Full Queue with al elements set to a given value
     * @param initial_value the value of all the initial elements
     */
    StaticFullQueue(T initial_value) : 
        StaticFullQueue()
    {  
        //stupid dumb C ass for loop idk how to do it better
        for (T* i = elements; i < elements + SIZE; i++){
            *i = initial_value;
        }
    }

    /**
     * @brief returns the array index of the tail element  
     * 
     * tail = Newest element, last in iteration going to be overwritten last
     * @return size_t 
     */
    inline size_t tail() {
        return ((ptrdiff_t)head_ - 1 < 0) ? SIZE - 1 : head_ - 1;
    }

    /**
     * @brief Adds a new element to que queue, overwriting the oldest element
     * 
     * The overwritten element was the head.
     * @param element 
     */
    void add(T element){
        elements[head_] = element;
        head_ = inc(head_);
    }

    /**
     * @brief Returns the array containing all the elements.  
     * Element are "ordered", but the first one is not necessarily the head; it way me any element in the middle of the queue, 
     * followed by all elements in order up to the tail, followed by the head, followed by all the other elements in order.
     * 
     * @return const T* a pointer to all the SIZE elements.
     */
    const T* data() const {
        return elements;
    }

    /**
     * @brief Returns the head element.
     * 
     * head = Oldest element, first in iteration, going to be overwritten next.
     * @return T the oldest element.
     */
    T head(){
        return elements[head_];
    }

    /**
     * @brief returns the element at a specified index in the queue (head = index 0, tail = index SIZE - 1)
     * 
     * @param i an index between 0 and SIZE - 1, inclusive
     * @return const T the element at this position in the queue
     */
    const T operator[](std::size_t i){
        return elements[(head_ + i) % SIZE];
    }

    /**
     * @brief Iterator iterating over elements of a SFQ.
     */
    class Iterator;

    /**
     * @brief Returns an iterator to this SFQ, pointing to the first element.
     * @return Iterator 
     */
    Iterator iterator(){
        return Iterator(*this);
    }

    /**
     * @brief Returns an iterator to this SFQ, pointing just before the first element.
     * @return Iterator 
     */
    Iterator before_start(){
        return Iterator(*this, tail());
    }

    /**
     * @brief See ::iterator
     * @return Iterator 
     */
    inline Iterator begin(){
        return iterator();
    }

    /**
     * @brief Returns an iterator to this SFQ, already poiting past the end (and therefore invalid, see StaticFullQueue<T, SIZE>::Iterator::past_end)
     * @return Iterator 
     */
    inline Iterator end(){
        return Iterator(*this, nullptr);
    }

    /**
     * @brief Returns whether this queue contains a certain element.
     * @param val 
     * @return true if one of the elements in the queue equals val.
     */
    bool contains(const T& val) const {
        for (const T& v : elements){
            if (val == v) return true;
        }
        return false;
    }
    
    /**
     * @brief Returns whether this queue does not contains a certain element.
     * 
     * sfq.contains(val) <=> !sfq.containsNot(val); however this function is a bit more efficient than just doing !sfq.contains
     * @param val 
     * @return true if none of the elements in the queue equal val.
     */
    bool containsNot(const T& val) const {
        for (const T& v : elements){
            if (val != v) return true;
        }
        return false;
    }

    /**
     * @brief Prints all the elements in the queue to a given stream.
     * @param out the output
     * @param separator a character inserted between each element (generaly a space or newline)
     */
    void print(std::ostream& out, const std::string& separator){
        for (const T& v : *this){
            out << v << separator;
        }
    }
};

/**
 * @brief operator overload allowing one to feed a SFQ to an ostream through <<.s
 * @tparam T
 * @tparam size 
 * @param out 
 * @param sfq 
 * @return ostream& a reference to the stream so we can keep using more <<
 */
template<typename T, size_t size>
ostream& operator<<(ostream& out, StaticFullQueue<T, size> sfq){
    sfq.print(out, " ");
    return out;
}

/**
 * @brief Iterator iterating over elements of a SFQ.
 * 
 * SFQ iterators are invalidated if : 
 * - an element is added to the queue (except if each addition if followed by a call to ::advance)
 * - the queue is moved.
 * @tparam T type of thge queue elements 
 * @tparam SIZE number of elements in the queue
 */
template<typename T, size_t SIZE>
class StaticFullQueue<T, SIZE>::Iterator {
    friend class StaticFullQueue<T, SIZE>;

    protected:
    using SFQ = StaticFullQueue<T, SIZE>;
    SFQ& sfq; ///< Queue we are iterating over
    T* cursor;  ///< Pointer to the current element.

    /**
     * @brief Construct a new SFQ Iterator, iterating over a given queue, starting with a given element.
     * 
     * Does not check whether the element is actually in the queue; if not, this iterator will have undefined behavior.
     * @param sfq_ 
     * @param cursor_ 
     */
    Iterator(SFQ& sfq_, T* cursor_) : 
        sfq(sfq_), 
        cursor(cursor_)
    {
    }

    /**
     * @brief Construct a new SFQ Iterator, pointing to the element at a certain position in the queue.
     * @param sfq_ 
     * @param pos 
     */
    Iterator(SFQ& sfq_, int pos) : 
        Iterator(sfq_, sfq_.elements + pos)
    {}

    /**     
     * @brief Construct a new SFQ Iterator, pointing to the head.
     * @param sfq_ 
     * @param pos 
     */
    Iterator(SFQ& sfq_) : 
        Iterator(sfq_, sfq_.head_)
    {}
    public:

    /**
     * @brief Moves to the next element in the queue. 
     * This can make the iterator go past the end (if it pointed on the last element), making it invalid for all operations (other than comparison); see ::past_end
     */
    inline void advance(){
        if (!has_more()){
            cursor = nullptr;
            return;
        }
        cursor++;
        if (cursor >= sfq.elements + SIZE)
            cursor = sfq.elements;
    }

    /**
     * @brief Returns whether this iterator is past the end (making it invalid for most operations).  
     * An operator past the end can still be compared safely; ::advance will do nothing besides making them completely invalid even for comparison;  
     * All other operations may result in a crash. It is your responsility to handle iterator validity carefully, mainly using this method.
     * @return bool
     */
    bool past_end(){
        return !cursor;
    }

    /**
     * @brief Returns the current element.
     * @return const T& reference to the current element.
     */
    inline const T& value() const {
        return *cursor;
    }

    /**
     * @brief Returns whether there are more elements after the current one.  
     * @return true if the current element is not the last one.
     * @return false if the current element is the last one, in which case ::next is invalid.
     */
    inline bool has_more() const {
        return cursor != sfq.elements + sfq.tail();
    }

    /**
     * @brief Advances the iterator to the next element and returns it.  
     * This operation is invalid if the iterator was pointing to the last element, which you can test with ::has_more
     * @return const T& the next element in the queue.
     */
    const T& next(){
        advance();
        return value();
    }
    
    /**
     * @brief Port-incrementation : Advances the iterator and return the former current element.
     * @return const T& current element before avancing.
     */
    const T& operator++(){
        T* tmp = cursor;
        advance();
        return *tmp;
    }

    /**
     * @brief Pre-incrementation : see ::next
     * @return const T& the next element in the queue.
     */
    const T& operator++(int){
        return next();
    }

    /**
     * @brief Dereferencing : Returns the current element (see ::value)
     * @return const T& a reference to the current element.
     */
    const T& operator*(){
        return value();
    }

    /**
     * @brief Tests if two SFQ iterators are equal
     * i.e. they point to the same element in the same queue.  
     * @param it another iterator
     * @return bool
     */
    bool operator==(Iterator& it){
        return cursor == it.cursor;
    }

    /**
     * @brief Tests if two SFQ iterators are inequal.
     * i.e. they do not point to the same element.
     * @param it another iterator
     * @return bool
     */
    bool operator!=(Iterator& it){
        return cursor != it.cursor;
    }

    /**
     * @brief Compares a SFQ iterator with a simple pointer to an element.  
     * Equal if they both represent the same element.  
     * @param p a pointer
     * @return bool
     */
    bool operator==(T* p){
        return cursor == p;
    }
};