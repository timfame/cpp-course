//
// Created by timfame on 03.06.19.
//

#include "file_dearchiver.h"
#include "../lib/code.h"
#include "../lib/dearchiver.h"

file_dearchiver::file_dearchiver(std::string const &file_input, std::string const &file_output)
: read(file_input), write(file_output){}

void file_dearchiver::dearchive() {
    uint32_t tree_size = 0;
    for (size_t i = 0; i < BYTE_SIZE; i++) {
        tree_size <<= 8;
        tree_size += read.get_char();
    }
    uint32_t keys_size = read.get_char() + 1;
    uint32_t text_size = 0;
    for (size_t i = 0; i < BYTE_SIZE; i++) {
        text_size <<= 8;
        text_size += read.get_char();
    }
    code cur = code();
    for (size_t i = 0; i < tree_size / 8; i++) {
        cur.add_char(read.get_char());
    }
    if (tree_size % 8 != 0) {
        cur.add_char(read.get_char());
    }
    if (text_size != 0 || tree_size != 0) {
        for (size_t i = 0; i < keys_size; ++i) {
            cur.add_char(read.get_char());
        }
    }
    cur.force_push();
    dearchiver dearc(cur, tree_size, text_size);
    uint32_t cnt = 0;
    std::vector<uint8_t> to_unzip;
    while (read.has_char()) {
        cnt++;
        if (cnt == BLOCK_SIZE) {
            for (uint8_t c : dearc.dearchive(to_unzip)) {
                write.put(c);
            }
            to_unzip.clear();
            to_unzip.resize(0);
            cnt = 0;
        }
        to_unzip.push_back(read.get_char());
    }
    if (dearc.bad_eof()) {
        throw std::runtime_error("Incorrect zip file");
    }
    for (uint8_t c : dearc.dearchive(to_unzip)) {
        write.put(c);
    }
}