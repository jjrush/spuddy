# ════════════════════════════════════════════════════════════
# Multi-Stage Dockerfile: Dual C++ Services with Nix
# ════════════════════════════════════════════════════════════
# Stage 1: Physics Service (GCC 13, C++17)
# Stage 2: Growth Service (Clang 16, C++23)
# Stage 3: Runtime image with both binaries
# ════════════════════════════════════════════════════════════

# ─────────────────────────────────────────────────────────
# Base: Nix environment for builds
# ─────────────────────────────────────────────────────────
FROM nixos/nix:latest AS builder

# Enable flakes and nix-command (modern Nix features)
RUN mkdir -p /etc/nix && \
    echo "experimental-features = nix-command flakes" >> /etc/nix/nix.conf

# Install base dependencies
RUN nix-env -iA nixpkgs.git nixpkgs.which

# Create build directory
WORKDIR /build

# Copy Nix toolchain configuration
COPY toolchains.nix /build/

# Copy proto files (needed by both services)
COPY ../../proto /build/proto

# ─────────────────────────────────────────────────────────
# Build Proto Library First (shared by both services)
# ─────────────────────────────────────────────────────────
RUN nix-shell toolchains.nix -A physics-env --run "\
    cd /build/proto && \
    mkdir -p build && \
    cd build && \
    cmake .. && \
    make -j$(nproc) && \
    make install \
    "

# ─────────────────────────────────────────────────────────
# Stage 1: Build Physics Service (GCC 13, C++17)
# ─────────────────────────────────────────────────────────
FROM builder AS physics-builder

COPY physics/ /build/physics/

RUN nix-shell /build/toolchains.nix -A physics-env --run "\
    cd /build/physics && \
    mkdir -p build && \
    cd build && \
    cmake -DCMAKE_BUILD_TYPE=Release .. && \
    make -j$(nproc) \
    "

# ─────────────────────────────────────────────────────────
# Stage 2: Build Growth Service (Clang 16, C++23)
# ─────────────────────────────────────────────────────────
FROM builder AS growth-builder

COPY growth/ /build/growth/

RUN nix-shell /build/toolchains.nix -A growth-env --run "\
    cd /build/growth && \
    mkdir -p build && \
    cd build && \
    cmake -DCMAKE_BUILD_TYPE=Release .. && \
    make -j$(nproc) \
    "

# ─────────────────────────────────────────────────────────
# Stage 3: Runtime Image (minimal, with both binaries)
# ─────────────────────────────────────────────────────────
FROM debian:bookworm-slim AS runtime

# Install runtime dependencies only
RUN apt-get update && apt-get install -y \
    libprotobuf32 \
    libgrpc++1.51 \
    libmosquitto1 \
    && rm -rf /var/lib/apt/lists/*

# Copy built binaries from both build stages
COPY --from=physics-builder /build/physics/build/physics_service /usr/local/bin/
COPY --from=growth-builder /build/growth/build/growth_service /usr/local/bin/

# Make binaries executable
RUN chmod +x /usr/local/bin/physics_service /usr/local/bin/growth_service

# Verify binaries exist
RUN ls -lh /usr/local/bin/physics_service /usr/local/bin/growth_service

# Create non-root user for security
RUN useradd -r -s /bin/false potato && \
    mkdir -p /var/log/potato && \
    chown potato:potato /var/log/potato

USER potato

# EXPOSE ports (documentation only)
EXPOSE 50051

# Default to physics service (override in docker-compose)
CMD ["/usr/local/bin/physics_service"]
