//
// Created by timfame on 17.06.19.
//

#ifndef CIRCULAR_BUFFER_CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_CIRCULAR_BUFFER_H


#include <cstdio>
#include <initializer_list>

template <class T>
struct circular_buffer {

    circular_buffer() : size_(0), capacity_(1), data_(static_cast<T*>(::operator new (sizeof(T)))), first_(0), last_(0) {};

    explicit circular_buffer(size_t capacity) : size_(0), capacity_(capacity), data_(static_cast<T*>(::operator new (capacity * sizeof(T)))),
                                                                        first_(0), last_(0) {};

    circular_buffer(circular_buffer const& other) : size_(other.size_), capacity_(other.capacity_),
                                                    data_(static_cast<T*>(::operator new (capacity_ * sizeof(T)))),
                                                    first_(0), last_(size_) {
        for (size_t i = 0; i < size_; ++i)
            new (data_ + i) T(other.data_[i]);
    }

    circular_buffer(std::initializer_list<T> values) : size_(values.size()), capacity_(size_ * 2),
                                                        data_(static_cast<T*>(::operator new (capacity_ * sizeof(T)))),
                                                        first_(0), last_(size_) {
        for (size_t i = 0; i < size_; ++i)
            new (data_ + i) T(values[i]);
    }

    ~circular_buffer() {
        ::operator delete (data_);
    }

    void push_back(T const& element) {
        if (++last_ == capacity_)
            last_ = 0;
        if (last_ == first_) {
            if (size_ > 0)
                data_[first_].~T();
            ++first_;
        }
        if (first_ == capacity_)
            first_ = 0;
        T tmp(element);
        new (data_ + last_) T(tmp);
        ++size_;
        if (size_ > capacity_)
            size_ = capacity_;
    }

    void push_front(T const& element) {
        if (first_ == 0)
            first_ = capacity_ - 1;
        else
            --first_;
        if (last_ == first_) {
            if (size_ > 0)
                data_[first_].~T();
            if (last_ == 0)
                last_ = capacity_ - 1;
            else
                --last_;
        }
        T tmp(element);
        new (data_ + first_) T(tmp);
        ++size_;
        if (size_ > capacity_)
            size_ = capacity_;
    }

    T const& operator[](size_t index) const {
        return data_[(first_ + index) % size_];
    }

    T& operator[](size_t index) {
        return data_[(first_ + index) % size_];
    }

    size_t size() const {
        return size_;
    }

    bool empty() const {
        return size_ == 0;
    }

private:
    size_t size_;
    size_t capacity_;
    T* data_;
    size_t first_;
    size_t last_;
};


#endif //CIRCULAR_BUFFER_CIRCULAR_BUFFER_H
