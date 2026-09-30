cmake_minimum_required(VERSION 3.11)

project(psp-paf C CXX ASM)

add_library(psp-paf STATIC
    stubs.S
)

target_include_directories(psp-paf PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(BUILD_SAMPLES)
    add_subdirectory(samples)
endif()