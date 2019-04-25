//
// Created by timfame on 17.04.19.
//

#ifndef BIGINTEGER_BIG_INTEGER_H
#define BIGINTEGER_BIG_INTEGER_H

#include <vector>
#include <string>
#include <cstdlib>

typedef unsigned int bit;
typedef unsigned long long long_bit;
typedef long long sign_bit;

const bit BIT_SIZE = 32;
const long_bit BIT_MAX = 1ll << 32;
const bit LONG_BIT_SIZE = 64;

struct big_integer {

    big_integer();

    big_integer(int new_val);

    big_integer(bit new_val);

    big_integer(big_integer const& new_val);

    ~big_integer();

    explicit big_integer(std::string const& new_val);

    big_integer& operator =(big_integer const& other);

    big_integer& operator +=(big_integer const& other);
    big_integer& operator -=(big_integer const& other);
    big_integer& operator *=(big_integer const& other);
    big_integer& operator /=(big_integer const& other);
    big_integer& operator %=(big_integer const& other);

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

    big_integer div(bit other);
    bit mod(bit other);
    std::string to_string() const;
    big_integer abs() const;
    big_integer min(big_integer const& other) const;
    bit to_bit() const;

private:
    std::vector <bit> data;
    bool negate;

    template <typename T>
    big_integer& bitwise_operation(big_integer const& other, T foo);

    void swap(big_integer& b);
    bit normalize();
    big_integer pow_base(int i) const;

    bit bit_cast(long_bit x) const;
    long_bit long_cast(bit x) const;
    sign_bit sign_cast(bit x) const;

    void make_two_complement();
    void return_to_sign();
    void check_invariant();

    static const big_integer ZERO;
    static const big_integer BASE;
};

big_integer operator +(big_integer const& a, big_integer const& b);
big_integer operator -(big_integer const& a, big_integer const& b);
big_integer operator *(big_integer const& a, big_integer const& b);
big_integer operator /(big_integer const& a, big_integer const& b);
big_integer operator %(big_integer const& a, big_integer const& b);

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
