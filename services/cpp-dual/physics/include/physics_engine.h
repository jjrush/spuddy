#pragma once

#include <atomic>
#include <mutex>
#include <thread>
#include <string>

// Generated protobuf headers
#include "potato.pb.h"
#include "environment.pb.h"

namespace potato {

// ════════════════════════════════════════════════════════════
// PhysicsEngine: C++17 Threading Demonstration
// ════════════════════════════════════════════════════════════
// Features to implement:
// - std::thread for background simulation
// - std::mutex for protecting shared state
// - std::atomic for lock-free counters
// - RAII for thread/lock management
// ════════════════════════════════════════════════════════════

class PhysicsEngine {
public:
    PhysicsEngine();
    ~PhysicsEngine();

    // Disable copy (threads aren't copyable)
    PhysicsEngine(const PhysicsEngine&) = delete;
    PhysicsEngine& operator=(const PhysicsEngine&) = delete;

    // Start/stop the background simulation thread
    void Start();
    void Stop();

    // Called by gRPC handlers
    PotatoDNA CalculateMutation(const PotatoGrowth& growth,
                                const EnvironmentSnapshot& env);

    PotatoGrowth GetCurrentGrowthState(const std::string& potato_id);

    // Metrics (for health checks)
    int64_t GetRequestCount() const;
    double GetAverageProcessingTime() const;

private:
    // TODO(human): Background thread function
    // This runs continuously, updating physics state
    void SimulationLoop();

    // TODO(human): Physics calculations
    // Calculate forces, mutations, etc.
    void UpdatePhysicsState();
    PotatoDNA ComputeMutation(const PotatoGrowth& growth,
                              const EnvironmentSnapshot& env);

    // ─────────────────────────────────────────────────────────
    // Threading primitives (C++17)
    // ─────────────────────────────────────────────────────────

    // TODO(human): Add std::thread for background simulation
    // std::thread simulation_thread_;

    // TODO(human): Add std::mutex to protect shared_state_
    // std::mutex state_mutex_;

    // TODO(human): Add std::atomic for request counting
    // std::atomic<int64_t> request_count_{0};

    // TODO(human): Add std::atomic<bool> for shutdown signal
    // std::atomic<bool> running_{false};

    // ─────────────────────────────────────────────────────────
    // Shared state (protected by mutex)
    // ─────────────────────────────────────────────────────────

    PotatoState shared_state_;

    // Metrics
    double total_processing_time_ms_;
};

} // namespace potato
