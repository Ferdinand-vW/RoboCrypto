set(SOURCES bypto/exchange/back_testing.cpp
            bypto/exchange/binance.cpp
            bypto/order.cpp
            bypto/order_type.cpp
            bypto/common/csv.cpp
            bypto/common/utils.cpp
            bypto/data/binance/klines.cpp)

include_directories(${SOURCE_INCLUDE_DIR})
include_directories(${BINAPI_INCLUDE_DIR})

add_library(byptoLib ${SOURCES})

add_definitions(
    -UNDEBUG
    -DDTF_HEADER_ONLY
    -D__FLATJSON__CHILDS_TYPE=std::uint32_t
    -D__FLATJSON__VLEN_TYPE=std::uint32_t
)

set(BINAPI_SOURCES
    ${BINAPI_DIR}/src/errors.cpp
    ${BINAPI_DIR}/src/api.cpp
    ${BINAPI_DIR}/src/enums.cpp
    ${BINAPI_DIR}/src/pairslist.cpp
    ${BINAPI_DIR}/src/reports.cpp
    ${BINAPI_DIR}/src/tools.cpp
    ${BINAPI_DIR}/src/types.cpp
    ${BINAPI_DIR}/src/websocket.cpp
)

ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)
find_package(Boost 1.76.0 REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

find_package(OpenSSL REQUIRED)
set(PostgreSQL_ADDITIONAL_VERSIONS "12.8")
find_package(PostgreSQL REQUIRED)

FetchContent_Declare(
   taocpp-taopq
   GIT_REPOSITORY https://github.com/taocpp/taopq
   GIT_TAG dee2c272de16286534cbc00919b34407198777d7
)
FetchContent_MakeAvailable(taocpp-taopq)


# Add source to this project's executable.
add_executable (bypto main.cpp ${SOURCES} ${BINAPI_SOURCES})
target_link_libraries(byptoLib taocpp::taopq)
target_include_directories(bypto PUBLIC ${BINAPI_INCLUDE_DIR})
target_link_libraries(bypto byptoLib)
target_link_libraries(bypto ${BOOST_LIBRARIES})
target_link_libraries(bypto OpenSSL::SSL)
target_link_libraries(bypto pthread)
target_link_libraries(bypto z)
target_link_libraries(bypto crypto)
target_link_libraries(bypto taocpp::taopq)

set_property(TARGET bypto PROPERTY CXX_STANDARD 17)
set_property(TARGET bypto PROPERTY CXX_STANDARD_REQUIRED ON)