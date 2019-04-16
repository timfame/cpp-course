//
// Created by timfame on 17.04.19.
//

#include "big_integer.h"

big_integer:: big_integer() {
    negate = false;
    data = {0};
}

big_integer:: big_integer(int new_val) {
    data = {static_cast<unsigned int>(abs(new_val))};
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
    this->data = value.data;
    this->negate = value.negate;
}

big_integer:: ~big_integer() {
    data.clear();
}

big_integer& big_integer:: operator = (big_integer const& val) {
    this->data = val.data;
    this->negate = val.negate;
}

big_integer& big_integer:: operator +=(big_integer const& val) {
    return this + val;
}

big_integer& big_integer:: operator -=(big_integer const& val) {
    return this - val;
}

big_integer& big_integer:: operator *=(big_integer const& val) {
    return this * val;
}

big_integer& big_integer:: operator /=(big_integer const& val) {
    return this / val;
}

big_integer& big_integer:: operator %=(big_integer const& val) {
    return this % val;
}

big_integer& big_integer:: operator &=(big_integer const& val) {
    return this & val;
}

big_integer& big_integer:: operator |=(big_integer const& val) {
    return this | val;
}

big_integer& big_integer:: operator ^=(big_integer const& val) {
    return this ^ val;
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
    return -(*this) - 1;
}

big_integer big_integer:: operator +(big_integer a, big_integer const& b) {
    if (a.negate != b.negate) {
        return a - (-b);
    }

}