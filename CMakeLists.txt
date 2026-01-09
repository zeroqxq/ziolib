cmake_minimum_required(VERSION 3.16)
project(ziolib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_executable(ziotest
    examples/main.cpp
)

target_include_directories(ziotest PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ziotest PRIVATE ncurses)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

