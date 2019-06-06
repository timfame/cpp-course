//
// Created by timfame on 01.06.19.
//

#include "code.h"

code::code() : size(0), buff(0), start(0), not_first(false){}

code::code(std::string const& s) : size(s.size() * 8), buff(0), start(0), not_first(false){
    for (uint8_t i : s)
        str.push_back(i);
}

code::code(code const &s) : size(s.size), buff(s.buff), start(s.start), str(s.str), not_first(s.not_first){}

void code::set_index(uint32_t index) {
    start = index;
}

void code::add_bit(uint8_t c) {
    force_push();
    buff <<= 1;
    buff += c;
    ++size;
    not_first = true;
}

uint8_t code::next_bit() {
    uint8_t res = (str[start / 8] >> (7 - (start % 8))) & 1;
    start++;
    return res;
}

bool code::has_bit() const {
    return start < size;
}

void code::add_char(uint8_t c) {
    for (size_t i = 8; i >= 1; --i) {
        add_bit((c >> (i - 1)) & 1);
    }
}

uint8_t code::next_char() {
    uint8_t res = 0;
    for (size_t i = 0; i < 8; i++) {
        res <<= 1;
        res += next_bit();
    }
    return res;
}

bool code::has_char() const {
    return start + 7 < size;
}

const std::vector<uint8_t> &code::get_str() {
    cut();
    return str;
}

void code::clear() {
    str.resize(0);
    size = 0;
    buff = 0;
    start = 0;
}

uint32_t code::get_size() const {
    return size;
}

void code::cut() {
    if (size % 8 != 0) {
        buff <<= 8 - size % 8;
        str.push_back(buff);
        buff = 0;
        size = size / 8 * 8 + 8;
    }
}

void code::force_push() {
    if (size % 8 == 0 && not_first) {
        str.push_back(buff);
        buff = 0;
    }
}