#include <iostream>
#include <cstring>

struct string {
    string() {
        str = static_cast<char *>(malloc(sizeof(char *)));
        str[0] = '\0';
        _size = 0;
        capacity = 1;
    }

    string(char const *new_str) {
        _size = strlen(new_str);
        capacity = _size * 2 + 1;
        str = static_cast<char *>(malloc(capacity * sizeof(char *)));
        for (size_t i = 0; i < _size; i++)
            str[i] = new_str[i];
        str[_size] = '\0';
    }

    string(char const *new_str, size_t sz) {
        __glibcxx_assert(sz <= strlen(new_str));
        _size = sz;
        capacity = _size * 2 + 1;
        str = static_cast<char *>(malloc(capacity * sizeof(char *)));
        for (size_t i = 0; i < _size; i++)
            str[i] = new_str[i];
        str[_size] = '\0';
    }

    string(const string &new_str) {
        _size = new_str.size();
        capacity = _size * 2 + 1;
        str = static_cast<char *>(malloc(capacity * sizeof(char *)));
        for (size_t i = 0; i < _size; i++)
            str[i] = new_str[i];
        str[_size] = '\0';
    }

    char & operator [](size_t index) {
        __glibcxx_assert(index < _size);
        return str[index];
    }

    const char & operator [](size_t index) const{
        __glibcxx_assert(index < _size);
        return str[index];
    }

    string & operator =(const string & new_str) {
        free(str);
        _size = new_str.size();
        capacity = _size * 2 + 1;
        str = static_cast<char *>(malloc(capacity * sizeof(char *)));
        for (size_t i = 0; i < _size; i++)
            str[i] = new_str[i];
        str[_size] = '\0';
        return *this;
    }

    char const * data() const {
        return str;
    }

    char const * c_str() const {
        return str;
    }

    string & operator +=(char c) {
        ensure_capacity(1);
        str[_size] = c;
        _size++;
        str[_size] = '\0';
        return *this;
    }

    string & operator +=(char const * new_str) {
        append(new_str);
        return *this;
    }

    string & operator +=(string const & new_str) {
        append(new_str);
        return *this;
    }

    void clear() {
        free(str);
        str = static_cast<char *>(malloc(sizeof(char *)));
        str[0] = '\0';
        _size = 0;
        capacity = 1;
    }

    void shrink_to_fit() {
        capacity = _size + 1;
        char *new_str = static_cast<char *>(malloc(capacity * sizeof(char *)));
        for (size_t i = 0; i < _size; i++)
            new_str[i] = str[i];
        free(str);
        str = static_cast<char *>(malloc(capacity * sizeof(char *)));
        for (size_t i = 0; i < _size; i++)
            str[i] = new_str[i];
        str[_size] = '\0';
        free(new_str);
    }

    void append(char const * new_str) {
        size_t len= strlen(new_str);
        ensure_capacity(len);
        for (size_t i = 0; i < len; i++)
            str[i + _size] = new_str[i];
        _size += len;
        str[_size] = '\0';
    }

    void append(char const * new_str, size_t len) {
        ensure_capacity(len);
        for (size_t i = 0; i < len; i++)
            str[i + _size] = new_str[i];
        _size += len;
        str[_size] = '\0';
    }

    void append(string const & new_str) {
        size_t len = new_str.size();
        ensure_capacity(len);
        for (size_t i = 0; i < len; i++)
            str[i + _size] = new_str[i];
        _size += len;
        str[_size] = '\0';
    }

    void assign(char const * new_str, size_t sz) {
        free(str);
        __glibcxx_assert(sz <= strlen(s));
        _size = sz;
        capacity = _size * 2 + 1;
        str = static_cast<char *>(malloc(capacity * sizeof(char *)));
        for (size_t i = 0; i < _size; i++)
            str[i] = new_str[i];
        str[_size] = '\0';
    }

    size_t size() const {
        return _size;
    }

    bool empty() const {
        return _size == 0;
    }

    void print() {
        for (size_t i = 0; i < _size; i++)
            std::cout << str[i];
    }

    void println() {
        print();
        std::cout << "\n";
    }

private:
    size_t _size;
    size_t capacity = 0;
    char *str;

    void ensure_capacity(size_t x) {
        if (_size + x + 1 <= capacity)
            return;
        capacity *= 2;
        char *new_str = static_cast<char *>(malloc(capacity * sizeof(char *)));
        for (size_t i = 0; i < _size; i++)
            new_str[i] = str[i];
        free(str);
        str = static_cast<char *>(malloc(capacity * sizeof(char *)));
        for (size_t i = 0; i < _size; i++)
            str[i] = new_str[i];
        str[_size] = '\0';
        free(new_str);
    }
};

int main() {

    return 0;
}