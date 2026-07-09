cmake_minimum_required(VERSION 3.20)

project(accelerated_computing LANGUAGES CXX)

option(ENABLE_SIMD "Build SIMD implementations" ON)
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
                /arch:AVX2
        )
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(accelerated_computing
            PRIVATE
                -mavx2
        )
    endif()
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
