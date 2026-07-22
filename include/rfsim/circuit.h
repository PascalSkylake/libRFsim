#pragma once

#include <memory>
#include <string>

namespace rfsim {

class Circuit {
public:
    struct CircuitData;

    Circuit();
    ~Circuit();

    Circuit(Circuit&&) noexcept;
    Circuit& operator=(Circuit&&) noexcept;
    Circuit(const Circuit&) = delete;
    Circuit& operator=(const Circuit&) = delete;

    bool load_json(const std::string& filepath);
    bool load_json_string(const std::string& json_content);

    void add_resistor(const std::string& id, const std::string& node1,
                      const std::string& node2, double resistance_ohms);

private:
    std::unique_ptr<CircuitData> data_;

    friend class Simulator;
};

} // namespace rfsim
