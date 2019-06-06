//
// Created by timfame on 06.06.19.
//

#include "../gtest/gtest.h"
#include "../utility/file_archiver.h"
#include "../utility/file_dearchiver.h"

const static std::string path = "/home/timfame/cpp-course/Huffman/test/";
std::string type = ".txt";
std::string name;

std::string get_in() {
    return path + "in/" + name + type;
}

std::string get_zip() {
    return path + "zip/" + name + "_zip" + type;
}

std::string get_unzip() {
    return path + "unzip/" + name + "_unzip" + type;
}

void to_zip(std::string const& in, std::string const& out) {
    file_archiver f(in, out);
    f.archive();
}

void to_unzip(std::string const& in, std::string const& out) {
    file_dearchiver f(in, out);
    f.dearchive();
}

bool compare_files(std::string const& l, std::string const& r) {
    std::ifstream l_stream(l, std::ifstream::binary);
    std::ifstream r_stream(r, std::ifstream::binary);
    if (l_stream.tellg() != r_stream.tellg()) {
        return false;
    }
    l_stream.seekg(0, std::ifstream::beg);
    r_stream.seekg(0, std::ifstream::beg);
    return std::equal(std::istreambuf_iterator<char>(l_stream.rdbuf()),
                      std::istreambuf_iterator<char>(),
                      std::istreambuf_iterator<char>(r_stream.rdbuf()));
}

TEST(my_tests, site_copy) {
    name = "site_copy";
    to_zip(get_in(), get_zip());
    to_unzip(get_zip(), get_unzip());
    EXPECT_TRUE(compare_files(get_in(), get_unzip()));
}
