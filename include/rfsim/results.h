#pragma once

#include <complex>
#include <string>
#include <vector>

namespace rfsim {

struct SParameterPoint {
    double frequency_hz{};
    std::vector<std::vector<std::complex<double>>> s_matrix;
};

struct SimulationResult {
    bool success{};
    std::string error_message;
    std::vector<std::string> port_names;
    std::vector<SParameterPoint> data;

    bool write_touchstone(const std::string& filepath) const;
};

} // namespace rfsim
