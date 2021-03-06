//
// Created by timfame on 17.04.19.
//

#ifndef BIGINTEGER_BIG_INTEGER_H
#define BIGINTEGER_BIG_INTEGER_H

#include <string>
#include <cstdlib>
#include <my_vector/bit_vector.h>

struct big_integer {

    big_integer();

    big_integer(int new_val);

    big_integer(uint32_t new_val);

    big_integer(big_integer const& new_val) noexcept;

    ~big_integer() = default;

    explicit big_integer(std::string const& new_val);

    big_integer& operator =(big_integer const& other) noexcept;

    big_integer& operator +=(big_integer const& other);
    big_integer& operator -=(big_integer const& other);
    big_integer& operator *=(big_integer const& other);
    big_integer& operator *=(uint32_t other);
    big_integer& operator *=(int other);
    big_integer& operator /=(big_integer const& other);
    big_integer& operator /=(uint32_t other);
    big_integer& operator /=(int other);
    big_integer& operator %=(big_integer const& other);
    uint32_t operator %=(uint32_t other);
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

   // big_integer div(uint32_t other);
   // uint32_t mod(uint32_t other);
    std::string to_string() const;
    big_integer abs() const;
    big_integer min(big_integer const& other) const;
    uint32_t to_bit() const;

private:
    bit_vector data;
    bool negate;

    template <typename T>
    big_integer& bitwise_operation(big_integer const& other, T foo);

    void swap(big_integer& b);

    uint32_t bit_cast(uint64_t x) const;
    uint64_t long_cast(uint32_t x) const;
    long long sign_cast(uint32_t x) const;

    void make_two_complement();
    void return_to_sign();
    void check_invariant();

    static const big_integer ZERO;
    static const big_integer BASE;
    static const uint32_t BIT_SIZE = 32;
    static const uint64_t BIT_MAX = 1ll << 32;
};

big_integer operator +(big_integer const& a, big_integer const& b);
big_integer operator -(big_integer const& a, big_integer const& b);
big_integer operator *(big_integer const& a, big_integer const& b);
big_integer operator *(big_integer const& a, uint32_t other);
big_integer operator *(big_integer const& a, int other);
big_integer operator /(big_integer const& a, big_integer const& b);
big_integer operator /(big_integer const& a, uint32_t other);
big_integer operator /(big_integer const& a, int other);
big_integer operator %(big_integer const& a, big_integer const& b);
uint32_t operator %(big_integer const& a, uint32_t b);
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
