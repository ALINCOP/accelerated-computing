#pragma once

enum class Backend
{
    // Plain C++ implementation.
    Cpu,

    // AVX2 implementation, enabled when ENABLE_SIMD is set by CMake.
    Simd
};
