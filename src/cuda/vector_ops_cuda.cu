#include "vector_ops.hpp"

#include <cuda_runtime.h>

#include <cstddef>
#include <stdexcept>
#include <string>

namespace
{
void check_cuda(cudaError_t status, const char* message)
{
    if (status != cudaSuccess)
    {
        throw std::runtime_error(std::string(message) + ": " + cudaGetErrorString(status));
    }
}

__global__ void vector_add_kernel(const float* a,
                                  const float* b,
                                  float* out,
                                  std::size_t size)
{
    // Each CUDA thread computes one output element.
    const std::size_t index = static_cast<std::size_t>(blockIdx.x) * blockDim.x + threadIdx.x;

    // The last block may have more threads than remaining elements.
    if (index < size)
    {
        out[index] = a[index] + b[index];
    }
}

}

void vector_add_cuda(const float* a,
                     const float* b,
                     float* out,
                     std::size_t size)
{
    if (size == 0)
    {
        return;
    }

    const std::size_t bytes = size * sizeof(float);

    float* device_a = nullptr;
    float* device_b = nullptr;
    float* device_out = nullptr;

    // Allocate memory on the GPU using the CUDA Runtime API.
    check_cuda(cudaMalloc(&device_a, bytes), "Failed to allocate device_a");
    check_cuda(cudaMalloc(&device_b, bytes), "Failed to allocate device_b");
    check_cuda(cudaMalloc(&device_out, bytes), "Failed to allocate device_out");

    // Copy input data from CPU memory to GPU memory.
    check_cuda(cudaMemcpy(device_a, a, bytes, cudaMemcpyHostToDevice), "Failed to copy a to GPU");
    check_cuda(cudaMemcpy(device_b, b, bytes, cudaMemcpyHostToDevice), "Failed to copy b to GPU");

    constexpr int threads_per_block = 256;
    const int blocks = static_cast<int>((size + threads_per_block - 1) / threads_per_block);

    // Launch enough CUDA threads to cover all vector elements.
    vector_add_kernel<<<blocks, threads_per_block>>>(device_a, device_b, device_out, size);
    check_cuda(cudaGetLastError(), "Failed to launch vector_add_kernel");

    // Wait for the GPU to finish before copying the result back.
    check_cuda(cudaDeviceSynchronize(), "Failed to synchronize after vector_add_kernel");

    // Copy the result from GPU memory back to CPU memory.
    check_cuda(cudaMemcpy(out, device_out, bytes, cudaMemcpyDeviceToHost), "Failed to copy result to CPU");

    // Free the GPU memory allocated with cudaMalloc.
    cudaFree(device_a);
    cudaFree(device_b);
    cudaFree(device_out);
}
