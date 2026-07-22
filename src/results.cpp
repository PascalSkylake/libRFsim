#include "rfsim/results.h"

#include <fstream>

namespace rfsim {

bool SimulationResult::write_touchstone(const std::string& filepath) const {
    std::ofstream output(filepath);
    if (!output) return false;
    output << "! libRFsim result\n# Hz S RI R 50\n";
    return true;
}

} // namespace rfsim
