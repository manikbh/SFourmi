project(SFourmi)


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

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_image_DIR} ${SDL2_ttf_DIR})

add_subdirectory(sdl)
add_library(Os STATIC DisplayInfo.cpp  GraphXproc.cpp  GraphXtools.cpp  InitX.cpp UpdateFrame.cpp)
target_link_libraries(Os sdl  ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES})

