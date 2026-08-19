#pragma once

#include "backend.hpp"

#include <cstddef>

void vector_add(Backend backend,
                const float* a,
                const float* b,
                float* out,
                std::size_t size);

// Reference implementation using plain C++ loops.
void vector_add_cpu(const float* a,
                    const float* b,
                    float* out,
                    std::size_t size);

#ifdef ENABLE_SIMD
// SIMD implementation using AVX2 intrinsics.
void vector_add_simd(const float* a,
                     const float* b,
                     float* out,
                     std::size_t size);
#endif

#ifdef ENABLE_CUDA
// CUDA implementation using a GPU kernel.
void vector_add_cuda(const float* a,
                     const float* b,
                     float* out,
                     std::size_t size);
#endif
