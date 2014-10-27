cmake_minimum_required(VERSION 2.8.4)
project(sample_class)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories("../lua-5.2.3/include")
link_directories("../lua-5.2.3/lib")

set(SOURCE_FILES main.cpp)
add_executable(sample_class ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} lua)