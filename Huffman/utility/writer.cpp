//
// Created by timfame on 31.05.19.
//

#include "writer.h"

writer::writer(std::string const &file) : stream(file, std::ofstream::binary), index(0) {
    if (!stream.is_open()) {
        throw std::runtime_error("Output file not found");
    }
}

writer::~writer() {
    put_buffer();
    stream.close();
}

void writer::put_buffer() {
    stream.write(reinterpret_cast<char *>(buffer), index);
}

void writer::put(uint8_t c) {
    if (index == BUFF_SIZE) {
        put_buffer();
        index = 0;
    }
    buffer[index] = c;
    index++;
}