//
// Created by timfame on 17.04.19.
//

#include <algorithm>
#include <iostream>
#include "big_integer.h"

big_integer:: big_integer() {
    negate = false;
    data = {0};
}

big_integer:: big_integer(int new_val) {
    data = {static_cast<bit>(::abs(new_val))};
    negate = new_val < 0;
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
    if (negate != other.negate) {
        return *this -= (-other);
    }
    bool carry = 0;
    for (size_t i = 0; i < (std::max(data.size(), other.data.size())) || carry; i++) {
        if (i == data.size())
            data.push_back(0);
        data[i] = data[i] + (i < other.data.size() ? other.data[i] : 0) + carry;
        carry = data[i] >= BIT_MAX;
        if (carry) {
            data[i] -= BIT_MAX;
        }
    }
    check_invariant();
    return *this;
}

big_integer& big_integer:: operator -=(big_integer const& other) {
    if (negate != other.negate) {
        return *this += (-other);
    }
    std::vector val = other.data;
    if (*this < other) {
        negate = !negate;
        val = this->data;
        this->data = other.data;
    }
    bool carry = 0;
    for (size_t i = 0; i < val.size() || carry; i++) {
        data[i] -= carry + (i < val.size() ? val[i] : 0);
        carry = data[i] < 0;
        if (carry) {
            data[i] += BIT_MAX;
        }
    }
    while (data.size() > 1 && data.back() == 0) {
        data.pop_back();
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
            long_bit cur = res[i + j] + (long_bit)data[i] * (long_bit)(j < other.data.size() ? other.data[j] : 0) + carry;
            res[i + j] = bit(cur % BIT_MAX);
            carry = bit(cur / BIT_MAX);
        }
    }
    while (res.size() > 1 && res.back() == 0) {
        res.pop_back();
    }
    data = res;
    check_invariant();
    return *this;
}

big_integer big_integer:: div(int other) {
    big_integer res(*this);
    res.negate = res.negate != (other < 0);
    bit carry = 0;
    size_t i = res.data.size();
    while (i > 0) {
        i--;
        long_bit cur = res.data[i] + (long_bit)carry * (long_bit)BIT_MAX;
        res.data[i] = bit(cur / other);
        carry = bit(cur % other);
    }
    while (res.data.size() > 1 && res.data.back() == 0) {
        res.data.pop_back();
    }
    res.check_invariant();
    return res;
}

int big_integer:: mod(int other) {
    big_integer res(*this);
    res.negate = res.negate != (other < 0);
    bit carry = 0;
    size_t i = res.data.size();
    while (i > 0) {
        i--;
        long_bit cur = res.data[i] + (long_bit)carry * (long_bit)BIT_MAX;
        res.data[i] = bit(cur / other);
        carry = bit(cur % other);
    }
    return carry;
}

big_integer& big_integer:: operator /=(big_integer const& other) {
    big_integer divisor(other);
    int normalize_value = divisor.normalize();
    *this *= normalize_value;
    divisor *= normalize_value;
    size_t m = data.size(), n = divisor.data.size();

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
    std::cout << *this << " ";
    make_two_complement();
    std::cout << *this << " ";
    val.make_two_complement();
    for (size_t i = 0; i < std::max(data.size(), val.data.size()); i++) {
        if (i == data.size()) {
            data.push_back(0);
        }
        data[i] = foo(data[i], (i < val.data.size() ? val.data[i] : 0));
    }
   // return_to_sign();
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
        new_data[i + 1] |= data[i - create] >> (BIT_SIZE - shift);
    }
    data = new_data;
    check_invariant();
    return *this;
}

big_integer& big_integer:: operator >>=(int other) {
    if (other < 0) {
        return *this <<= -other;
    }
    if (other > data.size() * BIT_SIZE) {
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
        new_data[i - 1] |= data[i + destroy] << (BIT_SIZE - shift);
    }
    data = new_data;
    if (negate) {
        ++*this;
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
        if (a.negate ? a.data[i] < b.data[i] : a.data[i] > b.data[i]) {
            return true;
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

int big_integer:: normalize() {
    return BIT_MAX / (long_bit)(data.back() + 1);
}

void big_integer:: make_two_complement() {
    if (!negate) {
        return;
    }
    for (size_t i = 0; i < data.size(); i++) {
        data[i] = ~data[i];
    }
    ++*this;
}

void big_integer:: return_to_sign() {
    if (!negate) {
        return;
    }
    --*this;
    for (size_t i = 0; i < data.size(); i++) {
        data[i] = ~data[i];
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