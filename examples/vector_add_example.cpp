#include "backend.hpp"
#include "vector_ops.hpp"

#include <iostream>
#include <vector>

int main()
{
    const std::vector<float> a{1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
    const std::vector<float> b{10.0f, 20.0f, 30.0f, 40.0f, 50.0f};
    std::vector<float> out(a.size());

    vector_add(Backend::Cpu, a.data(), b.data(), out.data(), out.size());

    std::cout << "CPU: ";
    for (float value : out)
    {
        std::cout << value << ' ';
    }
    std::cout << '\n';

#ifdef ENABLE_SIMD
    vector_add(Backend::Simd, a.data(), b.data(), out.data(), out.size());

    std::cout << "SIMD: ";
    for (float value : out)
    {
        std::cout << value << ' ';
    }
    std::cout << '\n';
#endif

    return 0;
}
