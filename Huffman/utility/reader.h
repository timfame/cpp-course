//
// Created by timfame on 31.05.19.
//

#ifndef HUFFMAN_READER_H
#define HUFFMAN_READER_H


#include <string>
#include <fstream>

struct reader {

    reader() = default;

    explicit reader(std::string const &file);

    ~reader();

    uint8_t get_char();

    bool has_char();

private:
    const static size_t BUFF_SIZE = 1024;
    std::ifstream stream;
    uint8_t buffer[BUFF_SIZE];
    size_t index = 0;
    size_t count = 0;

    void get_buffer();
};


#endif //HUFFMAN_READER_H
