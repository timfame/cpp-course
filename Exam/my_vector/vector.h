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

        V const& operator*() const {
            return *ptr;
        }

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

    vector() noexcept : info(0) {};

    vector(vector const& other) : info(other.info) {
        if (is_dynamic()) {
            other.inc_refs();
            _data.dynamical.ptr = other._data.dynamical.ptr;
        } else {
            new (&_data.statical) T(other._data.statical[0]);
        }
    };

    ~vector() noexcept {
        if (is_dynamic()) {
            dec_refs();
            if (get_refs() == 0)
                _data.dynamical.~dynamic();
        } else if (size() > 0)
            _data.statical[0].~T();
    }

    template <class InputIterator>
    vector(InputIterator first, InputIterator last) : info(0){
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
                if (size() > 0)
                    _data.statical[0].~T();
            other.inc_refs();
            _data.dynamical.ptr = other._data.dynamical.ptr;
            set_dynamic();
        } else {
            if (is_dynamic())
                delete_dynamic();
            else
                if (size() > 0)
                    _data.statical[0].~T();
            new (&_data.statical) T(other._data.statical[0]);
            set_static();
        }
        set_size(other.size());
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
        return data()[size() - 1];
    }

    T& back() {
        delete_ref();
        return data()[size() - 1];
    }

    void push_back(T const& element) {
        delete_ref();
        T tmp(element);
        if (is_dynamic()) {
            ensure_capacity(tmp);
            //new (data() + size()) T(tmp);
        } else {
            if (empty()) {
                new (&_data.statical) T(tmp);
            } else {
                make_dynamical();
                new (data() + size()) T(tmp);
            }
        }
        inc_size();
    }

    void pop_back() {
        if (empty()) {
            throw std::runtime_error("Pop from empty vector");
        }
        delete_ref();
        if (is_dynamic() && size() <= 2)
            make_statical();
        dec_size();
    }

    T* data() const {
        return (is_dynamic() ? _data.dynamical.begin() : const_cast<T *>(_data.statical));
    }

    iterator begin() noexcept {
        return iterator(data());
    }

    iterator end() noexcept  {
        return iterator(data() + size());
    }

    const_iterator begin() const noexcept {
        return const_iterator(data());
    }

    const_iterator end() const noexcept {
        return const_iterator(data() + size());
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
        return size() == 0;
    }

    size_t size() const noexcept {
        return (is_dynamic() ? _data.dynamical.get_info(0) : (info >> 1u) & 1u);
    }

    void reserve(size_t n) {
        if (is_dynamic() && n < size())
            return;
        if (!is_dynamic() && n > 1) {
            make_dynamical();
        }
        delete_ref();
        new_dynamical(size(), n, get_refs());
    }

    size_t capacity() const {
        return (is_dynamic() ? _data.dynamical.get_info(1) : 1);
    }

    void shrink_to_fit() {
        reserve(size());
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
               set_size(n);
               return;
           }
       }
       if (n == 0)
           delete_dynamic(),
           set_static();
       else if (n == 1)
           make_statical();
       else {
           dynamic tmp(n, n * 2, get_refs());
           size_t sz = size();
           for (size_t i = 0; i < std::min(sz, n); ++i)
               new (tmp.begin() + i) T(data()[i]);
           T temp(val);
           for (size_t i = sz; i < n; ++i)
               new (tmp.begin() + i) T(temp);
           delete_dynamic();
           _data.dynamical = tmp;
       }
       set_size(n);
    }

    void clear() {
        if (is_dynamic())
            delete_dynamic();
        else
            if (size() > 0)
                _data.statical[0].~T();
        set_static();
        set_size(0);
    }

    friend void swap(vector& a, vector& b) {
        size_t as = a.size(), bs = b.size();
        if (a.is_dynamic() && b.is_dynamic())
            std::swap(a._data.dynamical.ptr, b._data.dynamical.ptr);
        else if (a.is_dynamic() && !b.is_dynamic())
            swap_ds(a, b);
        else if (!a.is_dynamic() && b.is_dynamic())
            swap_ds(b, a);
        else
            swap_ss(a, b);
        a.set_size(bs);
        b.set_size(as);
    }

    friend bool operator==(vector const& a, vector const& b) {
        if (a.is_dynamic() != b.is_dynamic() || a.size() != b.size())
            return false;
        for (size_t i = 0; i < a.size(); ++i)
            if (a.data()[i] != b.data()[i])
                return false;
        return true;
    }

    friend bool operator<(vector const& a, vector const& b) {
        if (a.size() != b.size())
            return a.size() < b.size();
        for (size_t i = 0; i < a.size(); ++i) {
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
    uint8_t info;
    struct dynamic {
        void* ptr;

        dynamic() : ptr(nullptr) {};

        dynamic(size_t sz, size_t c, size_t r) : ptr(::operator new(3 * sizeof(size_t) + c * sizeof(T))) {
            get_info(0) = sz;
            get_info(1) = c;
            get_info(2) = r;
        }

        explicit dynamic(dynamic const* other) : ptr(other->ptr) {}

        dynamic& operator=(dynamic const& other) {
            ptr = ::operator new(3 * sizeof(size_t) + other.get_info(1) * sizeof(T));
            get_info(0) = other.get_info(0);
            get_info(1) = other.get_info(1);
            get_info(2) = other.get_info(2);
            for (size_t i = 0; i < get_info(0); ++i)
                new (begin() + i) T(other.begin()[i]);
            return *this;
        }

        ~dynamic() {
            for (size_t i = 0; i < get_info(0); ++i)
                begin()[i].~T();
            ::operator delete(ptr);
        }

        bool unique() const {
            return get_info(2) == 1;
        }

        size_t const get_info(size_t index) const {
            return *((size_t*)(ptr) + index);
        }

        size_t& get_info(size_t index) {
            return *((size_t*)(ptr) + index);
        }

        T* begin() const {
            return ((T*)((size_t*)(ptr) + 3));
        }
    };
    union some {
        T statical[1];
        dynamic dynamical;
        some(){};
        ~some(){};
    } _data;

    bool is_dynamic() const {
        return info & 1u;
    }

    void set_dynamic() {
        if ((info & 1u) == 0)
            ++info;
    }

    void set_static() {
        if ((info & 1u) == 1)
            --info;
    }

    void set_size(size_t value) {
        if (is_dynamic())
            _data.dynamical.get_info(0) = value;
        else
            info = (value << 1u) + (info & 1u);
    }

    void inc_size() {
        if (is_dynamic())
            ++_data.dynamical.get_info(0);
        else
            info = (size() + 1) << 1;
    }

    void dec_size() {
        if (is_dynamic())
            --_data.dynamical.get_info(0);
        else
            info = (size() - 1) << 1;
    }

    size_t get_refs() const {
        return _data.dynamical.get_info(2);
    }

    void inc_refs() const {
        ++(*((size_t*)(_data.dynamical.ptr) + 2));
    }

    void dec_refs() const {
        --(*((size_t*)(_data.dynamical.ptr) + 2));
    }

    friend void swap_ds(vector& a, vector& b) {
        if (b.size() > 0) {
            T tmp(b._data.statical[0]);
            b._data.statical[0].~T();
            b._data.dynamical.ptr = a._data.dynamical.ptr;
            new (&a._data.statical) T(tmp);
        } else
            b._data.dynamical.ptr = a._data.dynamical.ptr;
        a.set_static();
        b.set_dynamic();
    };

    friend void swap_ss(vector& a, vector& b) {
        if (a.size() == 1 && b.size() == 1) {
            std::swap(a._data.statical, b._data.statical);
        } else if (a.size() == 1 && b.size() == 0) {
            new (&b._data.statical) T(a._data.statical[0]);
            a._data.statical[0].~T();
        } else if (a.size() == 0 && b.size() == 1) {
            new (&a._data.statical) T(b._data.statical[0]);
            b._data.statical[0].~T();
        }
    }

    void new_dynamical(size_t n, size_t cap, size_t ref) {
        dynamic tmp(n, cap, ref);
        for (size_t i = 0; i < n; ++i)
            new (tmp.begin() + i) T(data()[i]);
        delete_dynamic();
        _data.dynamical = tmp;
    }

    void make_dynamical() {
        try {
            dynamic tmp(size(), 4, 1);
            if (size() > 0) {
                new(tmp.begin()) T(_data.statical[0]);
                _data.statical[0].~T();
            }
            _data.dynamical = tmp;
        } catch (...) {
            throw;
        }
        set_dynamic();
    }

    void make_statical() {
        T elem(*data());
        size_t sz = size();
        delete_dynamic();
        new (&_data.statical) T(elem);
        set_static();
        set_size(sz);
    }

    void ensure_capacity(T const& element) {
        size_t new_size = size() + 1;
        if (new_size >= capacity()) {
            //new_dynamical(size(), new_size * 2, get_refs());
            try {
                dynamic tmp(size(), new_size * 2, get_refs());
                for (size_t i = 0; i < size(); ++i)
                    new(tmp.begin() + i) T(data()[i]);
                new(tmp.begin() + size()) T(element);
                _data.dynamical = tmp;
            } catch(...) {
                throw;
            }
        }
    }

    void delete_ref() {
        if (is_dynamic() && !_data.dynamical.unique()) {
            dynamic tmp(size(), capacity(), 1);
            for (size_t i = 0; i < size(); ++i)
                new (tmp.begin() + i) T(data()[i]);
            dec_refs();
            _data.dynamical = tmp;
        }
    }

    void delete_dynamic() {
        delete_ref();
        _data.dynamical.~dynamic();
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
