#include "vector_ops.hpp"

void vector_add_cpu(const float* a,
                    const float* b,
                    float* out,
                    std::size_t size)
{
    // Baseline implementation: one addition per loop iteration.
    for (std::size_t i = 0; i < size; ++i)
    {
        out[i] = a[i] + b[i];
    }
}
