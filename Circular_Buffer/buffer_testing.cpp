//
// Created by timfame on 17.06.19.
//

#include "gtest/gtest.h"
#include "fault_injection.h"
#include "counted.h"
#include "buffer/circular_buffer.h"

typedef circular_buffer<int> container;

TEST(correctness, default_ctor)
{
    faulty_run([]
       {
           container c;
           EXPECT_EQ(0u, c.size());
           EXPECT_TRUE(c.empty());
       });
}

TEST(correctness, push_simple)
{
    faulty_run([]
       {
           container c;
           c.push_back(4);
           EXPECT_FALSE(c.empty());
           EXPECT_EQ(1u, c.size());
           c.push_back(8);
           EXPECT_FALSE(c.empty());
           EXPECT_EQ(1u, c.size());
           c.push_front(15);
           EXPECT_FALSE(c.empty());
           EXPECT_EQ(1u, c.size());
           c.push_back(16);
           EXPECT_FALSE(c.empty());
           EXPECT_EQ(1u, c.size());
           c.push_front(23);
           EXPECT_FALSE(c.empty());
           EXPECT_EQ(1u, c.size());
           c.push_front(42);
           EXPECT_FALSE(c.empty());
           EXPECT_EQ(1u, c.size());
       });
}

TEST(correctness, push_big)
{
    faulty_run([]
        {
           container c(9);

           for (size_t i = 0; i != 20; ++i)
           {
               int val = (i + 42) % 13;
               c.push_back(val);
           }

           for (size_t i = 0; i != 20; ++i)
           {
               int val = (i + 42) % 13;
               EXPECT_EQ(val, c[i]);
           }

           for (size_t i = 0; i != 20; ++i)
           {
               int val = (i + 42) % 13;
               c.push_front(val);
           }

           for (size_t i = 0; i != 20; ++i)
           {
               int val = (i + 42) % 13;
               EXPECT_EQ(val, c[19 - i]);
           }
       });
}

