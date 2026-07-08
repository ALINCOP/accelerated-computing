#include "vector_ops.hpp"

#include <stdexcept>

void vector_add(Backend backend,
                const float* a,
                const float* b,
                float* out,
                std::size_t size)
{
    // Choose the implementation based on the requested backend.
    switch (backend)
    {
        case Backend::Cpu:
            vector_add_cpu(a, b, out, size);
            return;

#ifdef ENABLE_SIMD
        case Backend::Simd:
            vector_add_simd(a, b, out, size);
            return;
#endif
    }

    throw std::runtime_error("Selected backend is not available in this build");
}
