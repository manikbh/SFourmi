cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(SFourmi)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${SFourmi_SOURCE_DIR}/cmake")

if( ${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    set(USE_FLAGS "-s USE_SDL=2 -s  USE_SDL_IMAGE=2  -s USE_SDL_TTF=2")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${USE_FLAGS}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${USE_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${USE_FLAGS}")
    set(CMAKE_EXECUTABLE_SUFFIX .html)
else()
    find_package(SDL2 REQUIRED)
    find_package(SDL2_image REQUIRED)
    find_package(SDL2_ttf REQUIRED)
endif()

add_subdirectory(animaux)
add_subdirectory(common)
add_subdirectory(terrain)

add_executable(SFourmi SFourmi.cpp)

target_link_libraries(SFourmi Os common animaux terrain  ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_ttf_LIBRARIES})

