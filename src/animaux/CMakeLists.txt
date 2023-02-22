project(SFourmi)

add_subdirectory(autres)
add_subdirectory(fourmis)

add_library(animaux STATIC Animal.cpp)

target_link_libraries(animaux fourmis autres)

