//
// Created by timfame on 06.06.19.
//

#include "../gtest/gtest.h"
#include "../utility/file_archiver.h"
#include "../utility/file_dearchiver.h"
#include <random>
#include <functional>
#include <algorithm>

const static std::string path = "test/";
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

void check() {
    EXPECT_NO_THROW(to_zip(get_in(), get_zip()));
    EXPECT_NO_THROW(to_unzip(get_zip(), get_unzip()));
    EXPECT_TRUE(compare_files(get_in(), get_unzip()));
}

TEST(my_tests, site_test) {
    name = "site_test";
    check();
}

TEST(my_tests, medium_test) {
    name = "medium_test";
    check();
}

TEST(my_tests, big_test) {
    name = "big_test";
    check();
}

TEST(my_tests, code_test) {
    name = "huffman_test";
    type = ".cpp";
    check();
}

TEST(my_tests, photo_test) {
    name = "photo_test";
    type = ".png";
    check();
}

const uint8_t MIN_RAND = 0;
const uint8_t MAX_RAND = 255;

const uint32_t SMALL_SIZE = 1 << 10;
const uint32_t MEDIUM_SIZE = 1 << 20;
const uint32_t BIG_SIZE = 1 << 25;

void generate_file(uint32_t size) {
    writer w(get_in());
    std::mt19937 my_rand(time(nullptr));
    auto get_rand = std::bind(std::uniform_int_distribution<uint8_t>(MIN_RAND, MAX_RAND), my_rand);
    for (size_t i = 0; i < size; ++i) {
        uint8_t c = get_rand();
        w.put(c);
    }
}

void generate_file(uint32_t size, uint8_t c) {
    writer w(get_in());
    for (size_t i = 0; i < size; ++i) {
        w.put(c);
    }
}

TEST(random_tests, random_test_1B) {
    name = "random_test";
    type = ".txt";
    generate_file(1);
    check();
}

TEST(random_tests, random_test_10B) {
    generate_file(10);
    check();
}

TEST(random_tests, random_test_100B) {
    generate_file(100);
    check();
}

TEST(random_tests, random_test_500B) {
    generate_file(500);
    check();
}

TEST(random_tests, random_test_1KB) {
    generate_file(SMALL_SIZE);
    check();
}

TEST(random_tests, random_test_10KB) {
    generate_file(SMALL_SIZE * 10);
    check();
}

TEST(random_tests, random_test_100KB) {
    generate_file(SMALL_SIZE * 100);
    check();
}

TEST(random_tests, random_test_500KB) {
    generate_file(SMALL_SIZE * 500);
    check();
}

TEST(random_tests, random_test_1MB) {
    generate_file(MEDIUM_SIZE);
    check();
}

TEST(random_tests, random_test_32MB) {
    generate_file(BIG_SIZE);
    check();
}
TEST(random_tests, tests_1KB_1000_times) {
    for (size_t i = 0; i < 1000; ++i) {
        generate_file(SMALL_SIZE);
        check();
    }
}

TEST(random_tests, tests_1MB_10_times) {
    for (size_t i = 0; i < 10; ++i) {
        generate_file(MEDIUM_SIZE);
        check();
    }
}

TEST(random_tests, random_size_10_times) {
    std::mt19937 my_rand(time(nullptr));
    auto get_rand = std::bind(std::uniform_int_distribution<uint32_t>(0, 1 << 20), my_rand);
    for (size_t i = 0; i < 10; ++i) {
        generate_file(get_rand());
        check();
    }
}

TEST(random_tests, random_fibonacci_test) {
    uint32_t sz[25];
    sz[0] = 1, sz[1] = 1;
    uint32_t size = 2;
    for (size_t i = 2; i < 25; ++i) {
        sz[i] = sz[i - 1] + sz[i - 2];
        size += sz[i];
    }
    std::mt19937 my_rand(time(nullptr));
    auto get_rand = std::bind(std::uniform_int_distribution<uint32_t>(0, 1 << 20), my_rand);
    std::vector<uint8_t> text;
    for (uint32_t i : sz) {
        uint8_t c = get_rand();
        for (uint32_t j = 0; j < i; ++j) {
            text.push_back(c);
        }
    }
    std::random_shuffle(text.begin(), text.end());
    writer w(get_in());
    for (uint8_t c : text) {
        w.put(c);
    }
    check();
}

TEST(exception_tests, wrong_compressed_test) {
    name = "exception_test";
    generate_file(SMALL_SIZE, 255);
    EXPECT_ANY_THROW(to_unzip(get_zip(), get_unzip()));
}