//
// Created by timfame on 01.06.19.
//

#ifndef HUFFMAN_ARCHIEVER_H
#define HUFFMAN_ARCHIEVER_H

#include <fstream>
#include <vector>
#include <unordered_map>
#include "code.h"

struct archiever {

    archiever() = default;

    void add_buffer(std::string const& s);
    std::string& get_info();
    void add_code(code& cur, uint8_t c);
    void archive();

private:
    uint32_t count[256];
    std::unordered_map<uint8_t , std::vector<bool>> codes;
    std::string info = "";
    bool zipped = false;
    const static size_t BIT_SIZE = 32;
    uint32_t text_size = 0;
};


#endif //HUFFMAN_ARCHIEVER_H
