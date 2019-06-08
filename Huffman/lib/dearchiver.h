//
// Created by timfame on 03.06.19.
//

#ifndef HUFFMAN_DEARCHIEVER_H
#define HUFFMAN_DEARCHIEVER_H

#include <fstream>
#include "code.h"
#include <vector>
#include <unordered_map>

struct dearchiver {

    dearchiver() = default;

    dearchiver(code const& info_code, uint32_t tree_info, uint32_t text_info);

    std::string dearchive(std::vector <uint8_t> const& s);

    bool bad_eof() const;

private:
    code info;
    uint32_t tree_size;
    uint32_t text_size;
    uint32_t cnt_unzip = 0;
    std::vector<char> order_char;
    std::unordered_map<std::vector<bool>, char> zip;
    std::vector<bool> cur;
    std::vector<bool> last;
    const static uint32_t MAX_CODE = 256;
};


#endif //HUFFMAN_DEARCHIEVER_H
