# CMakeList.txt : CMake project for RoboCrypto, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.21)

project ("bypto")

if(WIN32)
    add_definitions(-D_WIN32_WINNT=0x0601)
endif()
    


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
set(BINAPI_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/libs/binapi/include)
set(BINAPI_DIR ${CMAKE_CURRENT_LIST_DIR}/libs/binapi)

include(FetchContent)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/libs/binapi)
add_subdirectory(src)
# TODO: Add tests and install targets if needed.
