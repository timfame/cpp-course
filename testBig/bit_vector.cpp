//
// Created by timfame on 18.05.19.
//

#include <memory>
#include <cstdlib>
#include <memory.h>
#include <iostream>
#include "bit_vector.h"

bit_vector::bit_vector() : is_dynamic(false), _size(0) {}

bit_vector::bit_vector(unsigned new_val) : is_dynamic(false), _size(1) {
    data.statical[0] = new_val;
}

bit_vector::dynamic::dynamic(size_t s, unsigned *a) : capacity(s), pointer(a, std::default_delete<unsigned[]>()) {}

bit_vector::bit_vector(bit_vector const &new_val) : is_dynamic(new_val.is_dynamic), _size(new_val._size){
    if (is_dynamic) {
        auto tmp = new unsigned[new_val.data.dynamical.capacity];
        std::copy(new_val.begin(), new_val.begin() + _size, tmp);
        new (&data.dynamical) dynamic(_size, tmp);
    } else {
        for (size_t i = 0; i < _size; i++)
            data.statical[i] = new_val.data.statical[i];
    }
}

bit_vector::bit_vector(size_t s, unsigned val) : is_dynamic(s > 4), _size(s){
    if (s < 5) {
        for (size_t i = 0; i < s; i++)
            data.statical[i] = val;
    } else {
        auto tmp = new unsigned[s];
        for (size_t i = 0; i < s; i++)
            tmp[i] = val;
        new (&data.dynamical) dynamic(s, tmp);
    }
}

bit_vector::~bit_vector() {
    if (is_dynamic)
        data.dynamical.~dynamic();
}

bit_vector& bit_vector::operator=(bit_vector other) {
    swap(other);
    return *this;
}

void bit_vector::swap(bit_vector &other) {
    std::swap(_size, other._size);
    std::swap(is_dynamic, other.is_dynamic);
    char *tmp[sizeof(data)];
    mempcpy(tmp, &data, sizeof(data));
    mempcpy(&data, &other.data, sizeof(data));
    mempcpy(&other.data, tmp, sizeof(data));
}

unsigned const& bit_vector::operator[](size_t index) const {
    return begin()[index];
}

unsigned& bit_vector::operator[](size_t index) {
    delete_ref();
    return begin()[index];
}

void bit_vector::push_back(unsigned element) {
    delete_ref();
    if (is_dynamic) {
        ensure_capacity();
        begin()[_size] = element;
        _size++;
        return;
    }
    if (_size < 4)
        begin()[_size] = element;
    else {
        make_dynamical();
        ensure_capacity();
        begin()[_size] = element;
    }
    _size++;
}

void bit_vector::pop_back() {
    if (empty())
        return;
    delete_ref();
    _size--;
}

unsigned& bit_vector::back() {
    delete_ref();
    return begin()[_size - 1];
}

unsigned const& bit_vector::back() const {
    return begin()[_size - 1];
}

void bit_vector::resize(size_t new_size) {
    if (!is_dynamic && new_size > 4)
        make_dynamical();
    else
    if (is_dynamic && new_size > data.dynamical.capacity) {
        auto tmp = new unsigned[new_size];
        data.dynamical.capacity = new_size;
        std::copy(begin(), begin() + new_size, tmp);
        data.dynamical.pointer.reset(tmp, std::default_delete<unsigned[]>());
    }
    _size = new_size;
}

size_t bit_vector::size() const{
    return _size;
}

bool bit_vector::empty() const{
    return _size == 0;
}

bool operator==(bit_vector const &a, bit_vector const &b) {
    if (a.size() != b.size())
        return false;
    for (size_t i = 0; i < a.size(); i++)
        if (a[i] != b[i])
            return false;
    return true;
}

void bit_vector::ensure_capacity() {
    size_t new_size = _size + 1;
    if (new_size >= data.dynamical.capacity) {
        data.dynamical.capacity = new_size * 2;
        auto tmp = new unsigned[new_size * 2];
        std::copy(begin(), begin() + _size, tmp);
        data.dynamical.pointer.reset(tmp, std::default_delete<unsigned[]>());
    }
}

void bit_vector::make_dynamical() {
    auto tmp = new unsigned[_size];
    std::copy(begin(), begin() + _size, tmp);
    new (&data.dynamical) dynamic(_size, tmp);
    is_dynamic = true;
}

unsigned* bit_vector::begin() const {
    return (is_dynamic ? data.dynamical.pointer.get() : const_cast<unsigned *>(data.statical));
}

void bit_vector::delete_ref() {
    if (is_dynamic && !data.dynamical.pointer.unique()) {
        auto tmp = new unsigned[data.dynamical.capacity];
        std::copy(begin(), begin() + _size, tmp);
        data.dynamical.pointer.reset(tmp, std::default_delete<unsigned[]>());
    }
}