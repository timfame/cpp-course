//
// Created by blagoi on 18.05.19.
//

#ifndef BIGINT_BIT_VECTOR_H
#define BIGINT_BIT_VECTOR_H

#include <cstdio>
#include <memory>

struct bit_vector {
    bit_vector();
    bit_vector(uint32_t new_val);
    bit_vector(bit_vector const& new_val) noexcept;
    bit_vector(size_t s, uint32_t val);
    ~bit_vector();

    bit_vector& operator=(bit_vector other) noexcept;
    uint32_t & operator[](size_t index);
    uint32_t const& operator[](size_t index) const;
    void push_back(uint32_t element);
    void pop_back();
    uint32_t & back();
    uint32_t const& back() const;

    friend bool operator ==(bit_vector const& a, bit_vector const& b);

    void swap(bit_vector &other);
    size_t size() const;
    bool empty() const;

private:
    static const size_t STATIC_SIZE = 4;
    bool is_dynamic = false;
    size_t _size;
    struct dynamic {
        dynamic(size_t s, uint32_t * a);
        size_t capacity;
        std::shared_ptr<uint32_t > pointer;
    };
    union some {
        uint32_t statical[STATIC_SIZE];
        dynamic dynamical;
        some(){};
        ~some(){};
    } data;
    uint32_t *begin() const;

    void ensure_capacity();
    void make_dynamical();
    void make_statical();
    void delete_ref();
};


#endif //BIGINT_BIT_VECTOR_H
