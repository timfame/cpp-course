//
// Created by timfame on 31.05.19.
//

#include "reader.h"


reader::reader(std::string const &file) : stream(file, std::ifstream::binary), index(0), count(0) {
    if (stream.fail()) {
        stream.close();
        throw std::runtime_error("Input file not found");
    }
    get_buffer();
}

reader::~reader() {
    stream.close();
}

bool reader::has_char() {
    return index < count || !stream.eof();
}

uint8_t reader::get_char() {
    if (index >= count) {
        if (stream.eof()) {
            throw std::runtime_error("There is no more data");
        }
        get_buffer();
    }
    index++;
    return buffer[index - 1];
}

void reader::get_buffer() {
    stream.read(reinterpret_cast<char *>(buffer), BUFF_SIZE);
    count = stream.gcount();
    index = 0;
}