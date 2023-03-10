cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(SFourmi)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -fsanitize=undefined -fsanitize=address -fno-omit-frame-pointer -fsanitize=leak -fsanitize=null -fsanitize=return -fsanitize=signed-integer-overflow -fsanitize=bounds-strict -fsanitize=object-size -fsanitize=enum -fsanitize-address-use-after-scope")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${SFourmi_SOURCE_DIR}/cmake/")


include_directories(include)
add_subdirectory(lib)
add_subdirectory(src)

file(COPY images DESTINATION ${CMAKE_BINARY_DIR})
file(COPY docs DESTINATION ${CMAKE_BINARY_DIR})
