#include <iostream>
#include <csignal>
#include <memory>

#include "physics_engine.h"
#include "grpc_server.h"

// Global server pointer for signal handler
std::unique_ptr<potato::GrpcServer> g_server;

void SignalHandler(int signal) {
    std::cout << "\n[Main] Received signal " << signal << ", shutting down..." << std::endl;
    if (g_server) {
        g_server->Shutdown();
    }
}

int main(int argc, char** argv) {
    std::cout << "════════════════════════════════════════════════" << std::endl;
    std::cout << "  Potato Physics Service (C++17 / GCC 13)" << std::endl;
    std::cout << "════════════════════════════════════════════════" << std::endl;

    // Register signal handlers for graceful shutdown
    std::signal(SIGINT, SignalHandler);
    std::signal(SIGTERM, SignalHandler);

    // Get configuration from environment
    const char* grpc_port_env = std::getenv("GRPC_PORT");
    std::string grpc_port = grpc_port_env ? grpc_port_env : "50051";
    std::string server_address = "0.0.0.0:" + grpc_port;

    try {
        // Create physics engine
        potato::PhysicsEngine engine;

        // Start background simulation thread
        engine.Start();

        // Create and run gRPC server
        g_server = std::make_unique<potato::GrpcServer>(server_address, &engine);

        std::cout << "[Main] Starting gRPC server..." << std::endl;
        g_server->Run();  // Blocks until shutdown

        // Cleanup
        std::cout << "[Main] Server stopped, cleaning up..." << std::endl;
        engine.Stop();

    } catch (const std::exception& e) {
        std::cerr << "[Main] Fatal error: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "[Main] Shutdown complete" << std::endl;
    return 0;
}
