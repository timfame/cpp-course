//
// Created by timfame on 17.04.19.
//

#ifndef BIGINTEGER_BIG_INTEGER_H
#define BIGINTEGER_BIG_INTEGER_H

#include <vector>
#include <string>
#include <cstdlib>

typedef unsigned int bit;

const int BIT_SIZE = 32;

class big_integer {

    big_integer();

    big_integer(int new_val);

    big_integer(big_integer const& new_val);

    ~big_integer();

    explicit big_integer(std::string const& new_val);

    big_integer & operator =(big_integer const& val);

    big_integer & operator +=(big_integer const& val);
    big_integer & operator -=(big_integer const& val);
    big_integer & operator *=(big_integer const& val);
    big_integer & operator /=(big_integer const& val);
    big_integer & operator %=(big_integer const& val);

    big_integer & operator &=(big_integer const& val);
    big_integer & operator |=(big_integer const& val);
    big_integer & operator ^=(big_integer const& val);

    big_integer operator +() const;
    big_integer operator -() const;
    big_integer operator ~() const;

    big_integer operator +(big_integer a, big_integer const& b);

private:
    std::vector <bit> data;
    bool negate;
};


#endif //BIGINTEGER_BIG_INTEGER_H
