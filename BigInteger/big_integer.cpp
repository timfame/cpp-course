//
// Created by timfame on 17.04.19.
//

#include <algorithm>
#include <iostream>
#include <vector>
#include "big_integer.h"

big_integer:: big_integer() {
    negate = false;
    data = {0};
}

big_integer:: big_integer(int new_val) {
    negate = new_val < 0;
    if (new_val == static_cast<int>(-(BIT_MAX / 2))) {
        data = {BIT_MAX / 2};
    } else {
        data = {static_cast<bit>(::abs(new_val))};
    }
}

big_integer:: big_integer(bit new_val) {
    data = {new_val};
    negate = false;
}

big_integer:: big_integer(big_integer const &new_val) {
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

big_integer:: ~big_integer() {
    data.clear();
}

big_integer& big_integer:: operator = (big_integer const& other) {
    if (*this != other) {
        big_integer(other).swap(*this);
    }
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
        long_bit cur = long_cast(data[i]) + (i < other.data.size() ? other.data[i] : 0) + carry;
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
    std::vector <bit> val = other.data;
    if (::abs(*this) < ::abs(other)) {
        negate = !negate;
        val = data;
        data = other.data;
    }
    bool carry = 0;
    for (size_t i = 0; i < val.size() || carry; i++) {
        sign_bit cur = sign_cast(data[i]) - carry - (i < val.size() ? val[i] : 0);
        carry = cur < 0;
        data[i] = bit_cast(cur + (carry ? BIT_MAX : 0));
    }
    check_invariant();
    return *this;
}

big_integer& big_integer:: operator *=(big_integer const& other) {
    negate = negate != other.negate;
    std:: vector <bit> res(data.size() + other.data.size());
    for (size_t i = 0; i < data.size(); i++) {
        bit carry = 0;
        for (size_t j = 0; j < other.data.size() || carry; j++) {
            long_bit cur = long_cast(data[i]) * (j < other.data.size() ? other.data[j] : 0) + carry + res[i + j];
            res[i + j] = bit_cast(cur % BIT_MAX);
            carry = bit_cast(cur / BIT_MAX);
        }
    }
    data = res;
    check_invariant();
    return *this;
}

big_integer big_integer:: div(bit other) {
    big_integer res(*this);
    bit carry = 0;
    size_t i = res.data.size();
    while (i > 0) {
        i--;
        long_bit cur = long_cast(carry) * BIT_MAX + res.data[i];
        res.data[i] = bit_cast(cur / other);
        carry = bit_cast(cur % other);
    }
    res.check_invariant();
    return res;
}

bit big_integer:: mod(bit other) {
    return (*this - this->div(other) * other).to_bit();
}

big_integer& big_integer:: operator /=(big_integer const& other) {
    bool neg = negate != other.negate;
    big_integer divisor(other);
    divisor = ::abs(divisor), *this = ::abs(*this);
    if (divisor.data.size() == 1) {
        *this = this->div(divisor.to_bit());
        negate = neg;
        return *this;
    }
    if (*this < divisor) {
        return *this = 0;
    }
    if (*this < 2 * divisor) {
        return *this = (negate ? -1 : 1);
    }
    bit normalize_value = BIT_MAX / long_cast(divisor.data.back() + 1);
    *this *= normalize_value;
    divisor *= normalize_value;
    size_t m = data.size(), n = divisor.data.size();
    m -= n;
    std:: vector <bit> quotient(m + 1, 0);
    bit last_bit = divisor.data.back();
    divisor <<= BIT_SIZE * m;
    if (*this >= divisor) {
        quotient[m] = 1;
        *this -= divisor;
    }
    size_t i = m;
    while (i > 0) {
        i--;
        divisor >>= BIT_SIZE;
        bit first = (n + i - 1 < data.size() ? data[n + i - 1] : 0);
        bit second = (n + i < data.size() ? data[n + i] : 0);
        big_integer cur = (first + BASE * second).div(last_bit);
        quotient[i] = ::min(cur, BASE - 1).to_bit();
        *this -= quotient[i] * divisor;
        while (*this < 0) {
            quotient[i]--;
            *this += divisor;
        }
        if (*this == ZERO) {
            break;
        }
    }
    data = quotient;
    negate = neg;
    check_invariant();
    return *this;
}

big_integer& big_integer:: operator %=(big_integer const& other) {
    return *this -= *this / other * other;
}

big_integer& big_integer:: operator &=(big_integer const& other) {
    return bitwise_operation(other, [](bit a, bit b) { return a & b;});
}

big_integer& big_integer:: operator |=(big_integer const& other) {
    return bitwise_operation(other, [](bit a, bit b) { return a | b;});
}

big_integer& big_integer:: operator ^=(big_integer const& other) {
    return bitwise_operation(other, [](bit a, bit b) { return a ^ b;});
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
    std:: vector <bit> new_data(data.size() + create + 1, 0);
    size_t i = 0;
    for (; i < create; i++) {
        new_data[i] = 0;
    }
    for (; i < new_data.size() - 1; i++) {
        new_data[i] |= data[i - create] << shift;
        new_data[i + 1] |= (shift > 0 ? data[i - create] >> (BIT_SIZE - shift) : 0);
    }
    data = new_data;
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
    std:: vector <bit> new_data(data.size() - destroy, 0);
    new_data[0] = data[destroy] >> shift;
    for (size_t i = 1; i < new_data.size(); i++) {
        new_data[i] |= data[i + destroy] >> shift;
        new_data[i - 1] |= (shift > 0 ? data[i + destroy] << (BIT_SIZE - shift) : 0);
    }
    data = new_data;
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

big_integer operator /(big_integer const& a, big_integer const& b) {
    return big_integer(a) /= b;
}

big_integer operator %(big_integer const& a, big_integer const& b) {
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

bit big_integer:: normalize() {
    return BIT_MAX / (long_bit)(data.back() + 1);
}

big_integer big_integer:: pow_base(int i) const {
    return big_integer(1) << (BIT_SIZE * i);
}

void big_integer:: make_two_complement() {
    if (!negate) {
        return;
    }
    for (unsigned int & i : data) {
        i = ~i;
    }
    --*this;  // So, it is +1, but we have negative number => to inc abs(number) use --
}

void big_integer:: return_to_sign() {
    if (!negate) {
        return;
    }
    ++*this;
    for (unsigned int & i : data) {
        i = ~i;
    }
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

bit big_integer:: to_bit() const {
    return data[0];
}

bit big_integer:: bit_cast(long_bit x) const {
    return static_cast<bit>(x);
}

long_bit big_integer:: long_cast(bit x) const {
    return static_cast<long_bit>(x);
}

sign_bit big_integer:: sign_cast(bit x) const {
    return static_cast<sign_bit>(x);
}

std::string big_integer:: to_string() const{
    std::string res;
    big_integer cur(*this);
    do {
        res += char(cur.mod(10) + 48);
        cur = cur.div(10);
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