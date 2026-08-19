cmake_minimum_required(VERSION 3.20)

project(accelerated_computing LANGUAGES CXX)

option(ENABLE_SIMD "Build SIMD implementations" ON)
option(ENABLE_CUDA "Build CUDA implementations" OFF)
option(ENABLE_TESTS "Build tests" ON)
option(ENABLE_BENCHMARKS "Build benchmarks" ON)

add_library(accelerated_computing
    src/cpu/vector_ops_cpu.cpp
    src/dispatch/vector_ops_dispatch.cpp
)

target_compile_features(accelerated_computing PUBLIC cxx_std_17)

target_include_directories(accelerated_computing
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(ENABLE_SIMD)
    target_sources(accelerated_computing
        PRIVATE
            src/simd/vector_ops_simd.cpp
    )

    target_compile_definitions(accelerated_computing
        PUBLIC
            ENABLE_SIMD
    )

    if(MSVC)
        target_compile_options(accelerated_computing
            PRIVATE
                $<$<COMPILE_LANGUAGE:CXX>:/arch:AVX2>
        )
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(accelerated_computing
            PRIVATE
                $<$<COMPILE_LANGUAGE:CXX>:-mavx2>
        )
    endif()
endif()

if(ENABLE_CUDA)
    enable_language(CUDA)

    if(NOT DEFINED CMAKE_CUDA_ARCHITECTURES)
        set(CMAKE_CUDA_ARCHITECTURES 89 CACHE STRING "CUDA GPU architectures")
    endif()

    target_sources(accelerated_computing
        PRIVATE
            src/cuda/vector_ops_cuda.cu
    )

    target_compile_definitions(accelerated_computing
        PUBLIC
            ENABLE_CUDA
    )

    set_target_properties(accelerated_computing
        PROPERTIES
            CUDA_STANDARD 17
            CUDA_STANDARD_REQUIRED ON
            CUDA_ARCHITECTURES ${CMAKE_CUDA_ARCHITECTURES}
            CUDA_RUNTIME_LIBRARY Shared
    )
endif()

add_executable(vector_add_example
    examples/vector_add_example.cpp
)

target_link_libraries(vector_add_example
    PRIVATE
        accelerated_computing
)

if(ENABLE_TESTS OR ENABLE_BENCHMARKS)
    include(FetchContent)
endif()

if(ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(ENABLE_BENCHMARKS)
    add_subdirectory(benchmarks)
endif()
