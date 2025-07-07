#include <myproject_core/core.hpp>

namespace myproject_core
{

auto Factorial(int n) noexcept -> int
{
    int result = 1;

    while (n > 0)
    {
        result *= n;
        --n;
    }

    return result;
}

} // namespace myproject_core
