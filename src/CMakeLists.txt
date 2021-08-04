set(SOURCES robocrypto/robo_crypto.cpp)

include_directories(${SOURCE_INCLUDE_DIR})

add_library(RoboLib ${SOURCES})

# Add source to this project's executable.
add_executable (RoboCrypto main.cpp ${SOURCES})

target_link_libraries(RoboCrypto RoboLib)