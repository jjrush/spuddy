#pragma once

#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>

#include "services.grpc.pb.h"
#include "physics_engine.h"

namespace potato {

// ════════════════════════════════════════════════════════════
// PhysicsEngineServiceImpl: gRPC Service Implementation
// ════════════════════════════════════════════════════════════

class PhysicsEngineServiceImpl final : public PhysicsEngine::Service {
public:
    explicit PhysicsEngineServiceImpl(potato::PhysicsEngine* engine);

    // ─────────────────────────────────────────────────────────
    // gRPC method implementations
    // ─────────────────────────────────────────────────────────

    grpc::Status CalculateMutation(
        grpc::ServerContext* context,
        const MutationRequest* request,
        MutationResponse* response) override;

    grpc::Status SimulateGrowth(
        grpc::ServerContext* context,
        const GrowthSimulationRequest* request,
        grpc::ServerWriter<GrowthSimulationUpdate>* writer) override;

    grpc::Status HealthCheck(
        grpc::ServerContext* context,
        const PhysicsHealthRequest* request,
        PhysicsHealthResponse* response) override;

private:
    potato::PhysicsEngine* engine_;  // Not owned
};

// ════════════════════════════════════════════════════════════
// GrpcServer: Manages server lifecycle
// ════════════════════════════════════════════════════════════

class GrpcServer {
public:
    GrpcServer(const std::string& server_address,
               potato::PhysicsEngine* engine);

    void Run();
    void Shutdown();

private:
    std::string server_address_;
    std::unique_ptr<grpc::Server> server_;
    std::unique_ptr<PhysicsEngineServiceImpl> service_;
};

} // namespace potato
