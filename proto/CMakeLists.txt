cmake_minimum_required(VERSION 3.15)
project(PotatoProto)

# ════════════════════════════════════════════════════════════
# Protobuf & gRPC Code Generation for C++
# ════════════════════════════════════════════════════════════

# Find required packages
find_package(Protobuf REQUIRED)
find_package(gRPC CONFIG REQUIRED)

# Proto source files
set(PROTO_FILES
    potato.proto
    environment.proto
    services.proto
)

# Output directory for generated files
set(PROTO_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
file(MAKE_DIRECTORY ${PROTO_BINARY_DIR})

# Generated source files (will be created by protoc)
set(PROTO_SRCS)
set(PROTO_HDRS)
set(GRPC_SRCS)
set(GRPC_HDRS)

# Generate C++ code for each proto file
foreach(PROTO_FILE ${PROTO_FILES})
    get_filename_component(PROTO_NAME ${PROTO_FILE} NAME_WE)

    # Protobuf outputs
    list(APPEND PROTO_SRCS "${PROTO_BINARY_DIR}/${PROTO_NAME}.pb.cc")
    list(APPEND PROTO_HDRS "${PROTO_BINARY_DIR}/${PROTO_NAME}.pb.h")

    # gRPC outputs (only for services.proto)
    if(PROTO_FILE STREQUAL "services.proto")
        list(APPEND GRPC_SRCS "${PROTO_BINARY_DIR}/${PROTO_NAME}.grpc.pb.cc")
        list(APPEND GRPC_HDRS "${PROTO_BINARY_DIR}/${PROTO_NAME}.grpc.pb.h")
    endif()
endforeach()

# Custom command to generate protobuf code
add_custom_command(
    OUTPUT ${PROTO_SRCS} ${PROTO_HDRS} ${GRPC_SRCS} ${GRPC_HDRS}
    COMMAND ${Protobuf_PROTOC_EXECUTABLE}
    ARGS
        --cpp_out=${PROTO_BINARY_DIR}
        --grpc_out=${PROTO_BINARY_DIR}
        --plugin=protoc-gen-grpc=$<TARGET_FILE:gRPC::grpc_cpp_plugin>
        -I ${CMAKE_CURRENT_SOURCE_DIR}
        ${PROTO_FILES}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS ${PROTO_FILES}
    COMMENT "Generating C++ protobuf and gRPC code"
    VERBATIM
)

# Create a library with the generated code
add_library(potato_proto STATIC
    ${PROTO_SRCS}
    ${PROTO_HDRS}
    ${GRPC_SRCS}
    ${GRPC_HDRS}
)

# Include directories
target_include_directories(potato_proto PUBLIC
    ${PROTO_BINARY_DIR}
    ${Protobuf_INCLUDE_DIRS}
)

# Link protobuf and gRPC libraries
target_link_libraries(potato_proto PUBLIC
    protobuf::libprotobuf
    gRPC::grpc++
    gRPC::grpc++_reflection
)

# C++17 required (matching our C++ Physics service)
target_compile_features(potato_proto PUBLIC cxx_std_17)

# ════════════════════════════════════════════════════════════
# Installation (optional - for system-wide install)
# ════════════════════════════════════════════════════════════

install(TARGETS potato_proto
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)

install(FILES ${PROTO_HDRS} ${GRPC_HDRS}
    DESTINATION include/potato
)

# ════════════════════════════════════════════════════════════
# Convenience targets for validation
# ════════════════════════════════════════════════════════════

# Target to just generate code without building library
add_custom_target(generate_proto
    DEPENDS ${PROTO_SRCS} ${PROTO_HDRS} ${GRPC_SRCS} ${GRPC_HDRS}
    COMMENT "Proto code generation complete"
)

# Print generated file locations (for debugging)
add_custom_target(show_proto_files
    COMMAND ${CMAKE_COMMAND} -E echo "Generated files in: ${PROTO_BINARY_DIR}"
    COMMAND ${CMAKE_COMMAND} -E echo "Proto sources: ${PROTO_SRCS}"
    COMMAND ${CMAKE_COMMAND} -E echo "gRPC sources: ${GRPC_SRCS}"
    VERBATIM
)
