set(SOURCES bypto/robo_crypto.cpp)

include_directories(${SOURCE_INCLUDE_DIR})

add_library(byptoLib ${SOURCES})

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

# Add source to this project's executable.
add_executable (bypto main.cpp ${SOURCES} ${BINAPI_SOURCES})

target_include_directories(bypto PUBLIC ${BINAPI_INCLUDE_DIR})
target_link_libraries(bypto byptoLib)
target_link_libraries(bypto binApiLib)
target_link_libraries(bypto ${BOOST_LIBRARIES})

set_property(TARGET bypto PROPERTY CXX_STANDARD 17)
set_property(TARGET bypto PROPERTY CXX_STANDARD_REQUIRED ON)