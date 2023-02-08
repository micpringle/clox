cmake_minimum_required(VERSION 3.24)
project(clox C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_OSX_ARCHITECTURES "arm64")

# For profiling
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

# DEBUG_TRACE_EXECUTION
# DEBUG_STRESS_GC
# DEBUG_LOG_GC
# DEBUG_PRINT_CODE
# NAN_BOXING
add_compile_definitions(NAN_BOXING)

add_executable(clox main.c common.h chunk.h chunk.c memory.h memory.c debug.h debug.c value.h value.c vm.c vm.h compiler.c compiler.h scanner.c scanner.h object.c object.h table.c table.h)
