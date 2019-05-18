//
// Created by timfame on 17.04.19.
//

#ifndef BIGINTEGER_BIG_INTEGER_H
#define BIGINTEGER_BIG_INTEGER_H

#include <string>
#include <cstdlib>
#include "bit_vector.h"

struct big_integer {

    big_integer();

    big_integer(int new_val);

    big_integer(unsigned new_val);

    big_integer(big_integer const& new_val);

    ~big_integer() = default;

    explicit big_integer(std::string const& new_val);

    big_integer& operator =(big_integer const& other);

    big_integer& operator +=(big_integer const& other);
    big_integer& operator -=(big_integer const& other);
    big_integer& operator *=(big_integer const& other);
    big_integer& operator *=(unsigned other);
    big_integer& operator *=(int other);
    big_integer& operator /=(big_integer const& other);
    big_integer& operator /=(unsigned other);
    big_integer& operator /=(int other);
    big_integer& operator %=(big_integer const& other);
    unsigned operator %=(unsigned other);
    int operator %=(int other);

    big_integer& operator &=(big_integer const& other);
    big_integer& operator |=(big_integer const& other);
    big_integer& operator ^=(big_integer const& other);
    big_integer& operator <<=(int other);
    big_integer& operator >>=(int other);

    big_integer operator +() const;
    big_integer operator -() const;
    big_integer operator ~() const;

    big_integer& operator ++();
    big_integer operator ++(int);
    big_integer& operator --();
    big_integer operator --(int);

    friend bool operator ==(big_integer const& a, big_integer const& b);
    friend bool operator >(big_integer const& a, big_integer const& b);

    // big_integer div(unsigned other);
    // unsigned mod(unsigned other);
    std::string to_string() const;
    big_integer abs() const;
    big_integer min(big_integer const& other) const;
    unsigned to_bit() const;

private:
    bit_vector data;
    bool negate;

    template <typename T>
    big_integer& bitwise_operation(big_integer const& other, T foo);

    void swap(big_integer& b);

    unsigned bit_cast(unsigned long long x) const;
    unsigned long long long_cast(unsigned x) const;
    long long sign_cast(unsigned x) const;

    void make_two_complement();
    void return_to_sign();
    void check_invariant();

    static const big_integer ZERO;
    static const big_integer BASE;
    static const unsigned BIT_SIZE = 32;
    static const unsigned long long BIT_MAX = 1ll << 32;
};

big_integer operator +(big_integer const& a, big_integer const& b);
big_integer operator -(big_integer const& a, big_integer const& b);
big_integer operator *(big_integer const& a, big_integer const& b);
big_integer operator *(big_integer const& a, unsigned other);
big_integer operator *(big_integer const& a, int other);
big_integer operator /(big_integer const& a, big_integer const& b);
big_integer operator /(big_integer const& a, unsigned other);
big_integer operator /(big_integer const& a, int other);
big_integer operator %(big_integer const& a, big_integer const& b);
unsigned operator %(big_integer const& a, unsigned b);
int operator %(big_integer const& a, int b);

big_integer operator &(big_integer const& a, big_integer const& b);
big_integer operator |(big_integer const& a, big_integer const& b);
big_integer operator ^(big_integer const& a, big_integer const& b);

big_integer operator <<(big_integer const& a, int b);
big_integer operator >>(big_integer const& a, int b);

bool operator ==(big_integer const& a, big_integer const& b);
bool operator !=(big_integer const& a, big_integer const& b);
bool operator >(big_integer const& a, big_integer const& b);
bool operator <(big_integer const& a, big_integer const& b);
bool operator >=(big_integer const& a, big_integer const& b);
bool operator <=(big_integer const& a, big_integer const& b);

big_integer abs(big_integer const& b);
big_integer min(big_integer const& a, big_integer const& b);

std::string to_string(big_integer const& b);
std::ostream& operator <<(std::ostream& out, big_integer const& a);

#endif //BIGINTEGER_BIG_INTEGER_H