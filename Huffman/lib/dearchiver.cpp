//
// Created by timfame on 03.06.19.
//

#include "dearchiver.h"

dearchiver::dearchiver(code const &info_code, uint32_t tree_info, uint32_t text_info) :
info(info_code), tree_size(tree_info), text_size(text_info), cnt_unzip(0){
    info.set_index(tree_size);
    while (info.has_char()) {
        order_char.push_back(info.next_char());
    }
    info.set_index(0);
    uint8_t cnt = 0;
    uint8_t prev = 0;
    for (uint32_t i = 0; i < tree_size; i += 2) {
        uint8_t t = (info.next_bit() << 1) + info.next_bit();
        if (t == 0) {
            throw std::runtime_error("Incorrect zip file");
        }
        if (t == 2)
            cur.push_back(0);
        if (t == 1)
            cur.push_back(1);
        if (t == 3) {
            if (prev == 1 || prev == 2) {
                zip[cur] = order_char[cnt];
                cnt++;
            }
            if (cur.empty()) {
                throw std::runtime_error("Incorrect zip file");
            }
            cur.pop_back();
        }
        prev = t;
    }
    if (!cur.empty()) {
        throw std::runtime_error("Incorrect zip file");
    }
}

std::string dearchiver::dearchive(std::string const &s) {
    std::string res;
    std::vector<bool> c(last);
    code source(s);
    while (source.has_bit()) {
        c.push_back(source.next_bit());
        if (zip.find(c) != zip.end()) {
            res.push_back(zip[c]);
            c.clear();
            cnt_unzip++;
            if (cnt_unzip == text_size) {
                break;
            }
        }
        if (c.size() > MAX_CODE) {
            throw std::runtime_error("Incorrect zip file");
        }
    }
    last = c;
    return res;
}

bool dearchiver::bad_eof() const {
    if (cnt_unzip < text_size) {
        return false;
    }
    for (bool i : last)
        if (i) {
            return true;
        }
    return false;
}