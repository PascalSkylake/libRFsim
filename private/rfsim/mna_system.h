#pragma once

#include <Eigen/Dense>
#include <complex>

namespace rfsim::internal {

class MnaSystem {
public:
    explicit MnaSystem(int num_nodes)
        : A(Eigen::MatrixXcd::Zero(num_nodes, num_nodes)),
          z(Eigen::VectorXcd::Zero(num_nodes)) {}

    void stamp_admittance(int n1, int n2, std::complex<double> y) {
        if (n1 > 0) A(n1 - 1, n1 - 1) += y;
        if (n2 > 0) A(n2 - 1, n2 - 1) += y;
        if (n1 > 0 && n2 > 0) {
            A(n1 - 1, n2 - 1) -= y;
            A(n2 - 1, n1 - 1) -= y;
        }
    }

    void stamp_current(int node, std::complex<double> current) {
        if (node > 0) z(node - 1) += current;
    }

    Eigen::VectorXcd solve() const { return A.fullPivLu().solve(z); }

    const Eigen::MatrixXcd& matrix() const { return A; }

private:
    Eigen::MatrixXcd A;
    Eigen::VectorXcd z;
};

} // namespace rfsim::internal
