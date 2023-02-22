cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(SFourmi)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${SFourmi_SOURCE_DIR}/cmake")

add_subdirectory(animaux)
add_subdirectory(common)
add_subdirectory(terrain)

add_executable(SFourmi SFourmi.cpp)

target_link_libraries(SFourmi Os common animaux terrain)

