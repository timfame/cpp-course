//
// Created by timfame on 17.04.19.
//

#include <algorithm>
#include <iostream>
#include "big_integer.h"

big_integer:: big_integer() : data({0}), negate(false) {}

big_integer:: big_integer(int new_val) : negate(new_val < 0){
    if (new_val == static_cast<int>(-(BIT_MAX / 2))) {
        data = bit_cast(BIT_MAX / 2);
    } else {
        data = static_cast<unsigned>(::abs(new_val));
    }
}

big_integer:: big_integer(uint32_t new_val) : data(new_val), negate(false){}

big_integer:: big_integer(big_integer const &new_val) noexcept {
    data = new_val.data;
    negate = new_val.negate;
}

big_integer:: big_integer(std::string const &new_val) {
    big_integer value(0);
    for (size_t i = new_val[0] == '-'; i < new_val.length(); i++) {
        value *= 10;
        value += new_val[i] - '0';
    }
    if (new_val[0] == '-') {
        value = -value;
    }
    data = value.data;
    negate = value.negate;
}

big_integer& big_integer:: operator = (big_integer const& other) noexcept {
    big_integer(other).swap(*this);
    return *this;
}

big_integer& big_integer:: operator +=(big_integer const& other) {
    if (negate != other.negate && other != ZERO) {
        return *this -= (-other);
    }
    bool carry = 0;
    for (size_t i = 0; i < (std::max(data.size(), other.data.size())) || carry; i++) {
        if (i == data.size())
            data.push_back(0);
        uint64_t cur = long_cast(data[i]) + (i < other.data.size() ? other.data[i] : 0) + carry;
        data[i] = bit_cast(cur);
        carry = cur >= BIT_MAX;
    }
    check_invariant();
    return *this;
}

big_integer& big_integer:: operator -=(big_integer const& other) {
    if (negate != other.negate && other != ZERO) {
        return *this += (-other);
    }
    bit_vector val = other.data;
    if (::abs(*this) < ::abs(other)) {
        negate = !negate;
        val = data;
        data = other.data;
    }
    bool carry = 0;
    for (size_t i = 0; i < val.size() || carry; i++) {
        long long cur = sign_cast(data[i]) - carry - (i < val.size() ? val[i] : 0);
        carry = cur < 0;
        data[i] = bit_cast(cur + (carry ? BIT_MAX : 0));
    }
    check_invariant();
    return *this;
}

big_integer& big_integer:: operator *=(big_integer const& other) {
    negate = negate != other.negate;
    bit_vector res(data.size() + other.data.size(), 0);
    for (size_t i = 0; i < data.size(); i++) {
        uint32_t carry = 0;
        for (size_t j = 0; j < other.data.size() || carry; j++) {
            uint64_t cur = long_cast(data[i]) * (j < other.data.size() ? other.data[j] : 0) + carry + res[i + j];
            res[i + j] = bit_cast(cur % BIT_MAX);
            carry = bit_cast(cur / BIT_MAX);
        }
    }
    std::swap(data, res);
    check_invariant();
    return *this;
}

big_integer& big_integer:: operator *=(uint32_t other) {
    uint32_t carry = 0;
    for (size_t i = 0; i < data.size() || carry; i++) {
        if (i == data.size()) {
            data.push_back(0);
        }
        uint64_t cur = long_cast(data[i]) * other + carry;
        data[i] = bit_cast(cur % BIT_MAX);
        carry = bit_cast(cur / BIT_MAX);
    }
    check_invariant();
    return *this;
}

big_integer& big_integer:: operator *=(int other) {
    negate = negate != (other < 0);
    uint32_t multiplier = bit_cast(BIT_MAX / 2);
    if (other != static_cast<int>(-(BIT_MAX / 2))) {
        multiplier = static_cast<unsigned>(::abs(other));
    }
    *this *= multiplier;
    return *this;
}

big_integer& big_integer:: operator /=(uint32_t other) {
    uint32_t carry = 0;
    size_t i = data.size();
    while (i > 0) {
        i--;
        uint64_t cur = long_cast(carry) * BIT_MAX + data[i];
        data[i] = bit_cast(cur / other);
        carry = bit_cast(cur % other);
    }
    check_invariant();
    return *this;
}

big_integer& big_integer:: operator /=(int other) {
    negate = negate != (other < 0);
    uint32_t divisor = bit_cast(BIT_MAX / 2);
    if (other != static_cast<int>(-(BIT_MAX / 2))) {
        divisor = static_cast<unsigned>(::abs(other));
    }
    *this /= divisor;
    return *this;
}

big_integer& big_integer:: operator /=(big_integer const& other) {
    bool neg = negate != other.negate;
    big_integer divisor(other);
    divisor = ::abs(divisor), *this = ::abs(*this);
    if (*this < divisor) {
        return *this = 0;
    }
    uint32_t normalize_value = bit_cast(BIT_MAX / long_cast(divisor.data.back() + 1));
    *this *= normalize_value;
    divisor *= normalize_value;
    size_t m = data.size(), n = divisor.data.size();
    m -= n;
    bit_vector quotient(m + 1, 0);
    uint32_t last_bit = divisor.data.back();
    divisor <<= BIT_SIZE * m;
    if (*this >= divisor) {
        quotient[m] = 1;
        *this -= divisor;
    }
    size_t i = m;
    while (i > 0) {
        i--;
        divisor >>= BIT_SIZE;
        uint32_t first = (n + i - 1 < data.size() ? data[n + i - 1] : 0);
        uint32_t second = (n + i < data.size() ? data[n + i] : 0);
        big_integer cur = (first + BASE * second) / last_bit;
        quotient[i] = ::min(cur, BASE - 1).to_bit();
        *this -= quotient[i] * divisor;
        while (*this < 0) {
            quotient[i]--;
            *this += divisor;
        }
    }
    std::swap(data, quotient);
    negate = neg;
    check_invariant();
    return *this;
}

big_integer& big_integer:: operator %=(big_integer const& other) {
    return *this -= *this / other * other;
}

uint32_t big_integer:: operator %=(uint32_t other) {
    return (*this -= *this / other * other).to_bit();
}

int big_integer:: operator %=(int other) {
    *this -= *this / other * other;
    int res = static_cast<int>(to_bit());
    if (negate) {
        res = -res;
    }
    return res;
}

big_integer& big_integer:: operator &=(big_integer const& other) {
    return bitwise_operation(other, std::bit_and<unsigned>());
}

big_integer& big_integer:: operator |=(big_integer const& other) {
    return bitwise_operation(other, std::bit_or<unsigned>());
}

big_integer& big_integer:: operator ^=(big_integer const& other) {
    return bitwise_operation(other, std::bit_xor<unsigned>());
}

template <typename T>
big_integer& big_integer:: bitwise_operation(big_integer const& other, T foo) {
    big_integer val(other);
    make_two_complement();
    val.make_two_complement();
    for (size_t i = 0; i < std::max(data.size(), val.data.size()); i++) {
        if (i == data.size()) {
            data.push_back(0);
        }
        data[i] = foo(data[i], (i < val.data.size() ? val.data[i] : 0));
    }
    negate = foo(negate, val.negate);
    return_to_sign();
    check_invariant();
    return *this;
}


big_integer& big_integer:: operator <<=(int other) {
    if (other < 0) {
        return *this >>= -other;
    }
    size_t create = other / BIT_SIZE;
    size_t shift = other % BIT_SIZE;
    bit_vector new_data(data.size() + create + 1, 0);
    size_t i = 0;
    for (; i < create; i++) {
        new_data[i] = 0;
    }
    for (; i < new_data.size() - 1; i++) {
        new_data[i] |= data[i - create] << shift;
        new_data[i + 1] |= (shift > 0 ? data[i - create] >> (BIT_SIZE - shift) : 0);
    }
    std::swap(data, new_data);
    check_invariant();
    return *this;
}

big_integer& big_integer:: operator >>=(int other) {
    if (other < 0) {
        return *this <<= -other;
    }
    if (other > int(data.size() * BIT_SIZE)) {
        return *this = ZERO;
    }
    if (negate) {
        --*this;
    }
    size_t destroy = other / BIT_SIZE;
    size_t shift = other % BIT_SIZE;
    bit_vector new_data(data.size() - destroy, 0);
    new_data[0] = data[destroy] >> shift;
    for (size_t i = 1; i < new_data.size(); i++) {
        new_data[i] |= data[i + destroy] >> shift;
        new_data[i - 1] |= (shift > 0 ? data[i + destroy] << (BIT_SIZE - shift) : 0);
    }
    std::swap(data, new_data);
    if (negate) {
        --*this;
    }
    check_invariant();
    return *this;
}

big_integer big_integer:: operator +() const {
    return *this;
}

big_integer big_integer:: operator -() const {
    big_integer temp(*this);
    temp.negate = !temp.negate;
    temp.check_invariant();
    return temp;
}

big_integer big_integer:: operator ~() const {
    return -*this - 1;
}

big_integer& big_integer:: operator ++() {
    return *this += 1;
}

big_integer big_integer:: operator ++(int) {
    big_integer temp(*this);
    ++*this;
    return temp;
}

big_integer& big_integer:: operator --() {
    return *this -= 1;
}

big_integer big_integer:: operator --(int) {
    big_integer temp(*this);
    --*this;
    return temp;
}

big_integer operator +(big_integer const& a, big_integer const& b) {
    return big_integer(a) += b;
}

big_integer operator -(big_integer const& a, big_integer const& b) {
    return big_integer(a) -= b;
}

big_integer operator *(big_integer const& a, big_integer const& b) {
    return big_integer(a) *= b;
}

big_integer operator *(big_integer const& a, uint32_t b) {
    return big_integer(a) *= b;
}

big_integer operator *(big_integer const& a, int b) {
    return big_integer(a) *= b;
}

big_integer operator /(big_integer const& a, big_integer const& b) {
    return big_integer(a) /= b;
}

big_integer operator /(big_integer const& a, uint32_t b) {
    return big_integer(a) /= b;
}

big_integer operator /(big_integer const& a, int b) {
    return big_integer(a) /= b;
}

big_integer operator %(big_integer const& a, big_integer const& b) {
    return big_integer(a) %= b;
}

uint32_t operator %(big_integer const& a, uint32_t b) {
    return big_integer(a) %= b;
}

int operator %(big_integer const& a, int b) {
    return big_integer(a) %= b;
}

big_integer operator &(big_integer const& a, big_integer const& b) {
    return big_integer(a) &= b;
}

big_integer operator |(big_integer const& a, big_integer const& b) {
    return big_integer(a) |= b;
}

big_integer operator ^(big_integer const& a, big_integer const& b) {
    return big_integer(a) ^= b;
}

big_integer operator <<(big_integer const& a, int b) {
    return big_integer(a) <<= b;
}

big_integer operator >>(big_integer const& a, int b) {
    return big_integer(a) >>= b;
}

bool operator ==(big_integer const& a, big_integer const& b) {
    return a.negate == b.negate && a.data == b.data;
}

bool operator !=(big_integer const& a, big_integer const& b) {
    return !(a == b);
}

bool operator >(big_integer const& a, big_integer const& b) {
    if (a.negate != b.negate) {
        return !a.negate;
    }
    if (a.data.size() != b.data.size()) {
        return a.data.size() > b.data.size();
    }
    size_t i = a.data.size();
    while (i > 0) {
        i--;
        if (a.data[i] != b.data[i]) {
            return (a.negate ? a.data[i] < b.data[i] : a.data[i] > b.data[i]);
        }
    }
    return false;
}

bool operator <(big_integer const& a, big_integer const& b) {
    return (b > a);
}

bool operator >=(big_integer const& a, big_integer const& b) {
    return !(a < b);
}

bool operator <=(big_integer const& a, big_integer const& b) {
    return !(a > b);
}

void big_integer:: swap(big_integer& b) {
    std::swap(data, b.data);
    std::swap(negate, b.negate);
}

void big_integer:: check_invariant() {
    while (data.size() > 1 && data.back() == 0) {
        data.pop_back();
    }
    if (data.size() == 1 && data[0] == 0) {
        negate = false;
    }
}

void big_integer:: make_two_complement() {
    if (!negate) {
        return;
    }
    for (size_t i = 0; i < data.size(); i++)
        data[i] = ~data[i];
    --*this;  // So, it is +1, but we have negative number => to inc abs(number) use --
}

void big_integer:: return_to_sign() {
    if (!negate) {
        return;
    }
    ++*this;
    for (size_t i = 0; i < data.size(); i++)
        data[i] = ~data[i];
}

big_integer big_integer:: abs() const{
    big_integer temp(*this);
    temp.negate = false;
    return temp;
}

big_integer abs(big_integer const& b) {
    return b.abs();
}

big_integer big_integer:: min(big_integer const& other) const{
    return (*this > other ? other : *this);
}

big_integer min(big_integer const& a, big_integer const& b) {
    return a.min(b);
}

uint32_t big_integer:: to_bit() const {
    return data[0];
}

uint32_t big_integer:: bit_cast(uint64_t x) const {
    return static_cast<unsigned>(x);
}

uint64_t big_integer:: long_cast(uint32_t x) const {
    return static_cast<uint64_t >(x);
}

long long big_integer:: sign_cast(uint32_t x) const {
    return static_cast<long long>(x);
}

std::string big_integer:: to_string() const{
    std::string res;
    big_integer cur(::abs(*this));
    do {
        res += char((cur % 10) + 48);
        cur /= 10;
    } while (::abs(cur) > 0);
    if (negate) {
        res += "-";
    }
    std::reverse(res.begin(), res.end());
    return res;
}

std::string to_string(big_integer const& b) {
    return b.to_string();
}

std::ostream& operator <<(std::ostream& out, big_integer const& a) {
    return out << to_string(a);
}

const big_integer big_integer:: ZERO = 0;

const big_integer big_integer:: BASE = big_integer(1) << BIT_SIZE;