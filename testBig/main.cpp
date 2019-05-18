#include <iostream>
#include "bit_vector.h"
#include "big_integer.h"

int main() {
    bit_vector a(4, 0);
    a.push_back(1);
    for (int i = 0; i < a.size(); i++)
        std::cout << a[i] << " ";
    return 0;
}