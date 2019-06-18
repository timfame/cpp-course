#include <iostream>
#include "buffer/circular_buffer.h"

using namespace std;

int main() {
    circular_buffer<int> c(9);
    for (size_t i = 0; i != 20; ++i)
    {
        int val = (i + 42) % 13;
        c.push_back(val);
    }
    for (size_t i = 0; i != 20; ++i)
    {
        int val = (i + 42) % 13;
        c.push_front(val);
    }
    return 0;
}