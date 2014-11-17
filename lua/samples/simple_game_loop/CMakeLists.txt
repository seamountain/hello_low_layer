cmake_minimum_required(VERSION 2.8.4)
project(simple_game_loop)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# g++ game_loop.cpp -o result.out -I./lua-5.2.3/include -L./lua-5.2.3/lib -llua -g `sdl2-config --cflags` `sdl2-config --libs`

include_directories("libs/lua-5.2.3/include" "libs/SDL2-2.0.3/include")
link_directories("libs/lua-5.2.3/lib")

set(SOURCE_FILES main.cpp lua/update.lua)
add_executable(simple_game_loop ${SOURCE_FILES})


# SDL2
# http://stackoverflow.com/questions/23850472/how-to-use-sdl2-and-sdl-image-with-cmake
INCLUDE(FindPkgConfig)

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${SDL2_LIBRARIES})


# Lua
TARGET_LINK_LIBRARIES(${PROJECT_NAME} lua)