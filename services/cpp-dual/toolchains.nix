{ pkgs ? import <nixpkgs> {} }:

# ════════════════════════════════════════════════════════════
# Multi-Compiler Toolchain Configuration
# Demonstrates Nix's ability to isolate different compiler versions
# ════════════════════════════════════════════════════════════

{
  # ─────────────────────────────────────────────────────────
  # Physics Service: GCC 13 + C++17
  # ─────────────────────────────────────────────────────────
  physics-env = pkgs.mkShell {
    name = "physics-gcc13-cpp17";

    buildInputs = with pkgs; [
      gcc13                    # GCC 13 compiler
      cmake                    # Build system
      protobuf                 # Protobuf library
      grpc                     # gRPC library
      pkg-config               # Find libraries
    ];

    # Set GCC as the active compiler
    shellHook = ''
      export CC=${pkgs.gcc13}/bin/gcc
      export CXX=${pkgs.gcc13}/bin/g++
      export CMAKE_CXX_STANDARD=17
      echo "Physics environment: GCC 13 (C++17)"
      echo "Compiler: $(which g++)"
      $CXX --version | head -n1
    '';
  };

  # ─────────────────────────────────────────────────────────
  # Growth Service: Clang 16 + C++23
  # ─────────────────────────────────────────────────────────
  growth-env = pkgs.mkShell {
    name = "growth-clang16-cpp23";

    buildInputs = with pkgs; [
      clang_16                 # Clang 16 compiler
      llvmPackages_16.libcxx   # libc++ (Clang's C++ stdlib)
      llvmPackages_16.libcxxabi
      cmake
      protobuf
      grpc
      pkg-config
      # MQTT library for publishing
      mosquitto                # Provides libmosquitto
    ];

    # Set Clang as the active compiler with libc++
    shellHook = ''
      export CC=${pkgs.clang_16}/bin/clang
      export CXX=${pkgs.clang_16}/bin/clang++
      export CMAKE_CXX_STANDARD=23
      export CXXFLAGS="-stdlib=libc++"
      export LDFLAGS="-stdlib=libc++ -lc++abi"
      echo "Growth environment: Clang 16 (C++23)"
      echo "Compiler: $(which clang++)"
      $CXX --version | head -n1
    '';
  };

  # ─────────────────────────────────────────────────────────
  # Shared dependencies (for reference)
  # ─────────────────────────────────────────────────────────
  # Both environments include:
  # - protobuf: For message serialization
  # - grpc: For RPC (physics only, but including in both is fine)
  # - cmake: Build system
  # - pkg-config: Dependency discovery
}
