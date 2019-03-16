#include <iostream>
#include <cstring>
#include <vector>

bool exist(char *text, char *key, size_t len) {
    const int p = 307;
    std:: vector <long long> pow(len);
    pow[0] = 1;
    for (size_t i = 1; i < len; i++)
        pow[i] = pow[i - 1] * p;
    std:: vector <long long> h(len);
    for (size_t i = 0; i < len; i++) {
        h[i] = (text[i] - 'a' + 1) * pow[i];
        if (i)
            h[i] += h[i - 1];
    }
    long long hash = 0;
    for (size_t i = 0; i < strlen(key); i++)
        hash += (key[i] - 'a' + 1) * pow[i];
    for (size_t i = 0; i + strlen(key) - 1 < len; i++) {
        long long cur = h[i + strlen(key) - 1];
        if (i)
            cur -= h[i - 1];
        if (cur == hash * pow[i])
            return true;
    }
    return false;
}

int main(int argv, char *argc[]) {
    char *word = argc[2];
    size_t BUFF_SIZE = strlen(word);
    FILE *file = fopen(argc[1], "r");
    char *str = new char[BUFF_SIZE * 2];
    size_t sz = fread(str, sizeof(char), BUFF_SIZE, file);
    size_t str_size = sz;
    bool found = false;
    while (sz != 0 && !found) {
        char *buff = new char[BUFF_SIZE];
        sz = fread(buff, sizeof(char), BUFF_SIZE, file);
        for (size_t i = 0; i < sz; i++)
            str[str_size + i] = buff[i];
        if (exist(str, word, str_size + sz))
            found = true;
        delete[] str;
        str = buff;
        str_size = sz;
    }
    delete[] str;
    std:: cout << (found ? "true" : "false");
    fclose(file);
}
