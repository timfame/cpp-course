#include "gtest/gtest.h"
#include "fault_injection.h"
#include "counted.h"
#include "my_vector/vector.h"

typedef vector<counted> container;

TEST(correctness, default_ctor)
{
    faulty_run([]
    {
        container c;
        EXPECT_EQ(0u, c.size());
        EXPECT_TRUE(c.empty());
    });
}

TEST(correctness, push_back_simple)
{
    faulty_run([]
    {
        container c;
        c.push_back(4);
        EXPECT_FALSE(c.empty());
        EXPECT_EQ(1u, c.size());
        c.push_back(8);
        EXPECT_FALSE(c.empty());
        EXPECT_EQ(2u, c.size());
        c.push_back(15);
        EXPECT_FALSE(c.empty());
        EXPECT_EQ(3u, c.size());
        c.push_back(16);
        EXPECT_FALSE(c.empty());
        EXPECT_EQ(4u, c.size());
        c.push_back(23);
        EXPECT_FALSE(c.empty());
        EXPECT_EQ(5u, c.size());
        c.push_back(42);
        EXPECT_FALSE(c.empty());
        EXPECT_EQ(6u, c.size());
    });
}

TEST(correctness, push_back_big)
{
    faulty_run([]
    {
        container c;
        
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
    });
}

TEST(correctness, pop_back)
{
    faulty_run([]
    {
        container c;
        c.push_back(4);
        c.push_back(8);
        c.push_back(15);
        c.push_back(16);
        c.push_back(23);
        c.push_back(42);
        c.pop_back();
        EXPECT_FALSE(c.empty());
        EXPECT_EQ(5u, c.size());
        c.pop_back();
        EXPECT_FALSE(c.empty());
        EXPECT_EQ(4u, c.size());
        c.pop_back();
        EXPECT_FALSE(c.empty());
        EXPECT_EQ(3u, c.size());
        c.pop_back();
        EXPECT_FALSE(c.empty());
        EXPECT_EQ(2u, c.size());
        c.pop_back();
        EXPECT_FALSE(c.empty());
        EXPECT_EQ(1u, c.size());
        c.pop_back();
        EXPECT_TRUE(c.empty());
        EXPECT_EQ(0u, c.size());
    });
}

TEST(correctness, copy_ctor)
{
    faulty_run([]
    {
        container c;
        c.push_back(1);
        c.push_back(2);
        c.push_back(3);

        container d = c;
        d[2] = 10;
        EXPECT_EQ(3, c[2]);

        d.push_back(4);
        EXPECT_EQ(3u, c.size());
        EXPECT_EQ(4u, d.size());
    });
}

TEST(correctness, copy_ctor_empty)
{
    faulty_run([]
    {
        container c;
        container d = c;
    });
}

TEST(correctness, assignment)
{
    faulty_run([]
    {
        container c;
        c.push_back(1);
        c.push_back(2);
        c.push_back(3);

        container d;
        d.push_back(4);
        d.push_back(5);

        d = c;
        EXPECT_EQ(3u, d.size());
        EXPECT_EQ(1, d[0]);
        EXPECT_EQ(2, d[1]);
        EXPECT_EQ(3, d[2]);
    });
}

TEST(correctness, assignment_empty)
{
    faulty_run([]
    {
        container c;
        c.push_back(1);
        c.push_back(2);
        c.push_back(3);
        container d;
        d = c;
    });
}

TEST(correctness, assignment_empty_2)
{
    faulty_run([]
    {
        container c;
        container d;
        d.push_back(1);
        d.push_back(2);
        d.push_back(3);
        d = c;
    });
}

TEST(correctness, assignment_self)
{
    faulty_run([]
    {
        container c;
        c.push_back(1);
        c.push_back(2);
        c.push_back(3);
        c = c;
    });
}

TEST(correctness, assignment_self_empty)
{
    faulty_run([]
    {
        container c;
        c = c;
    });
}

TEST(correctness, subscript)
{
    faulty_run([]
    {
        container c;
        c.push_back(4);
        c.push_back(8);
        c.push_back(15);
        c.push_back(16);
        c.push_back(23);
        c.push_back(42);
        
        EXPECT_EQ(4, c[0]);
        EXPECT_EQ(8, c[1]);
        EXPECT_EQ(15, c[2]);
        EXPECT_EQ(16, c[3]);
        EXPECT_EQ(23, c[4]);
        EXPECT_EQ(42, c[5]);
    });
}

TEST(correctness, const_subscript)
{
    faulty_run([]
    {
        container c;
        c.push_back(4);
        c.push_back(8);
        c.push_back(15);
        c.push_back(16);
        c.push_back(23);
        c.push_back(42);

        container const& cc = c;
        EXPECT_EQ(4, cc[0]);
        EXPECT_EQ(8, cc[1]);
        EXPECT_EQ(15, cc[2]);
        EXPECT_EQ(16, cc[3]);
        EXPECT_EQ(23, cc[4]);
        EXPECT_EQ(42, cc[5]);
    });
}

TEST(correctness, insert)
{
    faulty_run([]
    {
        container c;
        c.insert(c.begin(), 15);
        c.insert(c.begin() + 1, 42);
        c.insert(c.begin() + 1, 16);
        c.insert(c.begin() + 2, 23);
        c.insert(c.begin(), 4);
        c.insert(c.begin() + 1, 8);

        EXPECT_EQ(6u, c.size());
        EXPECT_EQ(4, c[0]);
        EXPECT_EQ(8, c[1]);
        EXPECT_EQ(15, c[2]);
        EXPECT_EQ(16, c[3]);
        EXPECT_EQ(23, c[4]);
        EXPECT_EQ(42, c[5]);
    });
}

TEST(correctness, erase)
{
    faulty_run([]
    {
        container c;
        c.push_back(4);
        c.push_back(8);
        c.push_back(15);
        c.push_back(16);
        c.push_back(23);
        c.push_back(42);

        c.erase(c.begin() + 2);
        EXPECT_EQ(4, c[0]);
        EXPECT_EQ(8, c[1]);
        EXPECT_EQ(16, c[2]);
        EXPECT_EQ(23, c[3]);
        EXPECT_EQ(42, c[4]);
    });
}

TEST(correctness, erase_begin)
{
    faulty_run([]
    {
        container c;
        c.push_back(4);
        c.push_back(8);
        c.push_back(15);
        c.push_back(16);
        c.push_back(23);
        c.push_back(42);

        c.erase(c.begin());
        EXPECT_EQ(8, c[0]);
        EXPECT_EQ(15, c[1]);
        EXPECT_EQ(16, c[2]);
        EXPECT_EQ(23, c[3]);
        EXPECT_EQ(42, c[4]);
    });
}

TEST(correctness, erase_end)
{
    faulty_run([]
    {
        container c;
        c.push_back(4);
        c.push_back(8);
        c.push_back(15);
        c.push_back(16);
        c.push_back(23);
        c.push_back(42);

        c.erase(c.end() - 1);
        EXPECT_EQ(4, c[0]);
        EXPECT_EQ(8, c[1]);
        EXPECT_EQ(15, c[2]);
        EXPECT_EQ(16, c[3]);
        EXPECT_EQ(23, c[4]);
    });
}

TEST(correctness, erase_range_begin)
{
    faulty_run([]
    {
        container c;
        c.push_back(4);
        c.push_back(8);
        c.push_back(15);
        c.push_back(16);
        c.push_back(23);
        c.push_back(42);

        c.erase(c.begin(), c.begin() + 2);
        EXPECT_EQ(15, c[0]);
        EXPECT_EQ(16, c[1]);
        EXPECT_EQ(23, c[2]);
        EXPECT_EQ(42, c[3]);
    });
}

TEST(correctness, erase_range_middle)
{
    faulty_run([]
    {
        container c;
        c.push_back(4);
        c.push_back(8);
        c.push_back(15);
        c.push_back(16);
        c.push_back(23);
        c.push_back(42);

        c.erase(c.begin() + 2, c.end() - 1);
        EXPECT_EQ(4, c[0]);
        EXPECT_EQ(8, c[1]);
        EXPECT_EQ(42, c[2]);
    });
}

TEST(correctness, erase_range_end)
{
    faulty_run([]
    {
        container c;
        c.push_back(4);
        c.push_back(8);
        c.push_back(15);
        c.push_back(16);
        c.push_back(23);
        c.push_back(42);

        c.erase(c.begin() + 3, c.end());
        EXPECT_EQ(4, c[0]);
        EXPECT_EQ(8, c[1]);
        EXPECT_EQ(15, c[2]);
    });
}

TEST(correctness, erase_range_all)
{
    faulty_run([]
    {
        container c;
        c.push_back(4);
        c.push_back(8);
        c.push_back(15);
        c.push_back(16);
        c.push_back(23);
        c.push_back(42);

        c.erase(c.begin(), c.end());
        EXPECT_TRUE(c.empty());
    });
}

TEST(correctness, reserve)
{
    faulty_run([]
    {
        container c;
        c.reserve(10);
        c.push_back(4);
        c.push_back(8);
        c.push_back(15);
        c.push_back(16);
        c.push_back(23);
        c.push_back(42);

        EXPECT_EQ(4, c[0]);
        EXPECT_EQ(8, c[1]);
        EXPECT_EQ(15, c[2]);
        EXPECT_EQ(16, c[3]);
        EXPECT_EQ(23, c[4]);
        EXPECT_EQ(42, c[5]);
    });
}

TEST(correctness, reserve_2)
{
    faulty_run([]
    {
        container c;
        c.push_back(4);
        c.push_back(8);
        c.push_back(15);
        c.push_back(16);
        c.push_back(23);
        c.push_back(42);
        c.reserve(100);

        EXPECT_EQ(4, c[0]);
        EXPECT_EQ(8, c[1]);
        EXPECT_EQ(15, c[2]);
        EXPECT_EQ(16, c[3]);
        EXPECT_EQ(23, c[4]);
        EXPECT_EQ(42, c[5]);
    });
}

TEST(correctness, front_back)
{
    faulty_run([]
    {
        container c;
        c.push_back(1);
        c.push_back(2);
        c.push_back(3);
        EXPECT_EQ(1, c.front());
        EXPECT_EQ(3, c.back());
    });
}

TEST(correctness, const_front_back)
{
    faulty_run([]
    {
        container c;
        c.push_back(1);
        c.push_back(2);
        c.push_back(3);

        container const& cc = c;
        EXPECT_EQ(1, cc.front());
        EXPECT_EQ(3, cc.back());
    });
}

TEST(correctness, front_back_ref)
{
    faulty_run([]
    {
        container c;
        c.push_back(1);
        c.push_back(2);
        c.push_back(3);
        EXPECT_EQ(&c.front(), &c[0]);
        EXPECT_EQ(&c.back(), &c[2]);
        c.front() = 10;
        c.back() = 20;
        EXPECT_EQ(10, c[0]);
        EXPECT_EQ(20, c[2]);
    });
}

TEST(correctness, const_front_back_ref)
{
    faulty_run([]
    {
        container c;
        c.push_back(1);
        c.push_back(2);
        c.push_back(3);
        
        container const& cc = c;
        EXPECT_EQ(&c.front(), &cc[0]);
        EXPECT_EQ(&c.back(), &cc[2]);
    });
}

TEST(correctness, push_back_element_of_itself)
{
    faulty_run([]
    {
        container c;
        c.push_back(0);
        c.push_back(1);
        
        for (size_t i = 0; i != 20; ++i)
            c.push_back(*(c.end() - 2));

        for (size_t i = 0; i != 22; ++i)
            EXPECT_EQ((int)i % 2, c[i]);
    });
}


TEST(correctness, push_back_element_of_itself_single)
{
    faulty_run([]
    {
        container c;
        c.push_back(42);
        c.push_back(c.front());
        c.push_back(c.front());
        c.push_back(c.front());
        c.push_back(c.front());
        
        for (size_t i = 0; i != c.size(); ++i)
            EXPECT_EQ(42, c[i]);
    });
}

TEST(correctness, insert_element_of_itself_1)
{
    faulty_run([]
    {
        container c;
        c.push_back(1);
        c.push_back(2);
        c.push_back(3);
        c.push_back(4);
        c.push_back(5);
        c.insert(c.begin() + 2, c[2]);

        EXPECT_EQ(6u, c.size());
        EXPECT_EQ(1, c[0]);
        EXPECT_EQ(2, c[1]);
        EXPECT_EQ(3, c[2]);
        EXPECT_EQ(3, c[3]);
        EXPECT_EQ(4, c[4]);
        EXPECT_EQ(5, c[5]);
    });
}

TEST(correctness, insert_element_of_itself_2)
{
    faulty_run([]
    {
        container c;
        c.push_back(1);
        c.push_back(2);
        c.push_back(3);
        c.push_back(4);
        c.push_back(5);
        c.insert(c.begin() + 3, c[2]);

        EXPECT_EQ(6u, c.size());
        EXPECT_EQ(1, c[0]);
        EXPECT_EQ(2, c[1]);
        EXPECT_EQ(3, c[2]);
        EXPECT_EQ(3, c[3]);
        EXPECT_EQ(4, c[4]);
        EXPECT_EQ(5, c[5]);
    });
}

TEST(correctness, insert_element_of_itself_single)
{
    faulty_run([]
    {
        container c;
        c.push_back(42);
        c.insert(c.begin(), c[0]);

        EXPECT_EQ(2u, c.size());
        for (size_t i = 0; i != 2; ++i)
            EXPECT_EQ(42, c[i]);
    });
}

TEST(exceptions, nothrow_default_ctor)
{
    faulty_run([]
    {
        EXPECT_NO_THROW(
        {
            container c;
        });
    });
}

TEST(exceptions, nothrow_subscript)
{
    faulty_run([]
    {
        container c;
        c.push_back(1);
        c.push_back(2);
        c.push_back(3);

        EXPECT_NO_THROW(
        {
            container const& cc = c;
            cc[0];
        });
    });
}

TEST(my_tests, resize_1)
{
    faulty_run([]
       {
           container c;
           for (int i = 1; i < 10; ++i)
               c.push_back(i);
           c.resize(5, 0);
           c.resize(8, 100);
           c.resize(10, 0);
           EXPECT_EQ(1, c[0]);
           EXPECT_EQ(2, c[1]);
           EXPECT_EQ(3, c[2]);
           EXPECT_EQ(4, c[3]);
           EXPECT_EQ(5, c[4]);
           EXPECT_EQ(100, c[5]);
           EXPECT_EQ(100, c[6]);
           EXPECT_EQ(100, c[7]);
           EXPECT_EQ(0, c[8]);
           EXPECT_EQ(0, c[9]);
       });
}

TEST(my_tests, resize_2)
{
    faulty_run([]
       {
           container c;
           for (int i = 1; i < 4; ++i)
               c.push_back(i);
           c.resize(5, 0);
           EXPECT_EQ(3, c[2]);
           EXPECT_EQ(1, c[0]);
           c.resize(1, 5);
           EXPECT_EQ(1u, c.size());
           EXPECT_EQ(1, c[0]);
           c.resize(5, 0);
           c.resize(0, 0);
       });
}

TEST(my_tests, clear) {
    faulty_run([]
       {
           container c;
           for (int i = 1; i < 10; ++i)
               c.push_back(i);
           c.resize(5, 0);
           c.resize(8, 100);
           c.resize(10, 0);
           EXPECT_EQ(10u, c.size());
           EXPECT_EQ(5, c[4]);
           EXPECT_EQ(100, c[5]);
           EXPECT_EQ(0, c[8]);
           c.clear();
           EXPECT_EQ(0u, c.size());
           c.push_back(1);
           EXPECT_EQ(1, c[0]);
       });
}

TEST(my_tests, swap_empty_empty) {
    faulty_run([]
       {
            container c;
            container d;
            swap(c, d);
            EXPECT_EQ(0u, c.size());
            EXPECT_EQ(0u, d.size());
            swap(d, c);
            EXPECT_EQ(0u, c.size());
            EXPECT_EQ(0u, d.size());
       });
}

TEST(my_tests, swap_empty_small) {
    faulty_run([]
       {
           container c;
           container d;
           d.push_back(1);
           swap(c, d);
           EXPECT_EQ(1u, c.size());
           EXPECT_EQ(1, c[0]);
           EXPECT_EQ(0u, d.size());
           swap(c, d);
           EXPECT_EQ(0u, c.size());
           EXPECT_EQ(1u, d.size());
           EXPECT_EQ(1, d[0]);
       });
}

TEST(my_tests, swap_small_small) {
    faulty_run([]
        {
            container c;
            container d;
            c.push_back(5);
            d.push_back(1);
            swap(c, d);
            EXPECT_EQ(1u, c.size());
            EXPECT_EQ(1u, d.size());
            EXPECT_EQ(1, c[0]);
            EXPECT_EQ(5, d[0]);
            swap(c, d);
            EXPECT_EQ(1u, c.size());
            EXPECT_EQ(1u, d.size());
            EXPECT_EQ(5, c[0]);
            EXPECT_EQ(1, d[0]);
        });
}

TEST(my_tests, swap_empty_big) {
    faulty_run([]
       {
           container c;
           container d;
           for (int i = 1; i < 10; ++i)
               d.push_back(i);
           swap(c, d);
           EXPECT_EQ(9u, c.size());
           EXPECT_EQ(0u, d.size());
           for (int i = 1; i < 10; ++i)
               EXPECT_EQ(i, c[i - 1]);
           swap(c, d);
           EXPECT_EQ(0u, c.size());
           EXPECT_EQ(9u, d.size());
           for (int i = 1; i < 10; ++i)
               EXPECT_EQ(i, d[i - 1]);
       });
}

TEST(my_tests, swap_small_big) {
    faulty_run([]
       {
           container c;
           container d;
           c.push_back(15);
           for (int i = 1; i < 8; i++)
               d.push_back(i);
           swap(c, d);
           EXPECT_EQ(7u, c.size());
           EXPECT_EQ(1u, d.size());
           for (int i = 1; i < 8; ++i)
               EXPECT_EQ(i, c[i - 1]);
           EXPECT_EQ(15, d[0]);
           swap(c, d);
           EXPECT_EQ(1u, c.size());
           EXPECT_EQ(7u, d.size());
           for (int i = 1; i < 8; ++i)
               EXPECT_EQ(i, d[i - 1]);
           EXPECT_EQ(15, c[0]);
       });
}

TEST(my_tests, swap_big_big) {
    faulty_run([]
       {
           container c;
           container d;
           for (int i = 5; i >= 0; --i)
               c.push_back(i);
           for (int i = 1; i < 8; i++)
               d.push_back(i);
           swap(c, d);
           EXPECT_EQ(7u, c.size());
           EXPECT_EQ(6u, d.size());
           for (int i = 1; i < 8; ++i)
               EXPECT_EQ(i, c[i - 1]);
           for (int i = 5; i >= 0; --i)
               EXPECT_EQ(i, d[5 - i]);
           swap(c, d);
           EXPECT_EQ(6u, c.size());
           EXPECT_EQ(7u, d.size());
           for (int i = 1; i < 8; ++i)
               EXPECT_EQ(i, d[i - 1]);
           for (int i = 5; i >= 0; --i)
               EXPECT_EQ(i, c[5 - i]);
       });
}

TEST(my_tests, some_operations) {
    faulty_run([]
       {
           container c;
           container d;
           for (int i = 5; i >= 0; --i)
               c.push_back(i);
           for (int i = 1; i < 8; i++)
               d.push_back(i);
           container e = d;
           swap(e, d);
           d.push_back(111);
           swap(c, d);
           container f = e;
           e.clear();
           f.pop_back();
           swap(e, f);
       });
}