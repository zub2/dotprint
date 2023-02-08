cmake_minimum_required(VERSION 3.1)
cmake_policy(SET CMP0135 NEW)
project(dotprint)
enable_testing()

# use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIBMM REQUIRED IMPORTED_TARGET glibmm-2.4)
pkg_check_modules(CAIROMM REQUIRED IMPORTED_TARGET cairomm-1.0)

add_subdirectory(src)
add_subdirectory(test)
