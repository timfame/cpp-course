//
// Created by timfame on 01.06.19.
//

#ifndef HUFFMAN_FILE_ARCHIVER_H
#define HUFFMAN_FILE_ARCHIVER_H

#include <fstream>
#include "reader.h"
#include "writer.h"

struct file_archiver {

    file_archiver() = default;

    explicit file_archiver(std::string const& file_input, std::string const& file_output);

    void archive();

private:
    reader read_count, read_zip;
    writer write_zip;

    const static uint32_t BLOCK_SIZE = 1 << 20;
};


#endif //HUFFMAN_FILE_ARCHIVER_H
