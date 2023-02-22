project(SFourmi)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_image_DIR} ${SDL2_ttf_DIR})

add_library(sdl STATIC SDLInterface.cpp)

target_link_libraries(sdl ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_ttf_LIBRARIES})

