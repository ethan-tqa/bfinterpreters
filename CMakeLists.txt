cmake_minimum_required(VERSION 2.6)

project(bfinterpreter LANGUAGES C CXX ASM_MASM)

add_subdirectory("bfinterpreter")

set(CMAKE_ASM_SOURCE_FILE_EXTENSIONS asm)