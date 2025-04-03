cmake_minimum_required(VERSION 3.29)
project(DoTheFlop C)

set(CMAKE_C_STANDARD 17)

# Use pkg-config for SDL2 and SDL2_image
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

add_executable(DoTheFlop main.c)

target_link_directories(DoTheFlop PRIVATE ${SDL2_LIBRARY_DIRS} ${SDL2_IMAGE_LIBRARY_DIRS})
target_link_libraries(DoTheFlop PRIVATE ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})
