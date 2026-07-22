#include "rfsim/circuit_data.h"

#include <nlohmann/json.hpp>

#include <fstream>
#include <iterator>

namespace rfsim {

namespace {
int node_index(Circuit::CircuitData& data, const std::string& name) {
    if (name.empty()) return -1;
    if (const auto found = data.node_map.find(name); found != data.node_map.end())
        return found->second;
    const int index = static_cast<int>(data.node_map.size()) - 1;
    data.node_map.emplace(name, index);
    return index;
}
}

Circuit::Circuit() : data_(std::make_unique<CircuitData>()) {}
Circuit::~Circuit() = default;
Circuit::Circuit(Circuit&&) noexcept = default;
Circuit& Circuit::operator=(Circuit&&) noexcept = default;

bool Circuit::load_json(const std::string& filepath) {
    std::ifstream input(filepath);
    if (!input) return false;
    return load_json_string({std::istreambuf_iterator<char>(input), {}});
}

bool Circuit::load_json_string(const std::string& content) {
    try {
        const auto json = nlohmann::json::parse(content);
        if (json.value("version", "") != "1.0" || !json.contains("components")) return false;

        auto parsed = std::make_unique<CircuitData>();
        for (const auto& port : json.value("ports", nlohmann::json::array())) {
            if (!port.contains("id") || !port.contains("node") || !port.contains("z0_ohms")) return false;
            node_index(*parsed, port.at("node").get<std::string>());
            const double z0 = port.at("z0_ohms").get<double>();
            if (z0 <= 0.0) return false;
            parsed->ports.push_back({port.at("id").get<std::string>(),
                                     node_index(*parsed, port.at("node").get<std::string>()), z0});
        }
        for (const auto& component : json.at("components")) {
            if (component.value("type", "") != "resistor") return false;
            const auto id = component.at("id").get<std::string>();
            const auto n1 = node_index(*parsed, component.at("node1").get<std::string>());
            const auto n2 = node_index(*parsed, component.at("node2").get<std::string>());
            const double resistance = component.at("val_ohms").get<double>();
            if (id.empty() || n1 < 0 || n2 < 0 || resistance <= 0.0) return false;
            parsed->resistors.push_back({id, n1, n2, resistance});
        }
        parsed->valid = true;
        data_ = std::move(parsed);
        return true;
    } catch (const nlohmann::json::exception&) {
        return false;
    }
}

void Circuit::add_resistor(const std::string& id, const std::string& node1,
                           const std::string& node2, double resistance_ohms) {
    const int n1 = node_index(*data_, node1);
    const int n2 = node_index(*data_, node2);
    if (!id.empty() && n1 >= 0 && n2 >= 0 && resistance_ohms > 0.0)
        data_->resistors.push_back({id, n1, n2, resistance_ohms});
}

} // namespace rfsim
