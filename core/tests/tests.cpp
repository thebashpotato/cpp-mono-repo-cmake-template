#include <gtest/gtest.h>

#include <myproject_core/config.hpp>
#include <myproject_core/core.hpp>

using namespace myproject_core;

TEST(FactorialTestSuite, Test1)
{
    ASSERT_EQ(Factorial(0), 1);
    ASSERT_EQ(Factorial(1), 1);
    ASSERT_EQ(Factorial(2), 2);
    ASSERT_EQ(Factorial(3), 6);
    ASSERT_EQ(Factorial(10), 3628800);
}

TEST(ConfigFileTestSuite, Test1)
{
    ASSERT_EQ(NAME, "myproject_core");
    ASSERT_EQ(VERSION_MAJOR, 0);
    ASSERT_EQ(VERSION_MINOR, 1);
    ASSERT_EQ(VERSION_PATCH, 0);
}
