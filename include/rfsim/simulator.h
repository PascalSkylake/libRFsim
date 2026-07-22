#pragma once

#include "rfsim/circuit.h"
#include "rfsim/results.h"

namespace rfsim {

class Simulator {
public:
    struct Config {
        double start_hz{};
        double stop_hz{};
        int num_points{};
    };

    static SimulationResult run_linear_sim(const Circuit& circuit, const Config& config);
};

} // namespace rfsim
