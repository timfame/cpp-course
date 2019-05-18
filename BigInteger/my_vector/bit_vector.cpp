//
// Created by timfame on 18.05.19.
//

#include <memory>
#include <cstdlib>
#include <memory.h>
#include "bit_vector.h"

bit_vector::bit_vector() : is_dynamic(false), _size(0) {}

bit_vector::bit_vector(uint32_t new_val) : is_dynamic(false), _size(1) {
    data.statical[0] = new_val;
}

bit_vector::dynamic::dynamic(size_t s, uint32_t *a) : capacity(s), pointer(a, std::default_delete<uint32_t []>()) {}

bit_vector::bit_vector(bit_vector const &new_val) noexcept : is_dynamic(new_val.is_dynamic), _size(new_val._size) {
    if (is_dynamic)
        new (&data.dynamical) dynamic(new_val.data.dynamical);
    else {
        for (size_t i = 0; i < _size; i++)
            data.statical[i] = new_val.data.statical[i];
    }
}

bit_vector::bit_vector(size_t s, uint32_t val) : is_dynamic(s > STATIC_SIZE), _size(s){
    if (s <= STATIC_SIZE) {
        for (size_t i = 0; i < s; i++)
            data.statical[i] = val;
    } else {
        auto tmp = new uint32_t [s];
        for (size_t i = 0; i < s; i++)
            tmp[i] = val;
        new (&data.dynamical) dynamic(s, tmp);
    }
}

bit_vector::~bit_vector() {
    if (is_dynamic)
        data.dynamical.~dynamic();
}

bit_vector& bit_vector::operator=(bit_vector other) noexcept{
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

uint32_t const& bit_vector::operator[](size_t index) const {
    return begin()[index];
}

uint32_t & bit_vector::operator[](size_t index) {
    delete_ref();
    return begin()[index];
}

void bit_vector::push_back(uint32_t element) {
    delete_ref();
    if (is_dynamic) {
        ensure_capacity();
        begin()[_size] = element;
        _size++;
        return;
    }
    if (_size < STATIC_SIZE)
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
    if (_size <= STATIC_SIZE)
        make_statical();
}

uint32_t & bit_vector::back() {
    delete_ref();
    return begin()[_size - 1];
}

uint32_t const& bit_vector::back() const {
    return begin()[_size - 1];
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
        auto tmp = new uint32_t [new_size * 2];
        std::copy(begin(), begin() + _size, tmp);
        data.dynamical.pointer.reset(tmp, std::default_delete<uint32_t[]>());
    }
}

void bit_vector::make_dynamical() {
    auto tmp = new uint32_t [_size * 2];
    std::copy(begin(), begin() + _size, tmp);
    new (&data.dynamical) dynamic(_size * 2, tmp);
    is_dynamic = true;
}

void bit_vector::make_statical() {
    auto tmp = new uint32_t[_size];
    for (size_t i = 0; i < _size; i++)
        tmp[i] = begin()[i];
    for (size_t i = 0; i < _size; i++)
        data.statical[i] = tmp[i];
    is_dynamic = false;
    delete[] tmp;
}

uint32_t * bit_vector::begin() const {
    return (is_dynamic ? data.dynamical.pointer.get() : const_cast<uint32_t *>(data.statical));
}

void bit_vector::delete_ref() {
    if (is_dynamic && !data.dynamical.pointer.unique()) {
        auto tmp = new uint32_t [data.dynamical.capacity];
        std::copy(begin(), begin() + _size, tmp);
        data.dynamical.pointer.reset(tmp, std::default_delete<uint32_t[]>());
    }
}