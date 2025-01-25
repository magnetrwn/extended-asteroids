cmake_minimum_required(VERSION 3.12.0)
project(asteroids)

add_definitions(-DSETTINGS_FILE="asteroids.ini")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(RAYLIB_PATH "${CMAKE_SOURCE_DIR}/lib/raylib")
include_directories(${RAYLIB_PATH}/include)
link_directories(${RAYLIB_PATH}/lib)

add_subdirectory(src)
add_subdirectory(lib/inipp)

add_compile_options(-Wall -Wextra -Wpedantic)
