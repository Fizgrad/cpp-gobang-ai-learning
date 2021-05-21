cmake_minimum_required(VERSION 3.17)
project(gobang)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,")
add_executable(gobang main.cpp board.cpp board.h coordinate.h main.cpp gobangAI.cpp gobangAI.h)
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")