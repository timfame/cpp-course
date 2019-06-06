//
// Created by timfame on 31.05.19.
//

#ifndef HUFFMAN_WRITER_H
#define HUFFMAN_WRITER_H


#include <fstream>

struct writer {

    writer() = default;

    explicit writer(std::string const &file);

    ~writer();

    void put(uint8_t c);

private:
    const static size_t BUFF_SIZE = 1024;
    std::ofstream stream;
    uint8_t buffer[BUFF_SIZE];
    size_t index;

    void put_buffer();
};


#endif //HUFFMAN_WRITER_H
