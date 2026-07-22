#pragma once

#include "rfsim/circuit.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace rfsim {

struct Circuit::CircuitData {
    struct Resistor { std::string id; int node1; int node2; double resistance; };
    struct Port { std::string id; int node; double z0; };
    std::unordered_map<std::string, int> node_map{{"gnd", 0}, {"0", 0}};
    std::vector<Resistor> resistors;
    std::vector<Port> ports;
    bool valid{false};
};

} // namespace rfsim
