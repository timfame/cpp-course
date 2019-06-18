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

    template <class V>
    struct VIterator : std::iterator<std::random_access_iterator_tag, V> {

        friend vector;

        VIterator() : ptr(nullptr) {};

        ~VIterator() = default;

        template <class Another>
        VIterator(Another* p) : ptr(p) {}

        template <class Another>
        VIterator(VIterator<Another> const& other) : ptr(other.ptr) {}

        template <class Another>
        VIterator& operator=(VIterator<Another> const& other) {
            ptr = other.ptr;
            return *this;
        }

        template <class Another>
        bool operator==(VIterator<Another> const& b) const {
            return ptr == b.ptr;
        }

        template <class Another>
        bool operator!=(VIterator<Another> const& b) const {
            return ptr != b.ptr;
        }

    //    V const& operator*() const {
    //        return *ptr;
    //    }

        V& operator*() {
            return *ptr;
        }

        V* operator->() {
            return ptr;
        }

        VIterator& operator++() {
            ++ptr;
            return *this;
        }

        VIterator operator++(int) {
            VIterator tmp(*this);
            ++ptr;
            return tmp;
        }

        VIterator& operator--() {
            --ptr;
            return *this;
        }

        VIterator operator--(int) {
            VIterator tmp(*this);
            --ptr;
            return tmp;
        }

        VIterator& operator+=(size_t index) {
            ptr += index;
            return *this;
        }

        VIterator operator+(size_t index) {
            return VIterator(*this) += index;
        }

        VIterator& operator-=(size_t index) {
            ptr -= index;
            return *this;
        }

        VIterator operator-(size_t index) {
            return VIterator(*this) -= index;
        }

        template <class Another>
        bool operator<(VIterator<Another> const& b) const {
            return ptr < b.ptr;
        }

        template <class Another>
        bool operator<=(VIterator<Another> const& b) const {
            return ptr <= b.ptr;
        }

        template <class Another>
        bool operator>(VIterator<Another> const& b) const {
            return ptr > b.ptr;
        }

        template <class Another>
        bool operator>=(VIterator<Another> const& b) const {
            return ptr > b.ptr;
        }

    private:
        V* ptr;
    };

    typedef T value_type;
    typedef VIterator<T> iterator;
    typedef VIterator<T const> const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    vector() noexcept : _size(0) {};

    vector(vector const& other) : _size(other._size) {
        if (is_dynamic()) {
            ++other._data.dynamical->refs;
            _data.dynamical = other._data.dynamical;
        } else {
            new (&_data.statical) T(other._data.statical[0]);
        }
    };

    ~vector() noexcept {
        if (is_dynamic()) {
            --_data.dynamical->refs;
            if (_data.dynamical->refs == 0)
                delete _data.dynamical;
        }
    }

    template <class InputIterator>
    vector(InputIterator first, InputIterator last) : _size(0){
        for (InputIterator i = first; i != last; ++i)
            push_back(*i);
    }

    vector& operator=(vector const& other) {
        if (this == &other)
            return *this;
        if (other.is_dynamic()) {
            if (is_dynamic())
                delete_dynamic();
            else
                if (_size > 0)
                    _data.statical[0].~T();
            ++other._data.dynamical->refs;
            _data.dynamical = new dynamic(other._data.dynamical);

        } else {
            if (is_dynamic())
                delete_dynamic();
            else
                if (_size > 0)
                    _data.statical[0].~T();
            new (&_data.statical) T(other._data.statical[0]);
        }
        _size = other._size;
        return *this;
    }

    template <class InputIterator>
    void assign(InputIterator first, InputIterator last) {
        clear();
        for (InputIterator i = first; i != last; ++i)
            push_back(*i);
    }

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
        return data()[_size - 1];
    }

    T& back() {
        delete_ref();
        return data()[_size - 1];
    }

    void push_back(T const& element) {
        delete_ref();
        T tmp(element);
        if (is_dynamic()) {
            ensure_capacity();
            new (_data.dynamical->ptr + _size) T(tmp);
        } else {
            if (empty()) {
                new(&_data.statical) T(tmp);
            } else {
                make_dynamical();
                ensure_capacity();
                new(_data.dynamical->ptr + _size) T(tmp);
            }
        }
        ++_size;
    }

    void pop_back() {
        if (empty()) {
            throw std::runtime_error("Pop from empty vector");
        }
        delete_ref();
        if (is_dynamic() && _size <= 2)
            make_statical();
        --_size;
    }

    T* data() const {
        return (is_dynamic() ? (_data.dynamical->ptr) : const_cast<T *>(_data.statical));
    }

    iterator begin() noexcept {
        return iterator(data());
    }

    iterator end() noexcept  {
        return iterator(data() + _size);
    }

    const_iterator begin() const noexcept {
        return const_iterator(data());
    }

    const_iterator end() const noexcept {
        return const_iterator(data() + _size);
    }

    reverse_iterator rbegin() noexcept {
        return reverse_iterator(begin());
    }

    reverse_iterator rend() noexcept {
        return reverse_iterator(end());
    }

    const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator(begin());
    }

    const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator(end());
    }

    bool empty() const noexcept {
        return _size == 0;
    }

    size_t size() const noexcept {
        return _size;
    }

    void reserve(size_t n) {
        if (is_dynamic() && n < _size)
            return;
        if (!is_dynamic() && n > 1) {
            make_dynamical();
        }
        delete_ref();
        new_dynamical(_size, n, _data.dynamical->refs);
    }

    size_t capacity() const {
        return (is_dynamic() ? _data.dynamical->capacity : 1);
    }

    void shrink_to_fit() {
        reserve(_size);
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
                   _data.statical[0].~T();
               _size = n;
               return;
           }
       }
       delete_ref();
       if (n == 0)
           delete_dynamic();
       else if (n == 1)
           make_statical();
       else {
           auto tmp = new dynamic(n * 2, _data.dynamical->refs);
           for (size_t i = 0; i < std::min(_size, n); ++i)
               new(tmp->ptr + i) T(data()[i]);
           delete_dynamic();
           _data.dynamical = tmp;
           T temp(val);
           for (size_t i = _size; i < n; ++i)
               new(tmp->ptr + i) T(temp);
       }
       _size = n;
    }

    void clear() {
        delete_ref();
        if (is_dynamic())
            delete_dynamic();
        else
            if (_size > 0)
                _data.statical[0].~T();
        _size = 0;
    }

    friend void swap(vector& a, vector& b) {
        if (a.is_dynamic() && b.is_dynamic())
            std::swap(a._data.dynamical, b._data.dynamical);
        else if (a.is_dynamic() && !b.is_dynamic())
            swap_ds(a, b);
        else if (!a.is_dynamic() && b.is_dynamic())
            swap_ds(b, a);
        else
            swap_ss(a, b);
        std::swap(a._size, b._size);
    }

    friend bool operator==(vector const& a, vector const& b) {
        if (a.is_dynamic() != b.is_dynamic() || a._size != b._size)
            return false;
        for (size_t i = 0; i < a._size; ++i)
            if (a.data()[i] != b.data()[i])
                return false;
        return true;
    }

    friend bool operator<(vector const& a, vector const& b) {
        if (a._size != b._size)
            return a._size < b._size;
        for (size_t i = 0; i < a._size; ++i) {
            if (a.data()[i] == b.data()[i])
                continue;
            return a.data()[i] < b.data()[i];
        }
        return false;
    }

    iterator insert(const_iterator pos, T const& val) {
        vector tmp;
        for (auto i = begin(); i != pos; ++i)
            tmp.push_back(*i);
        tmp.push_back(val);
        iterator ret = tmp.end() - 1;
        for (auto i = pos; i != end();  ++i)
            tmp.push_back(*i);
        swap(*this, tmp);
        return ret;
    }

    iterator erase(const_iterator pos) {
        vector tmp;
        for (auto i = begin(); i != pos; ++i)
            tmp.push_back(*i);
        iterator ret = tmp.end();
        if (pos != end())
            for (auto i = pos + 1; i != end(); ++i)
                tmp.push_back(*i);
        swap(*this, tmp);
        return ret;
    }

    iterator erase(const_iterator first, const_iterator last) {
        vector tmp;
        for (auto i = begin(); i != first; ++i)
            tmp.push_back(*i);
        iterator ret = tmp.end();
        for (auto i = last; i != end(); ++i)
            tmp.push_back(*i);
        swap(*this, tmp);
        return ret;
    }

private:
    size_t _size;
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
    } _data;

    bool is_dynamic() const {
        return _size > 1;
    }

    friend void swap_ds(vector& a, vector& b) {
        if (b._size > 0) {
            T tmp(b._data.statical[0]);
            b._data.statical[0].~T();
            b._data.dynamical = a._data.dynamical;
            new (&a._data.statical) T(tmp);
            return;
        }
        b._data.dynamical = a._data.dynamical;
    };

    friend void swap_ss(vector& a, vector& b) {
        if (a._size == 1 && b._size == 1) {
            std::swap(a._data.statical, b._data.statical);
        } else if (a._size == 1 && b._size == 0) {
            new (&b._data.statical) T(a._data.statical[0]);
            a._data.statical[0].~T();
        } else if (a._size == 0 && b._size == 1) {
            new (&a._data.statical) T(b._data.statical[0]);
            b._data.statical[0].~T();
        }
    }

    void new_dynamical(size_t n, size_t cap, size_t ref) {
        auto tmp = new dynamic(cap, ref);
        for (size_t i = 0; i < n; ++i)
            new (tmp->ptr + i) T(data()[i]);
        delete_dynamic();
        _data.dynamical = tmp;
    }

    void make_dynamical() {
        auto tmp = new dynamic(4, 1);
        if (_size > 0) {
            new(tmp->ptr) T(_data.statical[0]);
            _data.statical[0].~T();
        }
        _data.dynamical = tmp;
    }

    void make_statical() {
        T elem(*data());
        delete_dynamic();
        new (&_data.statical) T(elem);
    }

    void ensure_capacity() {
        size_t new_size = _size + 1;
        if (new_size >= _data.dynamical->capacity) {
            auto tmp = new dynamic(new_size * 2, _data.dynamical->refs);
            for (size_t i = 0; i < _size; ++i)
                new (tmp->ptr + i) T(data()[i]);
            delete_dynamic();
            _data.dynamical = tmp;
        }
    }

    void delete_ref() {
        if (is_dynamic() && !_data.dynamical->unique()) {
            auto tmp = new dynamic(capacity(), 1);
            for (size_t i = 0; i < _size; ++i)
                new (tmp->ptr + i) T(data()[i]);
            --_data.dynamical->refs;
            if (_data.dynamical->refs == 0) {
                delete_dynamic();
            }
            _data.dynamical = tmp;
        }
    }

    void delete_dynamic() {
        for (size_t i = 0; i < _size; ++i)
            _data.dynamical->ptr[i].~T();
        delete _data.dynamical;
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
