//
// Created by timfame on 31.05.19.
//

#ifndef HUFFMAN_HUFFMAN_H
#define HUFFMAN_HUFFMAN_H

#include <fstream>
#include <vector>
#include <queue>
#include <unordered_map>

struct huffman {

    huffman(std::vector<uint32_t> const& values);

    const std::unordered_map<uint8_t, std::vector<bool>> get_zip() const;

    const std::vector<bool> &get_tree() const;

    const std::vector<uint8_t > &get_unzip_order() const;

    ~huffman() = default;

private:

    std::unordered_map<uint8_t, std::vector<bool>> zip;
    std::unordered_map<std::vector<bool>, uint8_t> unzip;
    std::vector<bool> tree, cur;
    std::vector<uint8_t> unzip_order;

    void add_bit(std::string const& word, uint8_t c);
    void dfs();
    void add_edge(uint8_t c);

    struct cmp {
        bool operator()(const std::pair<uint32_t, std::string>& a, const std::pair<uint32_t, std::string>& b) const {
            return a > b;
        }
    };
};


#endif //HUFFMAN_HUFFMAN_H
