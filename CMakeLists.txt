cmake_minimum_required(VERSION 3.15)
project(lis_lab9 C)

set(CMAKE_C_STANDARD 99)

include_directories("inc")
include_directories("src")

add_library(gui src/gui.c)
add_executable(lis_lab9 src/main.c)

target_link_libraries(lis_lab9 ncurses)
target_link_libraries(lis_lab9 panel)
target_link_libraries(lis_lab9 gui)

