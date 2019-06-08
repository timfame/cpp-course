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
    while (read_zip.has_char()) {
        uint32_t cnt_chars = cur.get_size() / 8;
        if (cur.get_size() % 8) {
            ++cnt_chars;
        }
        if (cnt_chars >= BLOCK_SIZE) {
            std::vector<uint8_t> to_write = cur.get_str();
            for (uint32_t i = 0; i < to_write.size(); ++i) {
                if (i == to_write.size() - 1 && cur.if_cut() > 0) {
                    break;
                }
                write_zip.put(to_write[i]);
            }
            cur.clear();
        }
        arc.add_code(cur, read_zip.get_char());
    }
    for (uint8_t c : cur.get_str()) {
        write_zip.put(c);
    }
}