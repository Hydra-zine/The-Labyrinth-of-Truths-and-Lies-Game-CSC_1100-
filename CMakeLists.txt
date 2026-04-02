cmake_minimum_required(VERSION 3.16)
project(my_game)

set(CMAKE_CXX_STANDARD 17)

# Tell CMake where Homebrew keeps things (required on Apple Silicon)
list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew")

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)

add_executable(my_game src/main.cpp)

target_link_libraries(my_game PRIVATE
    SDL2::SDL2
    SDL2_ttf::SDL2_ttf
    SDL2_image::SDL2_image
)
