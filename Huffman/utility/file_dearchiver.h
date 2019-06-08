//
// Created by timfame on 03.06.19.
//

#ifndef HUFFMAN_FILE_DEARCHIVER_H
#define HUFFMAN_FILE_DEARCHIVER_H


#include <fstream>
#include "reader.h"
#include "writer.h"

struct file_dearchiver {

    file_dearchiver() = default;

    file_dearchiver(std::string const& file_input, std::string const& file_output);

    void dearchive();

private:
    reader read;
    writer write;

    const static uint32_t BLOCK_SIZE = 1 << 10;
    const static size_t BYTE_SIZE = 4;
};


#endif //HUFFMAN_FILE_DEARCHIVER_H
