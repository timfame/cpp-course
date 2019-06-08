//
// Created by timfame on 31.05.19.
//

#include "huffman.h"

#include <queue>
#include <algorithm>

huffman::huffman(std::vector<uint32_t> const &values) {
    std::priority_queue<std::pair<uint32_t, std::string>, std::vector<std::pair<uint32_t, std::string>>, cmp> q;
    for (size_t i = 0; i < values.size(); ++i) {
        if (values[i] == 0)
            continue;
        q.push({values[i], std::string(1, static_cast<uint8_t >(i))});
    }
    if (q.size() == 1) {
        auto f = q.top(); q.pop();
        add_bit(f.second, 0);
        add_edge('L');
        add_edge('U');
        unzip_order.push_back(f.second[0]);
    }
    while (q.size() > 1) {
        auto f1 = q.top(); q.pop();
        auto f2 = q.top(); q.pop();
        add_bit(f1.second, 0);
        add_bit(f2.second, 1);
        f1.first += f1.first;
        f1.second += f2.second;
        q.push(f1);
    }
    for (auto &v : zip) {
        std::reverse(v.second.begin(), v.second.end());
    }
    for (const auto& v : zip) {
        unzip[v.second] = v.first;
    }
    if (!q.empty()) {
        dfs();
    }

}

void huffman::add_bit(std::string const &word, uint8_t c) {
    for (uint8_t v : word) {
        zip[v].push_back(c);
    }
}

void huffman::dfs() {
    if (unzip.find(cur) != unzip.end()) {
        unzip_order.push_back(unzip[cur]);
        return;
    }
    cur.push_back(0);
    add_edge('L');
    dfs();
    add_edge('U');
    cur.pop_back();

    cur.push_back(1);
    add_edge('R');
    dfs();
    add_edge('U');
    cur.pop_back();
}

const std::unordered_map<uint8_t, std::vector<bool>> huffman::get_zip() const {
    return zip;
}

void huffman::add_edge(uint8_t c) {
    switch (c) {
        case 'L':
            tree.push_back(1), tree.push_back(0);
            break;
        case 'R':
            tree.push_back(0), tree.push_back(1);
            break;
        case 'U':
            tree.push_back(1), tree.push_back(1);
            break;
    }
}

const std::vector<bool> &huffman::get_tree() const {
    return tree;
}

const std::vector<uint8_t> &huffman::get_unzip_order() const {
    return unzip_order;
}
