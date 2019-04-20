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

const bit BIT_SIZE = 32;
const long_bit BIT_MAX = 1ll << 32;
const bit LONG_BIT_SIZE = 64;

struct big_integer {

    big_integer();

    big_integer(int new_val);

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

    big_integer div(int other);
    int mod(int other);
    std::string to_string() const;
    big_integer abs() const;

private:
    std::vector <bit> data;
    bool negate;

    template <typename T>
    big_integer& bitwise_operation(big_integer const& other, T foo);

    void swap(big_integer& b);
    void check_invariant();
    int normalize();
    void make_two_complement();
    void return_to_sign();

    static const big_integer ZERO;
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

std::string to_string(big_integer const& b);
std::ostream& operator <<(std::ostream& out, big_integer const& a);

#endif //BIGINTEGER_BIG_INTEGER_H
