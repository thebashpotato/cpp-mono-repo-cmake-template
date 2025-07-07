#include <cstdlib>
#include <iostream>
#include <myproject_core/config.hpp>
#include <myproject_core/core.hpp>

using namespace myproject_core;

auto main() -> int
{
    std::cout << myproject_name << " " << myproject_version << '\n';
    std::cout << "Factorial 10 = " << Factorial(10) << '\n';

    return EXIT_SUCCESS;
}
