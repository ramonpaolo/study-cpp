cmake_minimum_required(VERSION 3.28)
project(Snake_Game)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SDL3_DIR "/usr/local/lib/include/SDL3")

find_package(SDL3 REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} SDL3::SDL3)
