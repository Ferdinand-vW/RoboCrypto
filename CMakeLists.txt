# CMakeList.txt : CMake project for RoboCrypto, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("RoboCrypto")

set(SOURCE_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)


add_subdirectory(src)
# TODO: Add tests and install targets if needed.
