//
// Created by timfame on 01.06.19.
//

#include "file_archiver.h"
#include "../lib/archiver.h"
#include "../lib/code.h"

file_archiver::file_archiver(std::string const &file_input, std::string const &file_output)
: read_count(file_input), read_zip(file_input), write_zip(file_output) {}

void file_archiver::archive() {
    archiever arc = archiever();
    std::string buff;
    while (read_count.has_char()) {
        buff.push_back(read_count.get_char());
        if (buff.size() == BLOCK_SIZE) {
            arc.add_buffer(buff);
            buff = "";
        }
    }
    arc.add_buffer(buff);
    arc.archive();
    code cur(arc.get_info());
    uint32_t cnt = arc.get_info().size();
    while (read_zip.has_char()) {
        ++cnt;
        if (cnt == BLOCK_SIZE) {
            for (uint8_t c : cur.get_str()) {
                write_zip.put(c);
            }
            cur.clear();
        }
        arc.add_code(cur, read_zip.get_char());
    }
    for (uint8_t c : cur.get_str()) {
        write_zip.put(c);
    }
}