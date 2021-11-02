
ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)
find_package(Boost 1.76.0 REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

find_package(OpenSSL REQUIRED)
set(PostgreSQL_ADDITIONAL_VERSIONS "12.8")
find_package(PostgreSQL REQUIRED)

include(ExternalProject)
set(taocpp_SOURCE_DIR ${BUILD_DIR}/_deps/taocpp-src)
set(taocpp_BINARY_DIR ${BUILD_DIR}/_deps/taocpp-build)
set(taocpp_LIB ${taocpp_BINARY_DIR}/libtaopq.a)
ExternalProject_Add(
    taocpp
    GIT_REPOSITORY https://github.com/taocpp/taopq
    GIT_TAG dee2c272de16286534cbc00919b34407198777d7
    SOURCE_DIR ${taocpp_SOURCE_DIR}
    BINARY_DIR ${taocpp_BINARY_DIR}
    INSTALL_COMMAND ""
    BUILD_BYPRODUCTS ${taocpp_LIB}
    CMAKE_ARGS -DCMAKE_CXX_STANDARD=20
)



set(binapi_SOURCE_DIR ${BUILD_DIR}/_deps/binapi-src)
set(binapi_BINARY_DIR ${BUILD_DIR}/_deps/binapi-build)
set(binapi_LIB ${binapi_BINARY_DIR}/libbinapi_lib.a)
ExternalProject_Add(
    binapi
    GIT_REPOSITORY https://github.com/Ferdinand-vW/binapi
    GIT_TAG cf09db45b78707c995017a7777fdbc9a142f5372
    SOURCE_DIR ${binapi_SOURCE_DIR}
    BINARY_DIR ${binapi_BINARY_DIR}
    INSTALL_COMMAND ""
    BUILD_BYPRODUCTS ${binapi_LIB}
    CMAKE_ARGS -DCMAKE_CXX_STANDARD=20
)

set(SOURCES bypto/account.cpp
            bypto/exchange/back_testing.cpp
            bypto/exchange/binance.cpp
            bypto/order/generic.cpp
            bypto/order/order_type.cpp
            bypto/order/fill.cpp
            bypto/order/types.cpp
            bypto/common/csv.cpp
            bypto/common/utils.cpp
            bypto/common/types.cpp
            bypto/data/kline.cpp
            bypto/data/binance.cpp)

include_directories(${SOURCE_INCLUDE_DIR})
include_directories(${PostgreSQL_INCLUDE_DIRS})


add_library(byptoObj OBJECT ${SOURCES})
add_dependencies(byptoObj binapi taocpp)
target_include_directories(byptoObj PUBLIC ${taocpp_SOURCE_DIR}/include)
target_include_directories(byptoObj PUBLIC ${binapi_SOURCE_DIR}/include)

add_library(byptoLib STATIC $<TARGET_OBJECTS:byptoObj>)
target_include_directories(byptoLib PUBLIC ${taocpp_SOURCE_DIR}/include)
target_include_directories(byptoLib PUBLIC ${binapi_SOURCE_DIR}/include)

# Add source to this project's executable.
add_executable (bypto main.cpp $<TARGET_OBJECTS:byptoObj>)
target_include_directories(bypto PUBLIC ${taocpp_SOURCE_DIR}/include)
target_include_directories(bypto PUBLIC ${binapi_SOURCE_DIR}/include)

target_link_libraries(byptoObj ${binapi_LIB})
target_link_libraries(byptoObj ${taocpp_LIB})
target_link_libraries(byptoLib ${PostgreSQL_LIBRARIES})
target_link_libraries(bypto byptoLib)
target_link_libraries(bypto ${BOOST_LIBRARIES})
target_link_libraries(bypto dl)
target_link_libraries(bypto OpenSSL::SSL)
target_link_libraries(bypto pthread)
target_link_libraries(bypto z)
target_link_libraries(bypto crypto)
target_link_libraries(bypto ${taocpp_LIB})
target_link_libraries(bypto ${binapi_LIB})

set_property(TARGET byptoLib PROPERTY CXX_STANDARD 20)
set_property(TARGET byptoLib PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET bypto PROPERTY CXX_STANDARD 20)
set_property(TARGET bypto PROPERTY CXX_STANDARD_REQUIRED ON)