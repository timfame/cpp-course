//
// Created by timfame on 01.06.19.
//

#include "archiver.h"
#include "huffman.h"
#include "code.h"

#include <vector>
#include <algorithm>

void archiever::add_buffer(std::string const &s) {
    if (zipped) {
        return;
    }
    for (uint8_t i : s)
        count[i]++;
    text_size += s.size();
}

void archiever::archive() {
    huffman hm(std::vector<uint32_t>(count, count + 256));
    std::vector<bool> tree = hm.get_tree();
    codes = hm.get_zip();
    std::vector<bool> size_of_tree(BIT_SIZE, false);
    std::vector<bool> size_of_keys(8, false);
    std::vector<bool> size_of_text(BIT_SIZE, false);
    std::vector<uint8_t > keys = hm.get_unzip_order();
    std::string res;
    code cur = code();

    size_t n = tree.size();
    size_t index = BIT_SIZE - 1;
    while (n > 0) {
        size_of_tree[index] = n % 2;
        index--;
        n >>= 1;
    }
    for (auto i : size_of_tree) {
        cur.add_bit(i);
    }

    n = keys.size() - 1;
    index = 7;
    while (n > 0) {
        size_of_keys[index] = n % 2;
        index--;
        n >>= 1;
    }
    for (auto i : size_of_keys) {
        cur.add_bit(i);
    }

    n = text_size;
    index = BIT_SIZE - 1;
    while (n > 0) {
        size_of_text[index] = n % 2;
        index--;
        n >>= 1;
    }
    for (auto i : size_of_text) {
        cur.add_bit(i);
    }

    for (auto i : tree) {
        cur.add_bit(i);
    }
    for (auto i : keys) {
        cur.add_char(i);
    }
    cur.force_push();

    zipped = true;
    for (uint8_t i : cur.get_str()) {
        info.push_back(i);
    }
}

std::string& archiever::get_info() {
    if (!zipped) {
        throw std::runtime_error("There is no archive");
    }
    return info;
}

void archiever::add_code(code &cur, uint8_t c) {
    for (bool bit : codes[c]) {
        cur.add_bit(bit);
    }
}

