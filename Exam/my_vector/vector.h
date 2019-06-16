//
// Created by timfame on 15.06.19.
//

#ifndef EXAM_VECTOR_H
#define EXAM_VECTOR_H

#include <algorithm>
#include <utility>
#include <cstdint>
#include <memory>

template <class T>
struct vector {

    vector() noexcept : size_(0) {};

    vector(vector const& other) : size_(other.size_) {
        if (is_dynamic()) {
            ++other.data_.dynamical->refs;
            data_.dynamical = other.data_.dynamical;
        } else {
            new (&data_.statical) T(other.data_.statical[0]);
        }
    };

    ~vector() noexcept {
        if (is_dynamic()) {
            --data_.dynamical->refs;
            if (data_.dynamical->refs == 0)
                delete data_.dynamical;
        }
    }

  //  template <class InputIterator>
  //  vector(InputIterator first, InputIterator last);

    vector& operator=(vector const& other) {
        if (this == &other)
            return *this;
        if (other.is_dynamic()) {
            if (is_dynamic())
                delete_dynamic();
            else
                if (size_ > 0)
                    data_.statical[0].~T();
            ++other.data_.dynamical->refs;
            data_.dynamical = new dynamic(other.data_.dynamical);

        } else {
            if (is_dynamic())
                delete_dynamic();
            else
                if (size_ > 0)
                    data_.statical[0].~T();
            new (&data_.statical) T(other.data_.statical[0]);
        }
        size_ = other.size_;
        return *this;
    }

  //  template <class InputIterator>
  //  void assign(InputIterator first, InputIterator last);

    T const& operator[](size_t index) const {
        return data()[index];
    }

    T& operator[](size_t index) {
        delete_ref();
        return data()[index];
    }

    T const& front() const {
        return *data();
    }

    T& front() {
        delete_ref();
        return *data();
    }

    T const& back() const {
        return data()[size_ - 1];
    }

    T& back() {
        delete_ref();
        return data()[size_ - 1];
    }

    void push_back(T const& element) {
        delete_ref();
        T tmp(element);
        if (is_dynamic()) {
            ensure_capacity();
            new (data_.dynamical->ptr + size_) T(tmp);
        } else {
            if (empty()) {
                new(&data_.statical) T(tmp);
            } else {
                make_dynamical();
                ensure_capacity();
                new(data_.dynamical->ptr + size_) T(tmp);
            }
        }
        ++size_;
    }

    void pop_back() {
        if (empty()) {
            throw std::runtime_error("Pop from empty vector");
        }
        delete_ref();
        if (is_dynamic() && size_ <= 2)
            make_statical();
        --size_;
    }

    T* data() const {
        return (is_dynamic() ? (data_.dynamical->ptr) : const_cast<T *>(data_.statical));
    }

    bool empty() const noexcept {
        return size_ == 0;
    }

    size_t size() const noexcept {
        return size_;
    }

    void reserve(size_t n) {
        if (is_dynamic() && n < size_)
            return;
        if (!is_dynamic() && n > 1) {
            make_dynamical();
        }
        delete_ref();
        new_dynamical(size_, n, data_.dynamical->refs);
    }

    size_t capacity() const {
        return (is_dynamic() ? data_.dynamical->capacity : 1);
    }

    void shrink_to_fit() {
        reserve(size_);
    }

    void resize(size_t n) {
        if (n == 0) {
            clear();
            return;
        }
        resize(n, T());
    }

    void resize(size_t n, T const& val) {
       if (!is_dynamic()) {
           if (n > 1)
               make_dynamical();
           else {
               if (n == 0)
                   data_.statical[0].~T();
               size_ = n;
               return;
           }
       }
       delete_ref();
       if (n == 0)
           delete_dynamic();
       else if (n == 1)
           make_statical();
       else {
           auto tmp = new dynamic(n * 2, data_.dynamical->refs);
           for (size_t i = 0; i < std::min(size_, n); ++i)
               new(tmp->ptr + i) T(data()[i]);
           delete_dynamic();
           data_.dynamical = tmp;
           T temp(val);
           for (size_t i = size_; i < n; ++i)
               new(tmp->ptr + i) T(temp);
       }
       size_ = n;
    }

    void clear() {
        if (is_dynamic())
            delete_dynamic();
        else
            if (size_ > 0)
                data_.statical[0].~T();
        size_ = 0;
    }

    friend void swap(vector& a, vector& b) {
        if (a.is_dynamic() && b.is_dynamic())
            std::swap(a.data_.dynamical, b.data_.dynamical);
        else if (a.is_dynamic() && !b.is_dynamic())
            swap_ds(a, b);
        else if (!a.is_dynamic() && b.is_dynamic())
            swap_ds(b, a);
        else
            swap_ss(a, b);
        std::swap(a.size_, b.size_);
    }

    friend bool operator==(vector const& a, vector const& b) {
        if (a.is_dynamic() != b.is_dynamic() || a.size_ != b.size_)
            return false;
        for (size_t i = 0; i < a.size_; ++i)
            if (a.data()[i] != b.data()[i])
                return false;
        return true;
    }

    friend bool operator<(vector const& a, vector const& b) {
        if (a.size_ != b.size_)
            return a.size_ < b.size_;
        for (size_t i = 0; i < a.size_; ++i) {
            if (a.data()[i] == b.data()[i])
                continue;
            return a.data()[i] < b.data()[i];
        }
        return false;
    }

private:
    size_t size_;
    struct dynamic {
        T* ptr;
        size_t capacity;
        size_t refs;

        dynamic() : ptr(nullptr), capacity(0), refs(1) {};

        dynamic(size_t c, size_t r) : capacity(c), refs(r) {
            ptr = static_cast<T*>(::operator new(c * sizeof(T)));
        }

        explicit dynamic(dynamic const* other) : ptr(other->ptr), capacity(other->capacity), refs(other->refs) {}

        dynamic& operator=(dynamic const& other) {
            ptr = other.ptr;
            capacity = other.capacity;
            refs = other.refs;
            return *this;
        }

        ~dynamic() {
            ::operator delete (ptr);
        }

        bool unique() const {
            return refs == 1;
        }
    };
    union some {
        T statical[1];
        dynamic* dynamical;
        some(){};
        ~some(){};
    } data_;

    bool is_dynamic() const {
        return size_ > 1;
    }

    friend void swap_ds(vector& a, vector& b) {
        if (b.size_ > 0) {
            T tmp(b.data_.statical[0]);
            b.data_.statical[0].~T();
            b.data_.dynamical = a.data_.dynamical;
            new (&a.data_.statical) T(tmp);
            return;
        }
        b = a;
        b.delete_ref();
        a.delete_dynamic();
        b.size_ = 0;
    };

    friend void swap_ss(vector& a, vector& b) {
        if (a.size_ == 1 && b.size_ == 1) {
            std::swap(a.data_.statical, b.data_.statical);
        } else if (a.size_ == 1 && b.size_ == 0) {
            new (&b.data_.statical) T(a.data_.statical[0]);
            a.data_.statical[0].~T();
        } else if (a.size_ == 0 && b.size_ == 1) {
            new (&a.data_.statical) T(b.data_.statical[0]);
            b.data_.statical[0].~T();
        }
    }

    void new_dynamical(size_t n, size_t cap, size_t ref) {
        auto tmp = new dynamic(cap, ref);
        for (size_t i = 0; i < n; ++i)
            new (tmp->ptr + i) T(data()[i]);
        delete_dynamic();
        data_.dynamical = tmp;
    }

    void make_dynamical() {
        auto tmp = new dynamic(4, 1);
        if (size_ > 0) {
            new(tmp->ptr) T(data_.statical[0]);
            data_.statical[0].~T();
        }
        data_.dynamical = tmp;
    }

    void make_statical() {
        T elem = *data();
        delete_dynamic();
        new (&data_.statical) T(elem);
    }

    void ensure_capacity() {
        size_t new_size = size_ + 1;
        if (new_size >= data_.dynamical->capacity) {
            auto tmp = new dynamic(new_size * 2, data_.dynamical->refs);
            for (size_t i = 0; i < size_; ++i)
                new (tmp->ptr + i) T(data()[i]);
            delete_dynamic();
            data_.dynamical = tmp;
        }
    }

    void delete_ref() {
        if (is_dynamic() && !data_.dynamical->unique()) {
            auto tmp = new dynamic(capacity(), 1);
            for (size_t i = 0; i < size_; ++i)
                new (tmp->ptr + i) T(data()[i]);
            --data_.dynamical->refs;
            if (data_.dynamical->refs == 0) {
                delete_dynamic();
            }
            data_.dynamical = tmp;
        }
    }

    void delete_dynamic() {
        for (size_t i = 0; i < size_; ++i)
            data_.dynamical->ptr[i].~T();
        delete data_.dynamical;
    }
};

template <class T>
bool operator!=(vector<T> const& a, vector<T> const& b) {
    return !(a == b);
}

template <class T>
bool operator<=(vector<T> const& a, vector<T> const& b) {
    return !(b < a);
}

template <class T>
bool operator> (vector<T> const& a, vector<T> const& b) {
    return b < a;
}

template <class T>
bool operator>=(vector<T> const& a, vector<T> const& b) {
    return !(a < b);
}


#endif //EXAM_VECTOR_H
