#include "backend.hpp"
#include "vector_ops.hpp"

#include <gtest/gtest.h>

#include <cstddef>
#include <vector>

namespace
{
std::vector<float> make_test_vector(std::size_t size, float start_value)
{
    std::vector<float> values(size);

    // Generate predictable values so the test is easy to debug.
    for (std::size_t i = 0; i < size; ++i)
    {
        values[i] = start_value + static_cast<float>(i);
    }

    return values;
}

void expect_vectors_near(const std::vector<float>& expected,
                         const std::vector<float>& actual,
                         float epsilon)
{
    // Float operations can differ slightly between implementations,
    // so compare with a small tolerance instead of exact equality.
    ASSERT_EQ(expected.size(), actual.size());

    for (std::size_t i = 0; i < expected.size(); ++i)
    {
        EXPECT_NEAR(expected[i], actual[i], epsilon) << "Mismatch at index " << i;
    }
}
}

TEST(VectorAddCpu, AddsSmallVector)
{
    const std::vector<float> a{1.0f, 2.0f, 3.0f};
    const std::vector<float> b{10.0f, 20.0f, 30.0f};
    std::vector<float> out(a.size());

    vector_add(Backend::Cpu, a.data(), b.data(), out.data(), out.size());

    const std::vector<float> expected{11.0f, 22.0f, 33.0f};
    EXPECT_EQ(out, expected);
}

#ifdef ENABLE_SIMD
TEST(VectorAddSimd, MatchesCpuForImportantSizes)
{
    // These sizes cover only-tail, exact AVX2 blocks, and AVX2 blocks with tail.
    const std::vector<std::size_t> sizes{0, 1, 5, 8, 10, 1024, 1027};

    for (std::size_t size : sizes)
    {
        const std::vector<float> a = make_test_vector(size, 1.0f);
        const std::vector<float> b = make_test_vector(size, 100.0f);

        std::vector<float> cpu_out(size);
        std::vector<float> simd_out(size);

        // CPU is the reference result.
        vector_add(Backend::Cpu, a.data(), b.data(), cpu_out.data(), size);

        // SIMD should produce the same result as CPU.
        vector_add(Backend::Simd, a.data(), b.data(), simd_out.data(), size);

        expect_vectors_near(cpu_out, simd_out, 1e-5f);
    }
}
#endif
