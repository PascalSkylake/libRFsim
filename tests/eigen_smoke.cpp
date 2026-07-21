#include <cassert>

#include <Eigen/Dense>

int main() {
    Eigen::Matrix2d matrix;
    matrix << 1.0, 2.0,
              3.0, 4.0;

    assert(matrix.determinant() == -2.0);
    return 0;
}
