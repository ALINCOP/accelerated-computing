#include "vector_ops.hpp"

#include <immintrin.h>

void vector_add_simd(const float* a,
                     const float* b,
                     float* out,
                     std::size_t size)
{
    // AVX2 registers hold 8 floats: 256 bits / 32 bits per float.
    constexpr std::size_t simd_width = 8;

    std::size_t i = 0;

    // Process 8 elements at a time using unaligned loads/stores.
    for (; i + simd_width <= size; i += simd_width)
    {
        // Load 8 floats from a starting at index i.
        // const means this temporary SIMD value is not reassigned inside this iteration.
        const __m256 va = _mm256_loadu_ps(a + i);

        // Load 8 floats from b starting at index i.
        // The next loop iteration creates a new vb with the next 8 values.
        const __m256 vb = _mm256_loadu_ps(b + i);

        // Add the 8 pairs of floats in parallel.
        // A new result is created on each loop iteration.
        // const only prevents reassignment within the current iteration.
        const __m256 result = _mm256_add_ps(va, vb);

        // Store the 8 computed floats into out starting at index i.
        _mm256_storeu_ps(out + i, result);
    }

    // Process any remaining elements when size is not divisible by 8.
    for (; i < size; ++i)
    {
        out[i] = a[i] + b[i];
    }
}
