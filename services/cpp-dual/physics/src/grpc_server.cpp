#include "grpc_server.h"
#include <iostream>
#include <chrono>

namespace potato {

// ════════════════════════════════════════════════════════════
// PhysicsEngineServiceImpl
// ════════════════════════════════════════════════════════════

PhysicsEngineServiceImpl::PhysicsEngineServiceImpl(potato::PhysicsEngine* engine)
    : engine_(engine) {}

grpc::Status PhysicsEngineServiceImpl::CalculateMutation(
    grpc::ServerContext* context,
    const MutationRequest* request,
    MutationResponse* response) {

    std::cout << "[gRPC] CalculateMutation called for potato: "
              << request->potato_id() << std::endl;

    // TODO(human): Call engine_->CalculateMutation()
    // Pass request->current_growth() and request->environment()
    // Set response fields with the returned DNA

    // Example:
    // auto dna = engine_->CalculateMutation(
    //     request->current_growth(),
    //     request->environment());
    // *response->mutable_dna() = dna;
    // response->set_potato_id(request->potato_id());
    // response->set_mutation_occurred(dna.has_mutation());

    return grpc::Status::OK;
}

grpc::Status PhysicsEngineServiceImpl::SimulateGrowth(
    grpc::ServerContext* context,
    const GrowthSimulationRequest* request,
    grpc::ServerWriter<GrowthSimulationUpdate>* writer) {

    std::cout << "[gRPC] SimulateGrowth called for potato: "
              << request->potato_id() << std::endl;

    // TODO(human): Implement streaming simulation
    // This is a SERVER STREAMING RPC
    //
    // Pattern:
    // for (int day = 0; day < request->simulation_days(); ++day) {
    //     GrowthSimulationUpdate update;
    //     update.set_potato_id(request->potato_id());
    //     update.set_day(day);
    //
    //     // Simulate one day's growth
    //     // Set update.growth() with calculated values
    //
    //     writer->Write(update);  // Send update to client
    //
    //     // Check if client disconnected
    //     if (context->IsCancelled()) {
    //         return grpc::Status::CANCELLED;
    //     }
    // }

    return grpc::Status::OK;
}

grpc::Status PhysicsEngineServiceImpl::HealthCheck(
    grpc::ServerContext* context,
    const PhysicsHealthRequest* request,
    PhysicsHealthResponse* response) {

    std::cout << "[gRPC] HealthCheck called by: " << request->requester() << std::endl;

    // TODO(human): Populate health metrics from engine_
    response->set_healthy(true);
    response->set_version("1.0.0");
    // response->set_requests_processed(engine_->GetRequestCount());
    // response->set_avg_processing_time_ms(engine_->GetAverageProcessingTime());

    return grpc::Status::OK;
}

// ════════════════════════════════════════════════════════════
// GrpcServer
// ════════════════════════════════════════════════════════════

GrpcServer::GrpcServer(const std::string& server_address,
                       potato::PhysicsEngine* engine)
    : server_address_(server_address),
      service_(std::make_unique<PhysicsEngineServiceImpl>(engine)) {}

void GrpcServer::Run() {
    grpc::ServerBuilder builder;

    // Listen on address without authentication
    builder.AddListeningPort(server_address_, grpc::InsecureServerCredentials());

    // Register service
    builder.RegisterService(service_.get());

    // Build and start server
    server_ = builder.BuildAndStart();

    std::cout << "[gRPC] Server listening on " << server_address_ << std::endl;

    // Wait for server to shutdown
    server_->Wait();
}

void GrpcServer::Shutdown() {
    if (server_) {
        std::cout << "[gRPC] Shutting down server..." << std::endl;
        server_->Shutdown();
    }
}

} // namespace potato
