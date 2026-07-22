#include <cassert>
#include <cmath>
#include <complex>
#include <string>

#include <rfsim/circuit.h>
#include <rfsim/results.h>
#include <rfsim/simulator.h>

int main() {
    rfsim::Circuit circuit;
    assert(!circuit.load_json_string("{}"));
    circuit.add_resistor("R1", "n1", "gnd", 50.0);

    assert(circuit.load_json_string(R"({
        "version": "1.0",
        "ports": [{"id":"P1", "node":"n1", "z0_ohms":50.0},
                   {"id":"P2", "node":"n2", "z0_ohms":50.0}],
        "components": [{"type":"resistor", "id":"R1", "node1":"n1", "node2":"n2", "val_ohms":50.0}]
    })"));

    const auto result = rfsim::Simulator::run_linear_sim(
        circuit, {.start_hz = 1.0e9, .stop_hz = 2.0e9, .num_points = 2});
    assert(result.success);
    assert(result.data.size() == 2);
    assert(std::abs(result.data.front().s_matrix[0][0] - std::complex<double>(1.0 / 3.0, 0.0)) < 1e-12);
    assert(std::abs(result.data.front().s_matrix[1][0] - std::complex<double>(2.0 / 3.0, 0.0)) < 1e-12);
    assert(!rfsim::Simulator::run_linear_sim(circuit, {}).success);
    return 0;
}
