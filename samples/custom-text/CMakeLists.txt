cmake_minimum_required(VERSION 3.11)

project(custom-text C CXX ASM)

file(GLOB MODULE_SOURCES CONFIGURE_DEPENDS
    *.cpp
    *.exp
)

add_prx_module(custom-text
    ${MODULE_SOURCES} SystemCtrlForUser.S
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

target_link_libraries(custom-text PRIVATE
    psp-paf
)

target_compile_options(custom-text PRIVATE
    -Os
    -G0 # need so our hook is not fucking everything
    $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions -fno-rtti -fno-unwind-tables -fno-asynchronous-unwind-tables -fno-threadsafe-statics -fno-use-cxa-atexit>
)

target_link_options(custom-text PRIVATE 
    -nostdlib++
)