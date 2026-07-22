#include "rfsim/simulator.h"

#include "rfsim/circuit_data.h"
#include "rfsim/mna_system.h"

#include <cmath>
#include <complex>

namespace rfsim {

SimulationResult Simulator::run_linear_sim(const Circuit& circuit, const Config& config) {
    SimulationResult result;
    if (!circuit.data_->valid || config.num_points <= 0 || config.stop_hz < config.start_hz) {
        result.error_message = "invalid simulation configuration";
        return result;
    }

    result.port_names.reserve(circuit.data_->ports.size());
    for (const auto& port : circuit.data_->ports) result.port_names.push_back(port.id);

    const int node_count = static_cast<int>(circuit.data_->node_map.size()) - 1;
    const auto points = config.num_points == 1 ? 1 : config.num_points;
    for (int point = 0; point < points; ++point) {
        const double fraction = points == 1 ? 0.0 : static_cast<double>(point) / (points - 1);
        const double frequency = config.start_hz + fraction * (config.stop_hz - config.start_hz);
        internal::MnaSystem system(node_count);
        for (const auto& resistor : circuit.data_->resistors)
            system.stamp_admittance(resistor.node1, resistor.node2, 1.0 / resistor.resistance);
        for (const auto& port : circuit.data_->ports)
            system.stamp_admittance(port.node, 0, 1.0 / port.z0);

        SParameterPoint s_point;
        s_point.frequency_hz = frequency;
        s_point.s_matrix.assign(circuit.data_->ports.size(),
                                std::vector<std::complex<double>>(circuit.data_->ports.size()));
        for (std::size_t excited = 0; excited < circuit.data_->ports.size(); ++excited) {
            internal::MnaSystem excited_system = system;
            const auto& source = circuit.data_->ports[excited];
            const auto source_current = 2.0 / std::sqrt(source.z0);
            excited_system.stamp_current(source.node, source_current);
            const auto voltages = excited_system.solve();
            for (std::size_t observed = 0; observed < circuit.data_->ports.size(); ++observed) {
                const auto& port = circuit.data_->ports[observed];
                const auto voltage = port.node > 0 ? voltages(port.node - 1) : std::complex<double>{};
                s_point.s_matrix[observed][excited] = voltage / std::sqrt(port.z0) -
                                                       (observed == excited ? 1.0 : 0.0);
            }
        }
        result.data.push_back(std::move(s_point));
    }
    result.success = true;
    return result;
}

} // namespace rfsim
