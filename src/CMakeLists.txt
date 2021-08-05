set(SOURCES bypto/robo_crypto.cpp)

include_directories(${SOURCE_INCLUDE_DIR})

add_library(byptoLib ${SOURCES})

# Add source to this project's executable.
add_executable (bypto main.cpp ${SOURCES})

target_link_libraries(bypto byptoLib)