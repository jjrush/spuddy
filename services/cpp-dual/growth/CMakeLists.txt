cmake_minimum_required(VERSION 3.20)
project(GrowthService CXX)

# ════════════════════════════════════════════════════════════
# Growth Service: Clang 16 + C++23
# Features: std::jthread, std::stop_token, std::expected, ranges
# ════════════════════════════════════════════════════════════

# C++23 required
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Use libc++ (Clang's stdlib) for C++23 features
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -lc++abi")

# Compiler warnings
add_compile_options(-Wall -Wextra -Wpedantic)

# Find dependencies
find_package(Protobuf REQUIRED)
find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)

# Find MQTT library
pkg_check_modules(MOSQUITTO REQUIRED libmosquitto)

# Link to our generated protobuf library
find_library(POTATO_PROTO_LIB potato_proto HINTS /usr/local/lib)
include_directories(/usr/local/include/potato)

# Source files (to be created)
set(SOURCES
    src/main.cpp
    src/growth_simulator.cpp
    src/mqtt_publisher.cpp
)

# Create executable
add_executable(growth_service ${SOURCES})

# Link libraries
target_link_libraries(growth_service PRIVATE
    ${POTATO_PROTO_LIB}
    protobuf::libprotobuf
    Threads::Threads
    ${MOSQUITTO_LIBRARIES}
)

# Include directories
target_include_directories(growth_service PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${MOSQUITTO_INCLUDE_DIRS}
)

# Install
install(TARGETS growth_service DESTINATION bin)
