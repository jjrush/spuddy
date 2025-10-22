cmake_minimum_required(VERSION 3.15)
project(PhysicsService CXX)

# ════════════════════════════════════════════════════════════
# Physics Service: GCC 13 + C++17
# Features: std::thread, std::mutex, std::atomic, RAII
# ════════════════════════════════════════════════════════════

# C++17 required
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler warnings
add_compile_options(-Wall -Wextra -Wpedantic)

# Find dependencies
find_package(Protobuf REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(Threads REQUIRED)

# Link to our generated protobuf library
# (Assumes proto/CMakeLists.txt was built first)
find_library(POTATO_PROTO_LIB potato_proto HINTS /usr/local/lib)
include_directories(/usr/local/include/potato)

# Source files (to be created)
set(SOURCES
    src/main.cpp
    src/physics_engine.cpp
    src/grpc_server.cpp
)

# Create executable
add_executable(physics_service ${SOURCES})

# Link libraries
target_link_libraries(physics_service PRIVATE
    ${POTATO_PROTO_LIB}
    gRPC::grpc++
    gRPC::grpc++_reflection
    protobuf::libprotobuf
    Threads::Threads
)

# Include directories
target_include_directories(physics_service PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Install
install(TARGETS physics_service DESTINATION bin)
