project(SFourmi)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_image_DIR} ${SDL2_ttf_DIR})

add_subdirectory(sdl)
add_library(Os STATIC DisplayInfo.cpp  GraphXproc.cpp  GraphXtools.cpp  InitX.cpp UpdateFrame.cpp)
target_link_libraries(Os sdl  ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES})

