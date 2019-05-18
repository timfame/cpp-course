//
// Created by timfame on 18.05.19.
//

#ifndef BIGINT_BIT_VECTOR_H
#define BIGINT_BIT_VECTOR_H

#include <cstdio>
#include <memory>

struct bit_vector {
    bit_vector();
    bit_vector(unsigned new_val);
    bit_vector(bit_vector const& new_val);
    bit_vector(size_t s, unsigned val);
    ~bit_vector();

    bit_vector& operator=(bit_vector other);
    unsigned& operator[](size_t index);
    unsigned const& operator[](size_t index) const;
    void push_back(unsigned element);
    void pop_back();
    unsigned& back();
    unsigned const& back() const;
    void resize(size_t new_size);

    friend bool operator ==(bit_vector const& a, bit_vector const& b);

    void swap(bit_vector &other);
    size_t size() const;
    bool empty() const;

private:
    bool is_dynamic = false;
    size_t _size;
    struct dynamic {
        dynamic(size_t s, unsigned* a);
        size_t capacity;
        std::shared_ptr<unsigned> pointer;
    };
    union some {
        unsigned statical[2];
        dynamic dynamical;
        some(){};
        ~some(){};
    } data;
    unsigned *begin() const;

    void ensure_capacity();
    void make_dynamical();
    void delete_ref();
};


#endif //BIGINT_BIT_VECTOR_H