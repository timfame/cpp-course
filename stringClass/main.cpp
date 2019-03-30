#include <iostream>
#include <cstring>

struct string {
    string() {}

    string(char const *s) {
        _size = strlen(s);
        str = static_cast<char *>(malloc((_size + 1) * sizeof(char *)));
        for (size_t i = 0; i < _size; i++)
            str[i] = s[i];
        str[_size] = '\0';
    }

    string(char const *s, size_t sz) {
        str = static_cast<char *>(malloc((sz + 1) * sizeof(char *)));
        for (size_t i = 0; i < sz; i++)
            str[i] = s[i];
        str[sz] = '\0';
        _size = sz;
    }

    string(string s) {
        _size = s.size();
        str = static_cast<char *>(malloc((_size + 1) * sizeof(char *)));
        for (size_t i = 0; i < _size; i++)
            str[i] = s[i];
        str[_size] = '\0';
    }



    size_t size() {
        return _size;
    }

    bool empty() {
        return _size == 0;
    }

private:
    size_t _size;
    char *str;
};

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}