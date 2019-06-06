//
// Created by timfame on 01.06.19.
//

#ifndef HUFFMAN_CODE_H
#define HUFFMAN_CODE_H

#include <fstream>
#include <vector>

struct code {
    code();
    code(std::string const& s);
    code(code const& s);

    void set_index(uint32_t index);
    void add_bit(uint8_t c);
    uint8_t next_bit();
    bool has_bit() const;
    void add_char(uint8_t c);
    uint8_t next_char();
    bool has_char() const;
    void clear();
    uint32_t get_size() const;

    void cut();
    void force_push();

    const std::vector<uint8_t > &get_str();

private:
    std::vector<uint8_t > str;
    uint32_t size;
    uint32_t start;
    uint8_t buff;
    bool not_first;
};


#endif //HUFFMAN_CODE_H
